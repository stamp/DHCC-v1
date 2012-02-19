<div style="padding:10px;">
<?php

$f = new form('?save',$this->vals,$this->errors);

echo $f->start();
echo $f->text('head','Rubrik'); 
echo $f->simple('text','Text',100,25,'width:100%;');
echo $f->submit('Skicka');
echo $f->stop();

?>
</div>
