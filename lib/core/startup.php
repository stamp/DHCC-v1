<?php
// activate the autoload function for classes
function __autoload($class_name) {
    if (is_file(ROOT.'lib/core/'.$class_name.'.php')) 
        require_once ROOT.'lib/core/'.$class_name.'.php';

    if (is_file(ROOT.'lib/auto/'.$class_name.'.php'))
        require_once ROOT.'lib/auto/'.$class_name.'.php';
    
    if (is_file(ROOT.'modules/'.$class_name.'/module.inc'))
        require_once ROOT.'modules/'.$class_name.'/module.inc';
}

// include our
include('functions.inc');

define('START_TIME', microtime(true) );
define('THIS_FILE', basename($_SERVER['SCRIPT_FILENAME']) );
define('QUERY_STRING', str_replace('&','&amp;', $_SERVER['QUERY_STRING']) );

$logg = array();

ini_set ('display_errors',$displayErrors);

if (! defined('NO_DATABASE') ) {
   $db = new db($dbServer, $dbUser, $dbPasswd, $dbDatabase);
   define('DB_CON',$db->getLinkId());
   
   // If not the NO_SESSIONS contant were defined, use sessions
   if (! defined('NO_SESSIONS') ) require_once 'sessions.inc';
}

if(class_exists('safety')&&$safety = core::load('safety')) {
    $safety->init();
}

if (is_dir('modules/error')&&$error = core::load('error')) {
    $error->startup();
}


?>
