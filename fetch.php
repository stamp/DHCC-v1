<?php
require_once('config.inc');

$tpl = new template('templates');
if ($path = core::load('path',false)) {

    if($cleanpath = $path->process($_GET['path'])) 
    if (isset($path->module))
        core::run($path->module,$path->method,array('nodiv'=>1));

}

?>
