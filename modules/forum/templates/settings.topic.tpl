
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
    echo $f->select('lock','L�s',array(
        array('text'=>'Ol�st','val'=>'N'),
        array('text'=>'L�st','val'=>'Y'),
    ));
    echo $f->select('teamlock','Teaml�s (endast skrivr�ttighet kan l�sa)',array(
        array('text'=>'Ol�st','val'=>'N'),
        array('text'=>'L�st','val'=>'Y'),
    ));
if ($this->forum['moderator']) 
    echo pathadmin::helper('banned1','D�lj f�r f�ljade:',$f->val['banned1']);
echo $f->submit('Skicka');
echo $f->stop();

?></div>
