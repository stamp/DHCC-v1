<div style="padding:10px;">
<?php

$f = new form('?action=edit&post='.$_GET['post'],$this->vals,$this->errors);

echo $f->start();

echo $f->simple('text','Text',100,25,'width:600px;');
echo $f->submit('Skicka');
echo $f->stop();

?></div>
