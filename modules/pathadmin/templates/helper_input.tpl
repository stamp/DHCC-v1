<label><?php echo $this->helper_name; ?>
    <input type="text" id="<?php echo $this->helper_id; ?>" name="<?php echo $this->helper_id; ?>" value="<?php echo $this->helper_value; ?>">
    <a onClick="window.open('/modules/pathadmin/helper.php?path=<?php echo $this->helper_path; ?>&show=<?php echo $this->helper_id; ?>', 'helper', 'toolbar=0,scrollbars=0,location=0,statusbar=0,menubar=0,resizable=0,width=500,height=300');">Ändra</a>
</label>

