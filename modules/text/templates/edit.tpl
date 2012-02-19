<?php

$f = new form('',array('text'=>$this->content));

echo $f->start();
echo $f->editor('text','',100,25,'width:100%;height:500px;');
echo $f->submit('Spara');
echo $f->stop();

?>
