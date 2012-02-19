<?php if($this->forum['write']==1||$this->forum['moderator']==1) { ?>
[ <a href="/forum/<?php echo $this->forum['url']; ?>.htm?action=new">Ny tråd</a> ]
<?php } ?>
[ <a href="/forum/<?php echo $this->forum['url']; ?>.htm?action=clear">Markera allt som läst</a> ]

<table class="boxcontainer" width="560" cellpadding="0" cellspacing="0">
    <tr>
        <td class="boxhead"><b>Trådar</b></td>
        <td width="50" class="boxhead">&nbsp;</td>
        <td width="50" class="boxhead"><b>Inlägg</b></td>
        <td width="130" class="boxhead"><b>Senaste inlägget</b></td>
    </tr>
    <?php
    if (isset($this->topics)&&is_array($this->topics))
    foreach ($this->topics as $key => $line) { ?>
    <?
    if ( $line['new'] && !($line['teamlock']=='Y' && !$this->forum['write'] && !$this->forum['moderator'] ) ) {
        $c = '6B532E';
    } elseif ($line['sticky']==1) {
        $c = '111111';
        $line['tags'] .= ',Klistrad';
    } elseif ($line['sticky']==2) {
        $c = '222222';
        $line['tags'] .= ',Superklistrad';
    } else {
        $c = '000000';
    }

    if (intval($key/2)==($key/2)) {
        $e = str_split($c,2);
        $c = '';
        foreach($e as $d)
            $c .= dechex(hexdec($d)+17);
        
    }

    $c = '#'.$c;
    ?>
    <tr>
        <td style="padding:10px;padding-left:7px;background:<?php echo $c; ?>;">
            <?php
                $l = array();
                if ($tags = explode(',',$line['tags']))
                    foreach ($tags as $line2)
                        if (trim($line2)>'')
                            $l[] = "<span style=\"color:#0c0;font-size:10px;float:left;margin-right:2px;\">$line2</span>";

                echo implode($l,', ');
                
            ?>
            <?php if ($line['teamlock']=='Y'&&!$this->forum['write']&&!$this->forum['moderator']) { ?>
                <h3 style="color:#999;"><?php echo $line['head']; ?></h3>
            <?php } else { ?>
                <h3><a href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $line['url']; ?>.htm#new"><?php echo $line['head']; ?></a></h3>
            <?php } ?>
            <i style="color:#999;font-size:10px;">Startad <?php echo timestamp($line['created']); ?> av <a href="/users/<?php echo path::encode($line['owner']); ?>.htm"><?php echo $line['owner']; ?></a></i>
        </td>
        <td width="50" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;" align="right">
            <?php if ($line['lock']=='Y') { ?>
            <img src="/modules/forum/templates/images/denied.gif" title="Låst">
            <?php } ?>
           
            <?php if ($line['teamlock']=='Y') { ?>
            <img src="/modules/forum/templates/images/ikon-teamonly.gif" title="Teamlåst">
            <?php } ?>

            <?php if ($this->forum['moderator']||$line['own']) { ?>
            <a href="/forum/<?php echo $this->forum['url']; ?>/<?php echo $line['url']; ?>.htm?action=settings"><img border=0 src="/modules/forum/templates/images/moderator.png" title="Moderator"></a> 
            <?php } ?>
        </td>
        <td width="50" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;"><?php echo $line['posts']; ?></td>
        <td width="130" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;">
            <?php if($line['last_poster']>'') { ?>
            
                <?php echo timestamp($line['last_timestamp']); ?><br>
                av <a href="/users/<?php echo path::encode($line['last_poster']); ?>.htm"><?php echo $line['last_poster']; ?></a>
            <?php } ?>

        </td>
    <tr>
    <?php } ?>

</table>
