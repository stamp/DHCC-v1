<?php
$start = microtime(true);
require_once('config.inc');

if (!$db->tableExists('modules')) core::install();

$tpl = new template('templates');

if ($path = core::load('path',false)) {

    if (!isset($_GET['path'])) $_GET['path'] = '/start';

    if(isset($_GET['path']) && $_GET['path']=='/exit')
        core::run('user','_signout');

    $tpl->assign('fullpath',$_GET['path']);

    if($cleanpath = $path->process()) {
        $tpl->assign('sitehead','DHCC - '.$path->head);
    }
    $tpl->assign('menu',$path->getLevelMenu());
    $tpl->assign('cleanpath',$path->clean);
}

$ev = core::load('evaluate',false);

$crew = in_array('|G-3',explode(",",$path->access));

if(isset($_SESSION['validprofile']) && $_SESSION['validprofile'] !='Y' ){
    send(E_USER_WARNING,"Din profil måste uppdateras för att du ska kunna använda Crew Corner!");
    $tpl->sitehead='DHCC - Uppdatera din Profil';
    $tpl->display('header.tpl');
    core::run('extendeduser','_editProfile');
} elseif($crew && isset($_SESSION['valideventinfo']) && $_SESSION['valideventinfo'] !='Y'){
    send(E_USER_WARNING,"Dina Evenemangsuppgifter måste uppdateras för att du ska kunna använda Crew Corner!");
    $tpl->sitehead='DHCC - Uppdatera din Evenemangsinformation';
    $tpl->display('header.tpl');
    core::run('extendeduser','_editEventinfo');
} elseif($crew && isset($ev) && $ev->checkEvaluation()) {
    $tpl->sitehead='DHCC - Utvärdering';
    $tpl->display('header.tpl');
    $ev->doEvaluation();
} else {
    unset($ev);

    if (isset($_SESSION['id'])) { 
        core::run('events','getEventSelect',array('nodiv'=>1));

        $teams = core::load('teams');
        $data = $teams->_listTeams();
        $tpl->assign('teamlist',$data);
    } else {
        $tpl->assign('news',$db->fetchAll("SELECT timestamp as head, head as text FROM news WHERE list=1 ORDER BY id DESC LIMIT 5"));
    }

    $tpl->display('header.tpl');
    //developers
    if(in_array('|G73',explode(",",$path->access))){

    echo '<div style="border:1px dashed #c00;padding:5px;margin:5px;font-size:10px;';
    //if ($path->status=='hidden') echo 'background:#900;';
    echo '">';
            echo '<b>Module:</b> '.$path->module.' | ';
            echo '<b>Method:</b> '.$path->method.' | <b>Path:</b> '.$path->clean;
            if (isset($path->write))        echo ' | <b>Write:</b>&nbsp;'.$path->write ;
            if(isset($path->vars['team']))  echo ' | <b>Team:</b>&nbsp;'.$path->vars['team'] ;
            if(isset($path->vars['uid']))   echo ' | <b>User:</b>&nbsp;'.$path->vars['uid'] ;
            if(isset($path->accessline)) echo ' | <b>Access:</b> '.$path->accessline ;
            echo '</div>';
    }

    get();
    if (isset($path->module))
        if (is_file(ROOT.$path->module))
                include(ROOT.$path->module);
            else
                core::run($path->module,$path->method);
}

get();

$tpl->display('footer.tpl');
if($_SESSION['id'] == '766')
    echo $db->queryCount;


if($_SESSION['id'] == '635')
    echo '<div style="position:absolute;color:#000;left:0;top:0;background:#fff;">Tid: '.( microtime(true)-$start).'</div>';
?>
