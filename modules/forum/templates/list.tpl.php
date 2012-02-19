<div style="padding-left:10px;">[ <a href="/forum.htm?action=clear">Markera allt som läst</a>] </div>
<table class="boxcontainer" cellpadding="0" cellspacing="0">
<?php 
    if (isset($this->forums)&&is_array($this->forums))
    foreach ($this->forums as $k => $line) {
    if (!isset($head) || $head != $line['group']) {
    ?>
        <tr>
            <td class="boxhead"><b><?php echo $line['group']; ?></b></td>
            <td width="40" class="boxhead">&nbsp;</td>
            <td width="50" class="boxhead"><b>Trådar</b></td>
            <td width="50" class="boxhead"><b>Inlägg</b></td>
            <td width="130" class="boxhead"><b>Senaste inlägget</b></td>
        </tr>
    <?php
        $head = $line['group'];
    }
        $c = ($line['new']&&($line['write']||$line['moderator']||$line['read'])) ? '6B532E' : '000000';

        if (intval($k/2)==($k/2)) {
            $e = str_split($c,2);
            $c = '';
            foreach($e as $d)
                $c .= dechex(hexdec($d)+17);
            
        }
        $c = '#'.$c;
    
    ?>
        <tr>
            <td style="padding:10px;padding-left:7px;background:<?php echo $c; ?>;">
                <?php if ($line['write']||$line['moderator']||$line['read']) { ?>
                    <h3><a href="/forum/<?php echo $line['url']; ?>.htm"><?php echo htmlspecialchars($line['head']); ?></a></h3>
                    <i><?php echo htmlspecialchars($line['desc']); ?></i>
                <?php } else { ?>
                    <h3 style="color:#666;"><?php echo htmlspecialchars($line['head']); ?></h3>
                    <i style="color:#444;"><?php echo htmlspecialchars($line['desc']); ?></i>
                <?php } ?>
            </td>
            <td width="40" nowrap style="padding:10px;padding-left:0px;padding-right:0px;background:<?php echo $c; ?>;" align="right">
                <?php if ($line['moderator']==1) { ?>
                <a href="/forum/<?php echo $line['url']; ?>.htm?action=settings"><img border=0 src="/modules/forum/templates/images/moderator.png" title="Moderator"></a>
                <?php } ?>
                <?php if ($line['write']==1) { ?>
                <img src="/modules/forum/templates/images/write.png" title="Skriv rättigheter">
                <?php } ?>
            </div>
            <td width="50" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;"><?php echo $line['topics']; ?> 
            </td>
            <td width="50" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;"><?php echo $line['posts']; ?>
            </td>
            <td width="130" style="padding:4px;padding-left:7px;background:<?php echo $c; ?>;">
            <?php if($line['last_poster']>''&&($line['write']==1||$line['moderator']==1||$line['read']==1)) { ?>
            
                <?php echo timestamp($line['last_timestamp']); ?><br>
                av <a href="/users/<?php echo path::encode($line['last_poster']); ?>.htm"><?php echo $line['last_poster']; ?></a>
            <?php } ?>

            </td>
        </tr>
<?php } ?>
</table>
