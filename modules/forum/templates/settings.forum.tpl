
<div style="padding:10px;">
<?php

$f = new form('?action=settings',$this->vals,$this->errors);

echo $f->start();

echo $f->text('group','Grupp');
echo $f->text('head','Rubrik');
echo pathadmin::helper('read','L�sr�ttigheter',$f->val['read']);
echo pathadmin::helper('write','Skrivr�ttigheter',$f->val['write']);
echo pathadmin::helper('moderator','Moderatorer',$f->val['moderator']);
echo $f->textarea('desc','Beskrivning');
#echo $f->textarea('read','L�sr�ttigheter');
#echo $f->textarea('write','Skrivr�ttigheter');
#echo $f->textarea('moderator','Moderatorer');
echo $f->submit('Spara');
echo $f->stop();

?></div>
