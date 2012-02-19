<?php
echo '<script type="text/javascript">';
        echo 'function showhide(gid){
                if (document.getElementById(gid).style.display==\'none\'){
                        new Effect.BlindDown(gid,{duration:0.3});
                        new Element.update(gid+\'a\',\'» Göm lägg till lista\');
                    }
                    else{
                        new Effect.BlindUp(gid,{duration:0.3});
                        new Element.update(gid+\'a\',\'» Visa lägg till lista\');
                    } return false;}';
        echo '</script>';
if(isset($this->show)&& $this->show==1){
echo '<a class="showhide"  href="#" onClick="showhide(\'show\'); return false;"><span id="showa" >» Göm lägg till lista</span></a>';
echo '<div id="show">';
}
else{
echo '<a class="showhide"  href="#" onClick="showhide(\'show\'); return false;"><span id="showa" >» Visa lägg till lista</span></a>';
echo '<div style="display:none;" id="show">';
}
?>
<h1>Skapa lista<h1>
<?php

if(count($this->vals)==0 || is_object($this->vals)){
    unset($this->vals);
    $this->vals = array('0'=>'0');
}
    $f = new form('?done',$this->vals,$this->errors);
    
    echo $f->start();
    echo $f->text('name','Namn på listan');
    if(isset($this->admin) && $this->admin==1)
    echo $f->text('where','Where');
    $i=0;
    foreach($this->select AS $key=>$val){
        echo $f->select($i,'Fält',$this->select,array('onchange'=>'new Effect.BlindDown(\'test\',{duration:0.3});'));
        $i++;
    }
    echo $f->submit('Spara');
echo $f->stop();

echo '</div>';

?>
