<div style="margin-top:15px;">
<?php if (isset($this->data['head'])) { ?><b><?php echo $this->data['head']; ?></b><br><?php } ?>
    <?php if (isset($this->data['definition']['desc'])) { ?><i><?php echo $this->data['definition']['desc']; ?></i><br><?php } ?>
    <div style="width:300px;margin-top:3px;">
        <?php if (isset($this->data['definition']['label-right'])) { ?><div style="float:right;"><?php echo $this->data['definition']['label-right']; ?></div><?php } ?>
        <?php if (isset($this->data['definition']['label-left'])) { ?><div style="float:left;"><?php echo $this->data['definition']['label-left']; ?></div><?php } ?>
    <div class="slider" style="clear:both;border:1px solid #fff;background:#333;margin-bottom:10px;width:300px;"><div style="width:15px;height:25px;background-color:#f00;cursor:pointer;"> </div><input type="hidden" name="field_<?php echo $this->data['qid']; ?>" value="<?php if(isset($this->data['value'])&&is_numeric($this->data['value'])) echo $this->data['value']; ?>"></div>
    </div>
</div>
