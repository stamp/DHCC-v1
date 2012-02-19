<!--
<select onChange="url=this.options[this.selectedIndex].value; if(url){ location.href=url; }" style="width:185px">
<?php   foreach ($this->events as $key=> $line) { ?>
        <option value="?event=<?php echo $line['id']; ?>"<?php if (isset($_SESSION['event'])&&$line['id']==$_SESSION['event']){
          echo ' selected';  
          $selected=$key;
        } ?>><?php echo $line['name']; ?></option>
<?php   } ?>
</select><br>
-->
<div id="selEvent" style="display:none;position:relative;border-bottom:1px solid #fff;">
    <div style="padding:10px;">
<?php   foreach ($this->events as $key=> $line) { ?>
        <a href="?event=<?php echo $line['id']; ?>"><?php echo $line['name']; ?></a><br>
<?php   } ?>
    </div>
</div>
<br>
<?php 
if (isset($this->events) &&is_array($this->events)) 
    echo '<center onClick="Effect.toggle(\'selEvent\',\'blind\');" style="cursor:pointer;">'.$this->events[$selected]['name'].'</center><br>';
?>

