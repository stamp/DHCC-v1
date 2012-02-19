

<div class="searchHit"  style="position:relative;float:left;width:200px;padding:10px;cursor:pointer;" onMouseOver="this.style.background='#222222';" onMouseOut="this.style.background='';" onClick="location.href='/users/<?php echo path::encode($line['_username']); ?>'">
    <?php if ($p = gpicture($line['_picture'])) { ?>
    <img src="<?php echo $p; ?>" style="width:30px;height:40px;border:1px solid #999;position:absolute;top:10px;left:10px;" />
    <?php } else { ?>
    <div style="width:30px;height:40px;border:1px solid #999;position:absolute;top:10px;left:10px;">&nbsp;</div>
    <?php } ?>
    
    <div style="margin-left:42px;overflow:hidden;">
        <div style="background: #555;font-weight:900;padding: 2px 5px;<?php if ($line['_high']) echo 'background-color: #6B532E;'; ?>"><?php echo htmlspecialchars($line['_username']) ?></div>
        <div style="padding: 2px 5px;">
        <?php
            $ret = '';
            foreach($line as $key => $field) {
                if (substr($key,0,1) != '_') {
                    if ($ret=='')
                        $ret = $field;
                    else
                        $ret .= '<br />' . $field;
                }
            }
            
            echo $ret;
        ?>
        </div>
    </div>
    <div style="clear:both;"></div>
</div>

