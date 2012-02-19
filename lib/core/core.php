<?php

class core {
    
    const version = '1.4.0';

    function install() {
        global $db;
        
      // install a new table
      $db->installTables(
         array(
            'modules' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'varchar(100)',
                    'Key'     => 'PRI'
                ),
                array(
                    'Field'   => 'status',
                    'Type'    => 'varchar(255)',
                ),
                array(
                    'Field'   => 'version',
                    'Type'    => 'varchar(12)'
                )
            )
         )
      );

    }

    function load($module,$override = false) {
        global $db;
        
        if ($module == '') {
            return false;    
        }

        if (!is_dir(ROOT."modules/$module/")) {
            if(!$override)
                send(E_ERROR,"Module '$module' is missing");
            return false;    
        }

        if (!is_file(ROOT."modules/$module/module.inc")) {
            if(!$override)
                send(E_ERROR,"Module '$module' is broken");
            return false;    
        }
        
        $version = $db->fetchOne("SELECT version FROM modules WHERE id='$module'");
        
        if ($args = func_get_args()) {
            $args = array_slice($args,2);
        }
        
        $arg = '';
        foreach ($args as $key => $line) {
            if($arg == '')
                $arg = '$args['.$key.']';
            else
                $arg .= ',$args['.$key.']';
        }

        if ($override) {
            
            require_once(ROOT."modules/$module/module.inc");
            
            eval("\$mod = new $module($arg);");
            //$mod = new $module;
            return $mod;

        } else {
            if ($status = core::status($module)) {
                if ($status == 'active') {
                    require_once(ROOT."modules/$module/module.inc");
                    
                    eval("\$mod = new $module($arg);");
                    //$mod = new $module;
                    if ($version == $mod->_version) {
                        if (self::checkApi($mod))
                            return $mod;
                    } else send(E_ERROR,"Module '$module' (version $version) is not activated to right the version (version {$mod->_version})");
                } else send(E_WARNING,"Module '$module' is $status");
            } else send(E_ERROR,"Module '$module' is not installed");
        }

        return false;
    }

    function status ($module,$ver = null) {
        global $db;
        
        if (is_object($module))
            $obj = $module->_id;
        else
            $obj = $module;

        if ($status = $db->fetchSingle("SELECT status,version FROM modules WHERE id='$obj'")) {
                if ($status['version']==$ver || $ver == null)
                    $status = $status['status'];
                else 
                    $status = 'awating upgrade';

                return $status;
        } else {
            return 'not installed';
        }
    }

    function setStatus ($module,$status,$version = null) {
        global $db;
        
        $v = ($version != null) ? ",version='$version'" : '';
        
        if ($db->fetchOne("SELECT status FROM modules WHERE id='$module'")) {
            return $db->query("UPDATE modules SET status='$status'$v WHERE id='$module'");
        } else {
            return $db->query("INSERT INTO modules SET status='$status',id='$module'$v");
        }
    }

    function run($name1,$method = '_run',$extra = NULL) {
        global $tpl;
        
        if(!is_object($name1)&&$name1 == 'template') {
            return $tpl->display($method);
        } elseif(!is_object($name1)&&$name1 == 'include') {
            if(!is_file($method)) {
                send(E_WARNING,"Failed to include file '$method'");
                return false;
            }
            include($method);
        }

        if (is_object($name1)) {
            $module = $name1;
            $name = get_class($name1);
        } else {
            $name = $name1;
            if (!$module = core::load($name)) 
                return false;
        }
        

        if (!in_array($method,get_class_methods($module))) {
            send(E_ERROR,"Method '$method' wasnt found in module '".get_class($module)."'");
            return false;
        }

        $arg = '';

        /*
        if (func_num_args()>2) {
            $args = array_slice($args,2);

            $args = func_get_args();
        
            foreach ($args as $key => $line) {
                if($arg == '')
                    $arg = '$args['.$key.']';
                else
                    $arg .= ',$args['.$key.']';
            }
        }
        */
        if(isset($extra['nodiv']) && $extra['nodiv'] == 1){
            eval("\$module->$method($arg);");
            if (!is_object($name)) unset($module);
        }
        else{
            echo "\t\t".'<div class="module" id="'.$name.'">';
            eval("\$module->$method($arg);");
            if (!is_object($name)) unset($module);
            echo "\n\t\t</div>";
        }
        return true;
    }

    function getMethods($cls) {
        if (!is_object($cls))
            $cls = core::load($cls,true);

        $methods = get_class_methods($cls);
        $ret = array();

        if (isset($methods)&&is_array($methods))
        foreach ($methods as $line) {
            if (substr($line,0,1)=='_'&&substr($line,1,1)!='_')
                $ret[] = $line;
        }

        return $ret;
    }

    function getModules() {
        $folders = scandir('modules/');
        $modules = array();

        foreach ($folders as $key => $line) {
            if (filetype('modules/'.$line)=='dir'&&$line!='.'&&$line!='..') {
                $modules[$key] = $line;
            }
        } 
        return $modules;
    }

    function checkApi($src) {
        if ($r = $src->getRequiredModuleApi()) {
            if (!core::isCompatibleWithApi($r,module::version())) {
                send(E_ERROR,'Version conflict: Wrong ModuleApiVersion, Required: '.implode($r,'.').', Provided: '.implode(module::version(),'.'));
                return false;
            }
        }
        if ($r = $src->getRequiredCoreApi()) {
            if (!core::isCompatibleWithApi($r,core::version())) {
                send(E_ERROR,'<b>'.$src->getName().' module:</b> Version conflict: Wrong CoreApiVersion, Required: '.implode($r,'.').', Provided: '.implode(core::version(),'.'));
                return false;
            }
        }

        if ($r = $src->getRequiredDbApi()) {
            if (!core::isCompatibleWithApi($r,db::version())) {
                send(E_ERROR,'<b>'.$src->getName().' module:</b> Version conflict: Wrong DbApiVersion, Required: '.implode($r,'.').', Provided: '.implode(db::version(),'.'));
                return false;
            }
        }

        return true;
    }

    function version() {
        // return as an array
        return explode('.',self::version);
    }

    function isCompatibleWithApi($required, $provided) 
    {

        //Check inputs that they are correct arrays
            if (!is_array($required) || !is_array($provided)) {
                return false;
            }

            if (count($provided) < 2 || count($required) < 2) {
                return false;
            }
            
            for ($i = 0; $i < 1; $i++) {
                if (!is_numeric($required[$i]) || !is_numeric($provided[$i])) {
                    return false;
                }
            }
        //Major version should be the same
            if ($required[0] != $provided[0]) {
                return false;
            }

        //Minor version should be same or greater
            if ($required[1] > $provided[1]) {
                return false;
            }

        return true;
    }
// printArr {{{
/**
 * Print_r with <pre> tags
 * used to increase readabillity of debuging
 *
 */
function printArr(&$arr)
{
   if (is_array($arr) || is_object($arr)){
   echo "<pre>\n";
   print_r($arr);
   echo "\n</pre>\n";

   }
   else
       return false;

}

// }}}
}

?>
