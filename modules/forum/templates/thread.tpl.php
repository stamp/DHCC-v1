<?php if($this->forum['write']||$this->forum['moderator']) { ?>
<script language="javascript">
    function vote(v,i) {
        var opt = {
            method: 'get',
            onSuccess: function(t) {
                eval(t.responseText);
            },
            onFailure: function(t) {
                alert('Error ' + t.status + ' -- ' + t.statusText);
            }
        };
        $('vote_'+i).innerHTML = '<span style="color:#555">sparar...</span>';
        new Ajax.Request('/fetch.php?path=<?php echo $this->fullpath; ?>&vote='+i+'&dir='+v, opt);
        return false;
    }

    function abuse(i) {
        $('abuse_'+i).innerHTML = 
            '<span style="display:none;">'+
                $('abuse_'+i).innerHTML+
            '</span>'+
            '<div style="position:absolute;left:-457px;top:-120px;width:300px;background:#222;border:0px solid #444;padding:10px;">'+
                '<h3>Anmälan</h3>'+
                '<form onSubmit="send_abuse('+i+');return false;" id="abuseform_'+i+'">'+
                    '<textarea name="text" style="width:100%;height:75px;"></textarea>'+
                    '<input type="submit" value="Anmäl" style="display:inline;" /> eller <a href="" onClick="return hide_abuse('+i+');">Avbryt</a>'+
                '</form>'+
            '</div>';
        return false;

    }

    function send_abuse(i) {
        var opt = {
            method: 'post',
            postBody: Form.serialize('abuseform_'+i),
            onSuccess: function(t) {
                eval(t.responseText);
            },
            onFailure: function(t) {
                alert('Error ' + t.status + ' -- ' + t.statusText);
            }
        };
        $('abuse_'+i).innerHTML = '<span style="color:#555">skickar...</span>';
        new Ajax.Request('/fetch.php?path=<?php echo $this->fullpath; ?>&abuse='+i, opt);
        return false;
    }

    function hide_abuse(i) {
        $('abuse_'+i).innerHTML = $('abuse_'+i).firstChild.innerHTML;
        return false;
    }
</script>

[ <a href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=reply">Svara</a> ] [ <a onClick="Element.show('speedanswer');" style="cursor:pointer">Snabbsvara</a> ]

<div id="speedanswer" style="display:none;">
    <form method="post" action="?action=reply" enctype="multipart/form-data">
    <div class="post" style="margin:10px;"> 
        <div class="boxhead" style="border-bottom:2px solid #fff;">
            <b>Snabbsvara</b>
        </div>
        <div class="content" style="border:1px solid #fff;background:#aaa;border-top:0;border-left:0;border-right:0;margin-left:10px;width:620px;">
            <div>
                <textarea name="text" style="width:100%;height:50px;"></textarea>
                <input type="submit" value="Skicka">
            </div>
        </div>
        <div style="clear:both;"></div>
        &nbsp;
    </div>
    </form>
</div>

<?php }?>

<div style="padding:10px;">
<?php 


if (isset($this->posts)&&is_array($this->posts)) 
foreach ($this->posts as $line) { ?>
    <?php if ($line['status']=='active') {?>

        <div class="post" <?php if ($line['new']) echo 'style="background:#6B532E;"><a name="new"></a'; ?>> 
            <div class="boxhead" style="border-bottom:2px solid #fff;">
                <div style="float:right;"> <?php if ($line['new']) echo '<b>Ny!</b> '; ?>
                    
                    <?php
                    if (!$line['voted']&&($this->forum['write']||$this->forum['moderator'])) {
                        echo '<span id="vote_'.$line['id'].'"><a href="" onClick="return vote(\'false\','.$line['id'].');">-</a> ';
                        echo '('.$line['points'].')';
                        echo '<a href="" onClick="return vote(\'true\','.$line['id'].');">+</a></span>';
                    } else {
                        echo '('.$line['points'].')';
                    }
                    
                    ?>
                        
                    <?php 
                        if ($line['version']>1) 
                            echo 'Version: '.$line['version'];
                    ?>

                    <?php if($this->topic['own']||$this->forum['moderator']) { ?>
                    [ <a class="admin" href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=hide&post=<?php echo $line['id']; ?>">hide</a> ]
                    <?php }?>

                    <?php if($this->forum['moderator']==1) { ?>
                    [ <a class="admin" href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=remove&post=<?php echo $line['id']; ?>">remove</a> ]
                    <?php }?>

                    <?php if($this->forum['moderator']||$line['uid']==$_SESSION['id']) { ?>
                    [ <a class="admin" href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=edit&post=<?php echo $line['id']; ?>">edit</a> ]
                    <?php }?>
                
                    <div id="abuse_<?php echo $line['id']; ?>" style="position:relative;display:inline;">[ <a href="" onClick="return abuse(<?php echo $line['id']; ?>);">anmäl</a> ]</div>

                    <?php if($this->forum['write']||$this->forum['moderator']) { ?>
                    [ <a href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=quote&post=<?php echo $line['id']; ?>">quote</a> ]
                    <?php }?>
                    # <?php echo $line['post']; ?>
                </div>
                <b><?php echo timestamp($line['timestamp']); ?></b>
            </div>
            <div class="info">
                <center>
                    <a href='/users/<?php echo path::encode($line['user']); ?>.htm'><?php echo $line['user']; ?></a><br>
                    <?php 
                    if (isset($line['team'])&&$line['team']>'') echo $line['team'].'<br>'; 
                    if (isset($line['picture'])&&$line['picture']>'') {
                    ?>
                    <img src='<? echo $line['picture']; ?>'>
                    <?php } ?>

                </center>
            </div>
            <div class="content" style="border:1px solid #fff;background:#aaa;border-top:0;border-left:0;border-right:0;margin-left:5px;">
                <div>
                    <?php echo $line['text']; ?>
                </div>
            </div>
            <div style="clear:both;"></div>
            &nbsp;
        </div>
    <?php } elseif ($line['status']=='removed') { ?>
        <?php if ($this->forum['moderator']) { ?>
           <div class="post" style=""> 
                <div class="boxhead" style="border-bottom:2px solid #ccc;color:#999;background:#555">
                    <div style="float:right;">
                        <?php if($this->forum['moderator']==1) { ?>
                        [ <a class="admin" href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=unremove&post=<?php echo $line['id']; ?>">unremove</a> ]
                        <?php } ?>

                        REMOVED
                        # <?php echo $line['post']; ?>
                    </div>
                    <b><?php echo timestamp($line['timestamp']); ?></b>
                </div>
                <div class="info" style=";background:#555;">
                    <center>
                        <a href='/users/<?php echo path::encode($line['user']); ?>.htm'><?php echo $line['user']; ?></a><br>
                        <?php 
                        if (isset($line['team'])&&$line['team']>'') echo $line['team'].'<br>'; 
                        if (isset($line['picture'])&&$line['picture']>'') {
                        ?>
                        <img src='http://honeydew.gayhyllan.se/images/users/thumbs/small_<? echo $line['picture']; ?>.jpg'>
                        <?php } ?>

                    </center>
                </div>
                <div class="content" style="border:1px solid #fff;color:#333;background:#ccc;border-top:0;border-left:0;border-right:0;margin-left:5px;">
                    <div>
                        <?php echo $line['text']; ?>
                    </div>
                </div>
                <div style="clear:both;"></div>
                &nbsp;
            </div>
        <?php } else { ?>
            <div class="post" style=""> 
                <div class="boxhead" style="color:#777;border:0;font-size:9px;padding-top:1px;padding-bottom:1px;">
                    <div style="float:right;font-size:9px;">
                        REMOVED
                        # <?php echo $line['post']; ?>
                    </div>
                    <b><?php echo timestamp($line['timestamp']); ?></b> (<a href='/users/<?php echo path::encode($line['user']); ?>.htm'><?php echo $line['user']; ?></a>)
                </div>
            </div>
        <?php } ?>
    <?php } elseif ($line['status']=='hidden') { ?>
        <?php if ($this->forum['moderator']||$this->topic['own']) { ?>
           <div class="post" style=""> 
                <div class="boxhead" style="border-bottom:2px solid #ccc;color:#999;background:#555">
                    <div style="float:right;">
                        <?php if($this->forum['moderator']==1) { ?>
                        [ <a class="admin" href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=unremove&post=<?php echo $line['id']; ?>">unhide</a> ]
                        <?php } ?>

                        HIDDEN
                        # <?php echo $line['post']; ?>
                    </div>
                    <b><?php echo timestamp($line['timestamp']); ?></b>
                </div>
                <div class="info" style=";background:#555;">
                    <center>
                        <a href='/users/<?php echo path::encode($line['user']); ?>.htm'><?php echo $line['user']; ?></a><br>
                        <?php 
                        if (isset($line['team'])&&$line['team']>'') echo $line['team'].'<br>'; 
                        if (isset($line['picture'])&&$line['picture']>'') {
                        ?>
                        <img src='http://honeydew.gayhyllan.se/images/users/thumbs/small_<? echo $line['picture']; ?>.jpg'>
                        <?php } ?>

                    </center>
                </div>
                <div class="content" style="border:1px solid #fff;color:#333;background:#ccc;border-top:0;border-left:0;border-right:0;margin-left:5px;">
                    <div>
                        <?php echo $line['text']; ?>
                    </div>
                </div>
                <div style="clear:both;"></div>
                &nbsp;
            </div>
        <?php } else { ?>
            <div class="post" style=""> 
                <div class="boxhead" style="color:#777;border:0;font-size:9px;padding-top:1px;padding-bottom:1px;">
                    <div style="float:right;font-size:9px;">
                        HIDDEN
                        # <?php echo $line['post']; ?>
                    </div>
                    <b><?php echo timestamp($line['timestamp']); ?></b> (<a href='/users/<?php echo path::encode($line['user']); ?>.htm'><?php echo $line['user']; ?></a>)
                </div>
            </div>
        <?php } ?>
    <?php } ?>
<?php } 
?>
<?php if($this->forum['write']||$this->forum['moderator']) { ?>
[ <a href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $this->topic['url']; ?>.htm?action=reply">Svara</a> ] [ <a onClick="Effect.toggle('speedanswer2','blind');" style="cursor:pointer">Snabbsvara</a> ]

<div id="speedanswer2" style="display:none;">
    <form method="post" action="?action=reply" enctype="multipart/form-data">
    <div class="post" style="margin:10px;"> 
        <div class="boxhead" style="border-bottom:2px solid #fff;">
            <b>Snabbsvara</b>
        </div>
        <div class="content" style="border:1px solid #fff;background:#aaa;border-top:0;border-left:0;border-right:0;margin-left:10px;width:620px;">
            <div>
                <textarea name="text" style="width:100%;height:50px;"></textarea>
                <input type="submit" value="Skicka">
            </div>
        </div>
        <div style="clear:both;"></div>
        &nbsp;
    </div>
    </form>
</div>

<?php }?>
</div>
