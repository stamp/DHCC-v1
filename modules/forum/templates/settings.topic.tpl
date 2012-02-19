
<div style="padding:10px;">
<?php

$f = new form('?action=settings',$this->vals,$this->errors);

echo $f->start();

echo $f->text('head','Rubrik');
echo $f->text('tags','Etiketter');
    echo $f->select('sticky','Klistrad',array(
        array('text'=>'2 - Superklistrad','val'=>'2'),
        array('text'=>'1 - Klistrad','val'=>'1'),
        array('text'=>'0 - Normal','val'=>'0')
    ));
    echo $f->select('lock','Lås',array(
        array('text'=>'Olåst','val'=>'N'),
        array('text'=>'Låst','val'=>'Y'),
    ));
    echo $f->select('teamlock','Teamlås (endast skrivrättighet kan läsa)',array(
        array('text'=>'Olåst','val'=>'N'),
        array('text'=>'Låst','val'=>'Y'),
    ));
if ($this->forum['moderator']) 
    echo pathadmin::helper('banned1','Dölj för följade:',$f->val['banned1']);
echo $f->submit('Skicka');
echo $f->stop();

?></div>
