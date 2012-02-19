<?php

class mailfile {
    
    public $headers = array();

    function __construct($file) {
        if(!is_file($file)) {
            send(E_ERROR,"Error opening file '$file'");
            return false;
        }

        if (!$data = file_get_contents($file)) {
            send(E_ERROR,'Error reading file "'.$file.'"');
            return false;
        }

        $lines = explode("\n",$data);
        
        $this->file = explode('/',$file);
        if ($this->file[count($this->file)-2]=='new') {   
            $this->new = true;
            
        }

        $this->dir = implode( array_slice($this->file,0,count($this->file)-1) ,'/');
        $this->file = $this->file[count($this->file)-1];

        $header = true;
        foreach ($lines as $line) {
            $line = stripslashes($line);

            if ($header && trim($line) == '') {
                $header = false;
                continue;
            }

            if ($header) {
                $line = explode(':',$line,2);
                if (isset($line[1]))
                    $this->headers[trim($line[0])] = trim($line[1]);
            } else {
                $this->content .= $line . "\n";
            }
        }


        if (isset($this->headers['Importance'])) {
            if ($this->headers['Importance'] == 'high') 
                $this->prio = 'High';
            elseif ($this->headers['Importance'] == 'low')
                $this->prio = 'Low';
        } elseif (isset($this->headers['Priority'])) {
            if ($this->headers['Priority'] == 'Urgent')
                $this->prio = 'High';
            elseif ($this->headers['Priority'] == 'Non-Urgent')
                $this->prio = 'Low';
        } elseif (isset($this->headers['X-Priority'])) {
            if ($this->headers['X-Priority'] < 3)
                $this->prio = 'High';
            elseif ($this->headers['X-Priority'] > 3)
                $this->prio = 'Low';
        } 
        if (!isset($this->prio))
            $this->prio = 'Normal';
    }

    function mark() {
        $file = explode('/',ltrim($this->dir,'/').'/'.$this->file);

        if ($file[count($file)-2]!='new')    
            return false;
        
        $from = $this->dir.'/'.$this->file;
        $to = '/'.implode(array_slice($file, 0, count($file)-2) , '/').'/cur/'.$file[count($file)-1];

        rename( $from , $to );
    }

	function isSpam() {
		if ( !isset($this->headers['X-Spam-Status']) )
			return false;
		
		if ( substr($this->headers['X-Spam-Status'],0,3) == 'Yes' )
			return true;

		return false;
	}
}

class externalmail {
    
    public $mails = array();

    function __construct($dir,$create_dir = true) {
        $this->dir['main'] = rtrim($dir,'/');
        
        if(!is_dir($this->dir['main'])) {
            if($create_dir) {
                mkdir($this->dir['main']);
            } else {
                send(E_ERROR,"Directory dont exist '{$this->dir['main']}'");
                return false;
            }
        }

        $this->dir['cur']  = rtrim($dir,'/') . "/cur";

        if(!is_dir($this->dir['cur'])) {
            if($create_dir) {
                mkdir($this->dir['cur']);
            } else {
                send(E_ERROR,"Directory dont exist '{$this->dir['cur']}'");
                return false;
            }
        }

        $this->dir['new']  = rtrim($dir,'/') . "/new";

        if(!is_dir($this->dir['new'])) {
            if($create_dir) {
                mkdir($this->dir['new']);
            } else {
                send(E_ERROR,"Directory dont exist '{$this->dir['new']}'");
                return false;
            }
        }

        $this->dir['spam']  = rtrim($dir,'/') . "/spam";

        if(!is_dir($this->dir['spam'])) {
            if($create_dir) {
                mkdir($this->dir['spam']);
            } else {
                send(E_ERROR,"Directory dont exist '{$this->dir['spam']}'");
                return false;
            }
        }

        $this->ok = true;
    }

    function clean($file) {
        $file = preg_replace("/[^a-z\.0-9]/i",'',$file);
        return $file;
    }

    function remove($file) {
        $file = $this->clean($file);

        if (!is_dir($this->dir['cur'].'/../rem'))
            mkdir($this->dir['cur'].'/../rem');

        if( !is_file($this->dir['cur'].'/'.$file) )	{
			if ( !is_file($this->dir['new'].'/'.$file) )
				return send(E_ERROR,"The file '".$this->dir['cur'].'/'.$file."' dont exist!");
			else
				$file1 = $this->dir['new'].'/'.$file;
		} else 
			$file1 = $this->dir['cur'].'/'.$file;

        if ( !rename($file1,$this->dir['cur'].'/../rem/'.$file) );
			return send(E_ERROR,"Failed to move '".$this->dir['cur'].'/'.$file."'!");

    }

    function load($dir = '') {
        if (isset($this->ok)) {
            if ($dir == '') {
                $this->load('new');
                $this->load('cur');
            } elseif ($dir=='new'||$dir=='cur') {
                if (!$content = scandir($this->dir[$dir] ,1 ) ) {
                    send(E_ERROR,"Error loading dir '{$this->dir[$dir]}'");
                }
                
                foreach ($content as $file) {
                    if (!is_dir($file)) {
                        $mail = new mailfile($this->dir[$dir].'/'.$file);
                        if (isset($mail->file))
                            $this->mails[] = $mail;
                    }
                }   
            } else {
                $dir = $this->clean($dir);

                if (is_file($this->dir['cur'].'/'.$dir)) 
                    $this->mails[] = new mailfile($this->dir['cur'].'/'.$dir);
                elseif (is_file($this->dir['new'].'/'.$dir)) 
                    $this->mails[] = new mailfile($this->dir['new'].'/'.$dir);

                if (isset($this->mails[0]->new))
                    $this->mails[0]->mark();

                return true;
            }
        }
    }
    
    function check() {
        $dir = 'new';

        if (!$content = scandir($this->dir[$dir]) ) {
            send(E_ERROR,"Error loading dir '{$this->dir[$dir]}'");
        }
        
        $count == 0;

        foreach ($content as $file) {
            if (is_dir($file))
				continue;
			
			if ( 635 == $_SESSION['id'] ) {
				$mail = new mailfile($this->dir[$dir].'/'.$file);
				if ( $mail->isSpam() ) {
					if ( !is_dir($this->dir['spam']) )
						mkdir( $this->dir['spam'] );

					rename( $this->dir[$dir].'/'.$file,$this->dir['spam'].'/'.$file );
					
					continue;
				}
			}
            $count++;
        }

        return $count;
    }
}

class mail extends module {
    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Mail class');
        $this->setDescription('manage mailing');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');
        $this->setMaintainer('Jonas Falck <jonaz@jonaz.net>');


        parent::module();
    }

    function install () 
    {
        $tables = array(
            'structure' => array(
                array(
                    'Field' => 'mail',
                    'Type'  => 'varchar(255)'
                )
            )
        );

        $this->installTables($tables);

        parent::install();
        return true;
    }

    function uninstall () 
    {
        parent::uninstall();
    }
    

    function generate($gid = 0,$list='') {
        if ($gid == 0) {
            $lists = $this->fetchAll("SELECT * FROM structure WHERE mail>''");
            $this->list = array();
            if (isset($lists)&&is_array($lists))
                foreach ($lists as $line) 
                    $this->generate($line['gid'],$line['mail']);

            if($lists = $this->fetchAll("SELECT * FROM maillists"))
                foreach($lists as $list) {
                    $sources = explode(',',$list['Groups']);
                    $members = array();
                    foreach ($sources as $source) {
                        switch(substr($source,0,1)) {
                            case 'E':
                                $groups = $this->fetchAllOne("SELECT gid FROM structure WHERE event=".substr($source,1));
                                foreach ($groups as $gid) {
                                    if($members = $this->fetchAllOne("SELECT email FROM membership,user_profile WHERE gid=$gid AND user_profile.uid=membership.uid")) {
                                        if (isset($this->list[$list['name']])&&is_array($this->list[$list['name']]))
                                            $this->list[$list['name']] = array_merge($this->list[$list['name']],$members);
                                        else
                                            $this->list[$list['name']] = $members;
                                    }
                                }
                                break;
                            case 'G':
                                    $this->generate(substr($source,1),$list['name']);
                                break;
                            case 'U':
                                if($members = $this->fetchAllOne("SELECT email FROM user_profile WHERE user_profile.uid=".substr($source,1))) {
                                    if (isset($this->list[$list])&&is_array($this->list[$list]))
                                        $this->list[$list] = array_merge($this->list[$list],$members);
                                    else
                                        $this->list[$list] = $members;
                                }

                                break;
                        }

                    }
                }
            if($handle = fopen('mailaliases','w')) {
                foreach ($this->list as $listname => $members) {
                    fwrite($handle,"$listname: ".implode(array_unique($members),',')."\n");
                }

                fclose($handle);
            } else send(E_ERROR,'Error saving mail aliases to file!');
        } else {
            if ($teams = $this->fetchAll("SELECT * FROM structure WHERE parent=$gid"))
                foreach ($teams as $line)
                    $this->generate($line['gid'],$list);

            if($members = $this->fetchAllOne("SELECT email FROM membership,user_profile WHERE gid=$gid AND user_profile.uid=membership.uid")) {
                if (isset($this->list[$list])&&is_array($this->list[$list]))
                    $this->list[$list] = array_merge($this->list[$list],$members);
                else
                    $this->list[$list] = $members;
            }
        }
    }

    function _inbox() {
        
        global $path,$tpl;
        
        $user = $_SESSION['id'];
        if (isset($path->vars['uid']) && $path->write)
            $user = $path->vars['uid'];

        $user = $this->fetchSingle("SELECT uid,username FROM users WHERE uid=$user");

        if (!$ext = new externalmail('/safespace/mail/crew.dreamhack.se/'.strToLower(path::encode($user['username']))))
            return false;
        
        if (isset($_GET['remove'])) {
            $ext->remove($_GET['remove']);
        }

        if (isset($_GET['show'])&&$ext->load($_GET['show'])) {
            

            $mail = $ext->mails[0];
            $_POST['content'] = $mail->content;

            $mail = array(
                'Date' => timestamp($mail->headers['Date']),
                'From' => htmlspecialchars($mail->headers['From']),
                'To'   => htmlspecialchars($mail->headers['To']),
                'Subject' => htmlspecialchars($mail->headers['Subject']),
                'Spam' => isset($mail->headers['X-Spam-Status']) ? htmlspecialchars($mail->headers['X-Spam-Status']) : 'Not scanned',
                'Prio' => $mail->prio,
                'Content' => $_POST->clean('content',false),
                'Id' => $mail->file
            );
            $tpl->assign('mail',$mail);
            $this->display('mail.tpl');
        } else {
            $ext->load();
        
            $mails = array();
            foreach ($ext->mails as $key => $mail) {    
                $lnk = (isset($mail->new) ? '<b>' : '')."<a href=\"?show={$mail->file}\">";
                $b = isset($mail->new) ? '</a></b>' : '</a>';
                

                if(preg_match("/^(([^\<]*)\<([a-z_\-\.]+@[a-z_\-\.]+)\>)$/i",trim($mail->headers['From']),$resp) ) {
                    $from = $resp[2];
                } else $from = htmlspecialchars($mail->headers['From']);

                $mails[$key]['Datum']    = '<span id="'.date('Y-m-d H:i:s',strtotime($mail->headers['Date'])).'">' . (isset($mail->new) ? '<b>' : '') . date('d M',strtotime($mail->headers['Date'])) . (isset($mail->new) ? '</b>' : '').'</span>';
                $mails[$key]['Från']    = $lnk . $from . $b;
                $mails[$key]['Ämne'] = $lnk .'<div style="overflow:hidden;">' . htmlspecialchars($mail->headers['Subject']) . '</div>' . $b;
                //$mails[$key]['']        = '<input type="checkbox" id="remove_'.$mail->file.'" style="width:auto;height:auto;" class="remove">';
                $mails[$key][''] = ' <input type="button" value="Ta bort" style="display:inline;" onClick="location.href=\'?remove='.$mail->file.'\';">';
            }
            
            $this->display('inbox.tpl');

            $table = core::load('tables');
            $table->initarray['Datum'] = 'DESC';
            $table->create($mails,array('width'=>'100%'),'on');
            if (count($mails)==0)
                echo 'Här var de tomt :P';
        }
    }
    

    function _compose() {
        global $path,$tpl;
        
        if (isset($_GET['reply']))
            $mail = $_GET['reply'];
        elseif (isset($_GET['replyall']))
            $mail = $_GET['replyall'];
        elseif (isset($_GET['forward']))
            $mail = $_GET['forward'];

        if (isset($mail)) {
            $user = $_SESSION['id'];
            if (isset($path->vars['uid']) && $path->write)
                $user = $path->vars['uid'];

            $user = $this->fetchSingle("SELECT uid,username FROM users WHERE uid=$user");

            if (!$ext = new externalmail('/safespace/mail/crew.dreamhack.se/'.strToLower(path::encode($user['username']))))
                return false;

            $ext->load($mail);

            if (isset($ext->mails[0])) {

                if (isset($_GET['reply']) ) {
                    if (!isset($_GET['to']) )
                        $_POST['to'] = $ext->mails[0]->headers['From'];

                    if (!isset($_POST['subject']) )
                        $_POST['subject'] = 'RE: '.$ext->mails[0]->headers['Subject'];

                    if (!isset($_POST['text']) )
                        $_POST['text'] = $this->tab($ext->mails[0]->content);
                }

                if (isset($_GET['replyall']) ) {
                    if (!isset($_GET['to']) ) {
                        $to = array($ext->mails[0]->headers['From']);
                        if($f = explode(',',$ext->mails[0]->headers['To']))
                            $to = array_merge($to,$f);
                        $_POST['to'] = implode($to,',');
                    }

                    if (!isset($_POST['subject']) && substr($_POST['subject'],0,3) != 'RE:')
                        $_POST['subject'] = 'RE: '.$ext->mails[0]->headers['Subject'];

                    if (!isset($_POST['text']) )
                        $_POST['text'] = $this->tab($ext->mails[0]->content);
                }
                if (isset($_GET['forward']) ) {
                    if (!isset($_POST['subject']) && substr($_POST['subject'],0,3) != 'FW:')
                        $_POST['subject'] = 'FW: '.$ext->mails[0]->headers['Subject'];

                    if (!isset($_POST['text']) )
                        $_POST['text'] = $this->tab($ext->mails[0]->content);
                }

            }
        }

        // Autocomplete
        if (isset($_POST['search'])) {
            global $tpl;
            $s = $this->escapeStr(utf8_decode($_POST['search']));
            $data = $this->fetchAll("SELECT username, email, concat(firstname,' ',lastname) as name FROM users,user_profile WHERE (concat(firstname,' ',lastname) LIKE '%$s%' OR username LIKE '%$s%') AND users.uid=user_profile.uid");
            
            foreach ($data as $key => $line)
                foreach ($line as $key2 => $line2){
                    $data[$key][$key2] = preg_replace("/$s/i",'<span class="hit">'.$s.'</span>',$line2);
                }
            
            $tpl->assign('search',$data);
            $this->display('autocomplete.tpl');
            return true;
        }
        send(E_USER_WARNING,'OBS! Kom ihåg att det inte går att maila utifrån och in ännu.. men det går fint att maila ut. //stamp');
        get();

        $tpl->assign('from',$this->getFrom());

        if (isset($_POST['text'])&&isset($_POST['to'])&&isset($_POST['from'])&&isset($_POST['subject'])) {
            if (!in_array($_POST['from'],$tpl->from)) {
                send(E_ERROR,'OGILTIG Källa!');
                return false;
            }


            if ($this->send($_POST['from'],$_POST['to'],$_POST['subject'],$_POST['text'])) {
                $this->display('compose.success.tpl');
                return true;
            } 
            
            send(E_ERROR,'Misslyckades med att skicka mailet! Kontrollera adresser!');
            get();
        }

        if (!isset($_POST['to'])&&isset($path->vars['uid'])&&$_SESSION['id']!=$path->vars['uid'])
            $_POST['to'] = $this->fetchOne("SELECT username FROM users WHERE uid=".$path->vars['uid']);


        $this->display('compose.tpl'); 
    }

    function getFrom() {
        $data = array();
        
        if($self = $this->fetchSingle("SELECT username,concat(firstname,' ',lastname) as name FROM users JOIN user_profile USING (uid) WHERE users.uid={$_SESSION['id']}") )
            $data[] = $self['name'] . ' <' . path::encode($self['username']) . '@crew.dreamhack.se>';

        return $data;
    }
    
    function send($from,$to,$subject,$message) {
        
        // From
        if (!$this->check($from)) 
            return false;

        // TO
        $to = explode(',',$to);
        $to_valid = array();
        
        if (isset($to)&&is_array($to))
            foreach($to as $key => $u) {
                if (!$this->check($u,false)) {
                    if(!$user = $this->fetchSingle("SELECT username,firstname,lastname FROM users JOIN user_profile USING(uid) WHERE username LIKE '".$this->escapeStr(trim($u))."'")) {
                        send(E_USER_WARNING,"Kan inte hitta användare '".htmlspecialchars($u)."' i registret");
                        return false;
                    } else 
                        $to_valid[] = "{$user['firstname']} {$user['lastname']} <".path::encode($user['username'])."@crew.dreamhack.se>";
                } else 
                    $to_valid[] = $u;
            }
    
        if (count($to_valid)==0)
            return false;

        $to      = implode($to_valid,', ');

        // From
        $headers = 'From: '.$from;

        //ini_set('sendmail_path','/usr/sbin/exim4 -t');

        $this->generate();
        return mail($to, $subject, $message, $headers,'');
    }

    function check($email,$larm = true) {
        $pattern = 
            "/^([a-z_0-9!]+@[a-z0-9_\-\.]+\.[a-z]{2,4})$|".
            "^(([^\<]*)\<([a-z_\-\.0-9!]+@[a-z0-9_\-\.]+)\>)$/i";
        $res = preg_match($pattern,trim($email));
        if (!$res && $larm)
            send(E_USER_WARNING,"Felaktig adress '".htmlspecialchars(trim($email))."'");
        return $res;
    }

    function tab ($data) {
        $data = explode("\n",$data);

        foreach ($data as $key => $line)
            $data[$key] = "> $line";

        return implode($data,"\n");
    }
    // {{{old
    /*
    function _testmail(){
        //$to = array('Jonas Falck'=>'jonaz@jonaz.net',"jonas igen"=>"jonas.falck@gmail.com",'Jonas wge'=>'wge609n@tninet.se');
        $to = array();
        for($i=0;$i<=10;$i++){
            $to = array_merge(array('Jonas hotmail'.$i.''=>'jonaz_86@hotmail.com','Jonas Falck'.$i.''=>'jonaz@jonaz.net',"jonas igen$i"=>"jonas.falck@gmail.com",'Jonas wge'.$i.''=>'wge609n@tninet.se'),$to);
        }
        $from = array('Crew corner'=>'crewcorner@dreamhack.se');
        //$to= $this->mkarray($this->fetchAll("SELECT CONCAT(firstname,' ',lastname) AS name,email FROM users JOIN user_profile USING (uid) WHERE uid=766 LIMIT 10") );
        $body = "testbody";
        $subject = "testsubject";
        //$this->mail($subject,$body,$to,$from);

    }
    function mkarray($sqldata){
        foreach($sqldata AS $val){
            $to[$val['name']]= $val['email'];
        }
        if(count($to)> 0)
            return $to;
        else
            return false;
    }

    function send($subject,$body,$to,$from){
        //Ex:
        //maila array("Namn efternamn"=>"balba@blabal.net")
        //from: array("Jonas Falck"=>"jonaz@jonaz.net") 
        if(count($from)>1){
            send(E_USER_ERROR,"Du kan bara skicka från en person");
            return false;
        }
        require_once("phpmailer/class.phpmailer.php");
        $mail = new PHPMailer();
        $mail->From     = $from[key($from)];
        $mail->FromName = key($from);
        //$mail->Host     = "mail.adamo.se";
        //$mail->Mailer   = "smtp";
        

        foreach($to AS $name=>$email){
            $mail->Subject    = $subject;
            $mail->Body    = $body;
            //$mail->AltBody = $text_body;
            $mail->AddAddress($email, $name);
            if(!$mail->Send())
                echo "Det uppstod ett fel när mail skickades till " . $email . "<br />";
            else
                echo 'Skickade till: '.$name.' <'.$email.'><br>';
            $mail->ClearAddresses();
            $mail->ClearAttachments();
        }
        
        
   }
   */
   // }}}
}
?>
