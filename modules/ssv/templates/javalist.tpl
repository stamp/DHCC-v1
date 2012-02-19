<script>
<?php
	if (isset($this->ssvdata))
		foreach ($this->ssvdata as $l){
			echo "$(\"{$l['x']}-{$l['y']}\").style.backgroundColor={$l['color']};";
            if ($l['uid']>0)
			    echo "$(\"{$l['x']}-{$l['y']}\").className='U{$l['uid']}';";
		}
?>
</script>
