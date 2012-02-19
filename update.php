<?php
/*
 * Automatisk kontroll av olästa meddelanden
 *
 * 2006-04-26 Skapad (stamp)
 */

$tS = microtime(true);

require_once 'config.inc';


$tpl = new template('templates');

// If signd in
if (isset($_SESSION['id'])) {
    $tpl->assign('gb',$db->fetchOne("SELECT count(*) as cnt FROM guestbook WHERE new='new' AND gbid=".$_SESSION['id']));
    $tpl->assign('user',$db->fetchOne("SELECT username FROM users WHERE uid=".$_SESSION['id']));
    $tpl->assign('mail',cmail());
    $tpl->display('update.tpl.php');
} else {
    echo '1';
}

function cmail() {
    global $tpl;
    if (!is_dir('/safespace/mail/crew.dreamhack.se/'.strToLower(path::encode($tpl->user) )))
        return false;
    
    $mail = core::load('mail');

    if (!$ext = new externalmail('/safespace/mail/crew.dreamhack.se/'.strToLower(path::encode($tpl->user)) ))
        return false;

    return $ext->check();
}
?>
