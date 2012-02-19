<div style="margin-top:15px;">
    <?php if (isset($this->data['head'])) { ?><b><?php echo $this->data['head']; ?></b><br><?php } ?>
    <?php if (isset($this->data['definition']['desc'])) { ?><i><?php echo $this->data['definition']['desc']; ?></i><br><?php } ?>
        <textarea style="width:300px;margin-top:3px;" name="field_<?php echo $this->data['qid']; ?>"<?php if(isset($this->data['rows'])&&is_numeric($this->data['rows'])) echo ' rows="'.$this->data['rows'].'"'; ?>><?php if(isset($this->data['value'])) echo $this->data['value']; ?></textarea>
</div>
