<a href="#" onClick="showhide('form'); return false;"><div id="button">� �ndra</div></a>
    <div id="form" style="padding:20px;display:none;">
<h1>
<?php

$f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->textarea('press','�ndra Presentation',80,35);
    echo $f->submit('Spara');
    echo $f->stop();


?>
<script type="text/javascript">
function showhide(gid){
                if (document.getElementById(gid).style.display=='none'){
                        new Effect.BlindDown(gid,{duration:0.3});
                        new Element.update('button','� G�m �ndra');
                    }
                    else{
                        new Effect.BlindUp(gid,{duration:0.3});
                        new Element.update('button','� �ndra');
                    } return false;}
</script>

 </h1>   </div>
