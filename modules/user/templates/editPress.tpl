   <script type="text/javascript" src="/template/scripts/tiny_mce/tiny_mce.js"></script>
   <script type="text/javascript" src="/template/scripts/base.js"></script>

    <div style="padding:20px">
<h1>
<?php

$f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->textarea('text','Ändra Presentation',80,35);
    echo $f->submit('Spara');
    echo $f->stop();

?>
 </h1>   </div>
