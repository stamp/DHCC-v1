<?php

$dbServer     = 'localhost';
$dbUser       = 'dreamhack';
$dbPasswd     = '';
$dbDatabase   = 'dreamhack';

$manServer	  = 'modules.stamp.se';
$manPort	  = 80;

// Set errorreporting to all
$displayErrors  = 1;

// enable our errorHandler
define('DEBUG',0);

define('ROOT',substr($_SERVER['SCRIPT_FILENAME'],0,-strlen($_SERVER["SCRIPT_NAME"])).'/');
include(ROOT.'lib/core/startup.php');

?>
