<div style="padding:10px;"><h1>Statistik</h1>
<?php

function ashow($key,$data) {
    if (is_array($data)) {
        foreach ($data as $key => $line) {
            if (is_array($line)) {
                echo $key;
                echo '<div style="padding-left:10px;margin-bottom:10px;">';
            } else {
                echo '<div style="padding-left:10px;">';
            }
            ashow($key,$line);
            echo '</div>';
        }
    } else {
        echo '<div style="float:left;width:100px;overflow:hidden;">'.$key.'</div>';
        echo '<div style="float:left;">'.$data.'</div><br>';
    }
}

foreach ($this->stats as $key => $line) {
?>
    <div class="boxcontainer" style="float:left;width:300px;">
        <div class="boxhead"><?php echo $key; ?></div>
        <div style="padding:5px;">
        <?php echo ashow('',$line); ?>
        <div style="clear:both;"></div>
        </div>
    </div>
<?php 
}
?>
</div>
<div style="clear:both;">
</div>
