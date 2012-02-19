<h3>Gästbok</h3>
<style type="text/css">

.guestbookpost {
    margin-bottom:20px;
    width: 525px;
}

.guestbookpost .head {
background:#ddd;
border-bottom:#999 1px solid;
padding: 2px 5px;
}

</style>
<?php

    if(!$this->owner) {
        $f = new form('');

        echo $f->start();
        echo $f->textarea('message','Skriv ett inlägg:',70,2);
        echo $f->submit('skicka');
        echo $f->stop();
    }

?>

<?php if (isset($this->guestbook)) foreach ($this->guestbook as $key => $line) { ?>

    <div class="guestbookpost">
        <div class="head"><div style="float:right;">#<?php echo $line['postid']; ?></div><?php echo $line['from']; ?></div>
        <?php echo $line['text']; ?>
        <textarea></textarea>
    </div>

<?php } ?>
