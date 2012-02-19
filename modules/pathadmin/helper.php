<?php
chdir('../../');
require('config.inc');


$tpl = new template('templates-new');

if ($path = new path('path')) {
    
    if (!isset($_GET['path'])) die();

    if($cleanpath = $path->process($_GET['path'])) {
        $tpl->assign('sitehead',$path->head);
    }

    $tpl->assign('cleanpath',$path->clean);
    
    core::run('pathadmin','_helper');
}
    

?>

