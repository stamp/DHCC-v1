<table width="50%">
<?php
if(isset($this->lists) && is_array($this->lists))
foreach($this->lists AS $key=>$val){
    echo '<tr>';
    //echo '<td>'.$key.'</td>';
    echo '<td><a style="font-size:12px;"href="?list='.$key.'">'.$val['name'].'</a><br /></td>';
    if(isset($this->write) && $this->write){
    echo '<td><a href="?edit='.$key.'">» ändra</a><br /></td>';
    echo '<td><a href="?remove='.$key.'">» ta bort</a><br /></td>';
    }
    echo '</tr>';
}

?>
</table>
