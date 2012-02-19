<a href="#" onClick="showhide('form'); return false;"><div id="button">» Ändra</div></a>
    <div id="form" style="padding:20px;display:none;">
<h1>
<?php

$f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->textarea('press','Ändra Presentation',80,35);
    echo $f->submit('Spara');
    echo $f->stop();


?>
<script type="text/javascript">
function showhide(gid){
                if (document.getElementById(gid).style.display=='none'){
                        new Effect.BlindDown(gid,{duration:0.3});
                        new Element.update('button','» Göm Ändra');
                    }
                    else{
                        new Effect.BlindUp(gid,{duration:0.3});
                        new Element.update('button','» Ändra');
                    } return false;}
</script>

 </h1>   </div>
