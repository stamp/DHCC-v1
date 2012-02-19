
<div style="padding:10px;">

<?php if(isset($this->username)) echo "<h1 style=\"margin-bottom:10px;\">{$this->username}´s gästbok</h1>"; ?>
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
        <div class="post" style="<?php if ($line['new']=='new'&&$line['owner']) echo 'background:#500;'; ?>"> 
            <div class="boxhead" style="border-bottom:2px solid #fff;">
                <div style="float:right;">
                    <?php if($line['new']=='new'&&$line['owner']) echo "<b>OLÄST</b>"; ?>
                    <?php if($line['new']=='read'&&$line['owner']) echo "<i>obesvarad</i>"; ?>

                    <?php if ($line['owner']&&$this->gbid!=$line['from']) { ?>
                        <a onClick="Element.show('answer<?php echo $line['postid']; ?>');">Svara</a> - 
                    <?php } ?>
                    <?php if ($this->gbid!=$line['from']) { ?>
                        <a href="?dialog=<?php echo $line['from']; ?>">Dialog</a>
                    <?php } else echo '<span style="color:#999;">Dialog</span>'; ?>
                    <?php if ($line['owner']) { ?>
                         - <a href="">Ta bort</a>
                    <?php } ?>
                    #<?php echo $line['postid']; ?>
                </div>
                <b><?php echo timestamp($line['timestamp']); ?></b>
            </div>
            <div class="info">
                <center>
                    <?php echo $line['user']; ?><br>
                    <?php 
                    if (isset($line['team'])&&$line['team']>'') echo $line['team'].'<br>'; 
                    if (isset($line['picture'])&&$line['picture']>'') {
                    ?>
                    <img src='http://honeydew.gayhyllan.se/images/users/thumbs/small_<? echo $line['picture']; ?>.jpg'>
                    <?php } ?>

                </center>
            </div>
            <div class="content" style="border:1px solid #fff;background:#aaa;border-top:0;border-left:0;border-right:0;margin-left:5px;">
                <div>
                    <?php echo nl2br($line['text']); ?>
                    <div id="answer<?php echo $line['postid']; ?>" style="display:none;">
                        <br />
                        <form action="" method="post">
                            <input type="hidden" class="hidden" name="post" value="<?php echo $line['postid']; ?>"/>
                            <textarea name="message" style="width:425px;border:1px solid #ccc;"></textarea><br>
                            <input type="submit" class="submit" value="skicka" /> 
                        </form>
                    </div>

                </div>
            </div>
            <div style="clear:both;"></div>
            &nbsp;
        </div>

<?php } ?>
</div>
