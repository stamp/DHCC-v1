<h1>Teammedlemmars utvärdering av teamansvariga</h1>
<?php

if(isset($this->eval)&&is_array($this->eval))
foreach ($this->eval as $line) {?>
<div style="border:1px solid #999;margin:10px;padding:10px;">
    <?php echo $line; ?>
</div>
<?php
}
?>
