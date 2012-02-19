<?php

class manifest extends db {

    function __construct() {
        
    }

	function getData() {
		global $manServer, $manPort;
		if (!isset($this->data))
			$this->data = $this->getManifest($manServer, $manPort);
		
		return $this->data;
	}

	function install($module) {
		echo '<pre>';
		$this->l("Loading module '$module'...");
	    if($mod = core::load($module,true)) {
	       	$this->l("Ok");
	       	
	       	$this->l("Installing module:\n");
	       	if ($mod->install()) { 
				core::setStatus($mod->_id,'active',$mod->_version);
				$this->l("Module installed successfully!");
				echo '</pre>';
				return true;
			} else $this->l("<font color=\"#FF0000\">Module installation failed!</font>");
		} else $this->l("Failed");
		echo '</pre>';
		return false;
	}
	
	function activate($module) {
		echo '<pre>';
		$this->l("Loading module '$module'...");
	    if($mod = core::load($module,true)) {
	       	$this->l("Ok");
	       	
	       	$this->l("Activating module...");
	       	if (core::setStatus($mod->_id,'active')) {
				$this->l("Ok");
				echo '</pre>';
				return true;
			} else $this->l("Failed");
		} else $this->l("Failed");
		echo '</pre>';
		return false;
	}
	
	function disable ($module) {
		echo '<pre>';
		$this->l("Loading module '$module'...");
	    if($mod = core::load($module,true)) {
	       	$this->l("Ok");
	       	
	       	$this->l("Disabling module...");
			if (core::setStatus($mod->_id,'disabled')) {	       	
				$this->l("Ok");
				echo '</pre>';
				return true;
			} else $this->l("Failed");
		} else $this->l("Failed");
		echo '</pre>';
		return false;
	}

    function getModule($module) {
        global $manServer,$manPort;

        echo '<pre>';

        $data = $this->getData();
        $downloaded = array();

        $this->l("Looking up module '$module' in MANIFEST...");
        if(isset($data[$module])) 
            $this->l("Ok");
        else return $this->l('Failed');

        $this->l("Module dependencies: ");
            if (!isset($data[$module]['Build-Depends'])||trim($data[$module]['Build-Depends']) == '')
                $this->l("none\n");
            else {
                $dep = explode(' ',trim($data[$module]['Build-Depends']));
                
                foreach ($dep as $modu)
                    if (!is_dir('modules/'.$modu)) {
                        $this->l("Installing module '$modu'");
                        if(!$this->getModule($modu))
                            return $this->l("        Installation of module '$modu' failed!");
                    } else {
                        $this->l("        Skipping module '$modu'\n");
                    }
            }


        $this->l("Downloading module from $manServer:$manPort\n");
        foreach ($data[$module]['Files'] as $file) {
            $this->l("        Downloading: {$file['file']} {$file['size']}b...");
            if ($handle = fopen("http://$manServer:$manPort/arc/$module/{$file['file']}",'r')) {
                $this->l("Ok");
                $this->l("        Saving: {$file['file']}...");
                if ($handle2 = fopen($file['file'],'w')) {
                    if(fputs($handle2,stream_get_contents($handle))) {
                        $this->l("Ok");
                        fclose($handle2);

                        $this->l("        CRC check: {$file['file']} ".$this->file_crc($file['file'])."...");
                        if ($file['crc']==$this->file_crc($file['file'])) {
                            $this->l("Ok");
                            $downloaded[] = $file['file'];
                        } else return $this->l('Failed');
                    } else return $this->l('Failed');
                    fclose($handle);
                } else return $this->l('Failed');
            } else return $this->l('Failed');
        }

        if (is_dir('modules/'.$module)&&is_file('modules/'.$module.'/module.inc')) {
            $this->l("Loading old module '$module'...");
            if($mod = core::load($module,true)) 
                $this->l("Ok");
            else 
                return $this->l('Failed');

            if (!is_dir('modules/'.$module.'/archive')) {
                if (!is_writable('modules/'.$module)) {
                    $this->l("Changing 'modules/$module' to 777");
                    if(chmod("modules/$module",777))
                         $this->l("Ok");
                    else
                         return $this->l('Failed');
                }
                $this->l("Making archive folder");
                if(mkdir('modules/'.$module.'/archive')) 
                    $this->l("Ok");
                else 
                    return $this->l('Failed');
            }

            $this->l("Listing module files...");
            if($files = $this->getFiles("modules/$module/")) 
                $this->l("Ok");
            else 
                return $this->l('Failed');

           
            $file = $mod->getId()."_".$mod->getVersion(false).".tar.gz";
            $newpath = getcwd().'/modules/'.$module.'/archive/';
            $newfile = $file;
            $int = -1;
            
            while(is_file($newpath.$newfile)) {
                $int++;
                $newfile = explode('.',$file);
                
                $newfile[count($newfile)+1] = $newfile[count($newfile)-1];
                $newfile[count($newfile)-2] = $newfile[count($newfile)-3];
                $newfile[count($newfile)-3] = "$int";

                $newfile = implode($newfile,'.');
            }

            $this->l("Making package '$newfile'...");

            $tar = new tar($newpath.$newfile,true);
            
            $tar->create($files);
            if($tar->create($files))
                $this->l("Ok");
            else {
                return $this->l('Failed');
            }

            $this->l("Removing old module files...\n");
            foreach ($files as $line) {
                $this->l("        Removing: $line...");
                if(unlink($line))
                    $this->l("Ok");
                else 
                    return $this->l('Failed');
            }

            if (isset($data[$module]['Install']) && $data[$module]['Install'])
                core::setStatus($module,'not installed');
            else 
                core::setStatus($module,'active');

        }
        
        $this->l("Installing module files\n");
        foreach ($downloaded as $file) {
            $this->l("        Extracting: $file...");
            $res = system("tar -xzf $file");
            if ($res == '') 
                $this->l("Ok");
            else return $this->l('Failed');

            $this->l("        Removing: $file...");
            if (unlink($file)) 
                $this->l("Ok");
            else return $this->l('Failed');

        }

        

        $this->l("Module downloaded successfully");

        echo '</pre>';
        return true;
    }

    function getManifest($server,$port) {
            if ($handle = $this->getFile($server,$port,'/MANIFEST')) {
                $contents = array();
                $t = '_serverinfo';
                while (!feof($handle)) {
                    $data = explode( "\n", fread($handle, 8192) );
                    foreach ($data as $line) {
                        $line = explode( ':', $line,2);

                        if (isset($line[0])&&isset($line[1])) {
                            $line[0] = trim($line[0]);
                            $line[1] = trim($line[1]);
                            if($line[0] == 'Source') $t = $line[1];

                            if (!isset($contents[$t])) $contents[$t] = array();
                            $contents[$t][$line[0]] = $line[1];
                        } elseif (isset($line[0])) {
                            if ($line[0]) {
                                if (substr(trim($line[0]),0,2)=="F ") {
                                    $line = explode( ' ', substr(trim($line[0]),2) );
                                    if(isset($contents[$t]['Files'])&&isset($line[0])&&isset($line[1])&&isset($line[2])) $contents[$t]['Files'] = array();
                                        $contents[$t]['Files'][] = array('file'=>$line[0],'size'=>$line[1],'crc'=>$line[2]);
                                } else 
                                    if (trim($line[0])>'')
                                        $contents[$t][] = trim($line[0]);
                            } else $t = '_serverinfo';
                        }
                    }
                  }
                fclose($handle);
                
                if ($contents['_serverinfo'][0]!='HTTP/1.1 200 OK') {
                	send(E_WARNING,'Error downloading manifest from server! ('.$contents['_serverinfo'][0].')');
                	return false;
                }
                return $contents;
            } else {
            	send(E_WARNING,"Can not connect to server!");
            }
    }


    function getFile($server,$port,$file) {

        if($handle = fsockopen($server,$port, $errno, $errstr, 10) ) {
			fputs($handle, "GET $file HTTP/1.0\r\n");
			fputs($handle, "Host: $server\r\n");
			//fputs($handle, "Referer: http://\r\n");
			fputs($handle, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n\r\n");
			return $handle;
        }

        return false;
    }
    
    function sendModule($module) {
    	global $manServer,$manPort;
	    echo '<pre>';
        
        if ($module!='core') {
            $this->l("Loading module '$module'...");
            if(!$mod = core::load($module,true)) 
                return $this->l("Failed");
            $this->l("Ok");
        } 

        if (isset($mod->noUpload))
            return $this->l("Upload not allowed for this module!");

        $this->l("Listing package files...");
        if ($module=='core') {
            if(!$files = $this->getFiles("lib/")) 
                return $this->l("Failed");
            $files[] = 'admin.php';
        } else {
            if(!$files = $this->getFiles("modules/$module/")) 
                return $this->l("Failed");
        }

        $this->l("Ok");

        foreach ($files as $line)
            $this->l("\t$line\n");


        if ($module!='core') {
            $this->l("Loading module information:\n");
            $data = array(
                'Source'        => $mod->getId(),
                'Name'          => $mod->getName(),
                'Date'          => $this->getChanged($module),
                'Description'   => $mod->getDescription(),
                'Version'       => $mod->getVersion(false),
                'Maintainer'    => $mod->getMaintainer(false),
                'CoreApi'       => $mod->getRequiredCoreApi(false),
                'ModuleApi'     => $mod->getRequiredModuleApi(false),
                'DbApi'         => $mod->getRequiredDbApi(false),
                'Build-Depends' => $mod->getDepends(),
                'Install'       => $mod->getInstall()
            );
        } else {
            $this->l("Loading core information:\n");
            $data = array(
                'Source'        => 'core',
                'Date'          => $this->getChanged('core'),
                'Core'          => core::version,
                'Module'        => module::version,
                'Db'            => db::version
            );
        }
        foreach ($data as $key => $line)
            if (trim($line) > '')
                $this->l("\t$key: $line\n");
        
        if ($module=='core') {
            $file = 'core_'.$data['Date'].".tar.gz";
            $tar = getcwd()."/".$file;
        } else {
            $file = $mod->getId()."_".$mod->getVersion(false).".tar.gz";
            $tar = getcwd()."/".$file;
        }
        
        if (file_exists($tar)) {
            $this->l("Cleaning old packages...");
            if (unlink($tar)) 
                $this->l("Ok");
            else 
                $this->l("Failed\n");
        }
        

        $this->l("Making package '$file'...");
        
        $tarf = new tar($tar,true);
        
        if($tarf->create($files))
            $this->l("Ok");
        else {
            return $this->l('Failed');
        }
        
        
        $this->l("Uploading package to $manServer:$manPort...");
        $ret = $this->sendFile($manServer,$manPort,'/upload.php',$tar,$data);
        if (substr($ret,0,15) == "HTTP/1.1 200 OK") {
            $this->l("Ok");
            $this->l('<pre>'.$ret.'</pre>');
        } else {
            $ret = explode("\n",$ret);
            $this->l("Failed");
            $this->l("\t".$ret[0]);
        }
        
        $this->l("Cleaning...");
        if (unlink($tar)) 
            $this->l("Ok");
        else 
            return $this->l("Failed\n");

		echo '</pre>';
	}
	
	function l ($t) {
		global $charsLast;
		
		if(!isset($charsLast)) $charsLast=0;
		$tabs = 10;
		
		if ($t=='Ok') {
			$chars = intval($charsLast/8);
			echo str_repeat("\t",($tabs-$chars))."[ <font color=\"00bb00\">Ok</font> ]\n";
			$charsLast=0;
		} elseif ($t=='Failed') {
			$chars = intval($charsLast/8);
			echo str_repeat("\t",($tabs-$chars))."[ <font color=\"CC0000\">Failed</font> ]\n</pre>";
			$charsLast=0;
		} else {
	    	echo $t;
	    	$charsLast += strlen($t);
	    	flush();
	    }
	    
	    $temp = explode("\n",$t);
	    if (count($temp)>1)
	    	$charsLast=0;
        return false;
	}
	
	
	function file_crc($file)
	{
	   $file_string = file_get_contents($file);
	
	   $crc = crc32($file_string);
	  
	   return sprintf("%u", $crc);
	}
	
	function getFiles($path) {
	    
	    $files = array();
	    $path = rtrim($path,'/').'/';
	        
	    if (!is_dir($path))
	        return false;
	
	    $data = scandir($path);
	
	    foreach ($data as $file) {
	        if (substr($file,0,1)!='.')
	            if (filetype($path.$file)=='file') {
	                $files = array_merge($files,array($path.$file));
	            } else {
	                if ($file!='locales'&&$file!='archive')
	                    $files = array_merge($files,$this->getFiles($path.$file));
	            }
	    }
	
	    return $files;
	}
	
	   function mime_content_type ( $f )
	   {
	       return system ( trim( 'file -bi ' . escapeshellarg ( $f ) ) ) ;
	   }
	
	
	function sendfile($server,$port,$target,$file,$data = array()) {
	
	    $filename = explode('/',$file);
	    
	    $body = "--AaB03x\r\n";
	    $body .= "Content-Disposition: form-data; name=\"crc\"\r\n\r\n";
	    $body .= $this->file_crc($file)."\r\n";
	    foreach ($data as $key => $line) {
            if ($line>'') {
                $body .= "--AaB03x\r\n";
                $body .= "Content-Disposition: form-data; name=\"$key\"\r\n\r\n";
                $body .= $line."\r\n";
            }
	    }
	    $body .= "--AaB03x\r\n";
	    $body .= "Content-Disposition: form-data; name=\"files\"; filename=\"".$filename[count($filename)-1]."\"\r\n";
	    $body .= "Content-Type: application/octet-stream\r\n\r\n";
	    $body .= file_get_contents($file);
	    $body .= "\r\n--AaB03x--";
	
	    $data = "POST $target HTTP/1.0\r\n";
	    $data .= "Host: $server\r\n";
	    $data .= "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n";
	    $data .= "Content-Type: multipart/form-data; boundary=AaB03x\r\n";
	    $data .= "Content-Length: ".strlen($body)."\r\n\r\n";
	    $data .= $body;
	    
	    $ret = false;
	
	    if($handle = &fsockopen($server,$port, $errno, $errstr, 3) ) {
	        fputs($handle, $data);
	        $ret = stream_get_contents($handle);
	        fclose($handle);
	    } else {
	    	return $errstr;
		}
	    
	    return $ret;
	}
    function getChanged($module) {
        global $manifest;
        $changed = 0;
            
        if ($module=='core')
            $module = 'lib';
        else 
            $module = 'modules/'.$module;

        if($files = $manifest->getFiles($module))
            foreach ($files as $file) {
                $mod = filemtime($file);
                if ($mod > $changed)  $changed = $mod;
            }

        return $changed;
    }
}

?>
