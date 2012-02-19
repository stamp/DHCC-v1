<table  border=0 cellpadding=0 cellspacing=0 id="listtable">
<?php 
    if(isset($this->searchdata))
    foreach($this->searchdata as $i => $line) { ?>
    <tr id="row_<?php echo $line['access']; ?>" class="<?php echo $line['access']; ?> row">
        <td width=14 id="typ_<?php echo $line['access']; ?>" onClick="select(this);">
            <div class="img" title="<?php echo $line['access']; ?>" id="sel_<?php echo $i; ?>"></td>
        <td width=50 id="typ_<?php echo $line['access']; ?>" onClick="select(this);"><?php echo $line['type']; ?></td>
        <td width=290 id="nam_<?php echo $line['access']; ?>" onClick="select(this);"><div><?php echo $line['name']; ?></div></td>
        <td id="tea_<?php echo $line['access']; ?>" onClick="select(this);"><?php echo $line['event']; ?>&nbsp;</td>
    </tr>
<?php } ?>
</table>
