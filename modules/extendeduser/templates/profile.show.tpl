<?php
    if ($this->userinfo['picture']>'') {
        echo '<img src="'.$this->userinfo['picture'].'" style="border: 2px solid #fff;float:left;margin:20px;">';
    } else {
        echo '<img src="/images/users/nopic.jpg" style="border: 2px solid #fff;float:left;margin:20px;">';
    }
?>
    <?php
    if (isset($this->write)&&$this->write&&isset($this->memberteams)&&is_array($this->memberteams)) {

    echo '<div style="border:1px solid #fff;padding:5px;">';
        echo "<a href=\"/settings/".path::encode($this->userinfo['username'])."\" style=\"float:right;margin-right:20px;\">Administration</a>";
        if($this->eventinfo['checkedin'] == "0000-00-00 00:00:00")
            echo "<div><a href=\"?checkin=".$this->userinfo['uid']."\" >Checka In</a></div>";
        else
            echo '<div>Incheckad</div>';
        if($this->eventinfo['checkedout']== "0000-00-00 00:00:00")
            echo "<div><a href=\"?checkout=".$this->userinfo['uid']."\" >Checka Ut</a></div>";
        else
            echo '<div>Utcheckad</div>';
    echo '</div>';

    }
    
    ?>
<div style="float:left;margin-top:20px;">
    <h1 style="display: inline;"><?php echo $this->userhead; ?></h1><br />
    <i style="font-size:90%"><?php echo $this->userinfo['firstname'].' '.$this->userinfo['lastname']; ?></i><br>
    <table width="470" height="50" style="margin-top:10px;">
        <tr>
            <td nowrap valign="top"  style="padding-right:5px;">
                <p><b>Från</b><br><i><?php echo $this->user_profile['city']; ?></i></p>
                
                <p><b>Antal inloggningar</b><br><i><?php echo $this->userinfo['logincount']; ?> ggr</i></p>
                <p><b>Senast inloggad</b><br><i><?php echo timestamp($this->userinfo['latestlogin']); ?></i></p>
                
            </td>
            <td nowrap valign="top" width="120" style="border-left:2px solid #fff;padding-left:5px;padding-right:5px;">
                <h3>Teamtillhörighet</h3>
                <p>
                    <?php
                    
                    $a = '';
                    if (isset($this->memberteams)&&is_array($this->memberteams))
                    foreach ($this->memberteams as $line) {
                        echo $a.'<a href=/team/'.path::encode($line['name']).'.htm>'.$line['name'].'</a>';
                        if (isset($line['level'])) {
                            echo ' <b>('.$line['level'].')</b>';
                        }
                        $a = '<br>';
                    }
                    
                    ?>
                </p>
            </td>
            <td nowrap valign="top" width="170" style="border-left:2px solid #fff;padding-left:5px;padding-right:5px;">
                <h3>Erfarenhet</h3>
                <p>
                    <?php
                    
                    $a = '';
                    if (isset($this->history)&&is_array($this->history))
                    foreach ($this->history as $line) {
                        echo $a.'<b>'.$line['shortname'].'</b> - '.$line['name'].'</a>';
                        if (isset($line['level'])) {
                            echo ' <b>('.$line['level'].')</b>';
                        }
                        $a = '<br>';
                    }
                    
                    ?>
                </p>
            </td>
        </tr>
    </table>
</div>
<div style="clear:both;padding-left:20px;">
<?php echo nl2br($this->user_profile['press']); ?>

</div>
<div style="clear:both;padding:10px;">
<table>
<? if(isset($this->contact)&&is_array($this->contact)) foreach ($this->contact as $line) {
     ?> <tr>
         <td><b><?php echo $line['medium']; ?>:</b></td> <td><?php echo $line['text']; ?></td>
         <?php } ?></tr>
         </table>
</div>
