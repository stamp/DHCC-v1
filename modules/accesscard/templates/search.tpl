
<?php
    function gpicture($src) {
        
        if (strlen($src)==32) 
            $file = 'images/users/thumbs/micro_'.$src.'.jpg';
        else
            $file = 'images/users/old/'.$src;
        
        if (file_exists($file)&&rtrim($file,'/')==$file)
            return '/' . $file;

        return false;
    }

    function checkPicture($data) {
        if ($data['picture2']>'')
            if ($p = gpicture($data['picture2']))
                return $p;

        if ($data['picture']>'')
            if ($p = gpicture($data['picture']))
                return $p;

        return false;
    }
?>
<style>

#searchbox input, #searchbox label {
    display:inline;
}

</style>

<script language="javascript">
    function display() {
        boxes = document.getElementsByClassName('selectuser');
        
        users = '';

        boxes.each(
            function(node) {
                if(node.checked)
                    users = users + ',' + node.id.substring(5,node.id.length);
            }
        );
        if (users.length>0)
           window.open('/modules/pdf/test.php?path=/start&users='+users);
        else
            alert('Du måste välja någon att skriva ut!!');
    }
</script>

<div id="searchbox">Sök
    <?php

    $f = new form('',$_POST);

    echo $f->start();
    echo pathadmin::helper('search','','');
    echo $f->submit('Sök');
    echo $f->stop();

    ?>
</div>
<?php
if (isset($this->hits)&&is_array($this->hits)) { ?>

<input type="button" onClick="display();" value="Skriv ut">
<?php
    foreach ($this->hits as $line) { ?>

    <div class="searchHit" id="searchHits"  style="position:relative;float:left;width:200px;padding:10px;cursor:pointer;">
        <?php if ($p = checkPicture($line)) { ?>
        <img src="<?php echo $p; ?>" style="width:30px;height:40px;border:1px solid #999;position:absolute;top:10px;left:10px;" />
        <?php } else { ?>
        <div style="width:30px;height:40px;border:1px solid #999;position:absolute;top:10px;left:10px;">&nbsp;</div>
        <?php } ?>
        
        <div style="margin-left:42px;overflow:hidden;">
            <div style="background: #555;font-weight:900;padding: 2px;<?php if (!$p) echo 'background-color: #c00;'; ?>">
                <input type="checkbox" class="selectuser" <?php if ($p) echo 'checked="checked"'; ?> id="user_<?php echo $line['uid'];?>"  style="height:auto;width:auto;display:inline;">
                <?php echo htmlspecialchars($line['username']) ?>
            </div>
            <div style="padding: 2px 5px;">
            <?php echo $line['firstname'] . ' ' . $line['lastname']; ?><br>
            <?php echo $line['size']; ?>

            </div>
        </div>
        <div style="clear:both;"></div>
    </div>
<?php
    }
?>

<?php
    }
?>
