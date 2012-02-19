<b>Uppkommande event</b>
<?php
	if (isset($this->ssvdata)&&is_array($this->ssvdata))
		foreach ($this->ssvdata as $key => $line){
         if ($line['time'] < date('Y-m-d H:i:s') && $line['status'] >= 0)
            $c = "EE0000";
         elseif ($line['status'] < 0)
            $c = "EECC00";
         else
            $c = "555555";


         if (intval($key/2)==($key/2)) {
           $e = str_split($c,2);
           $c = '';
           foreach($e as $d)
               $c .= dechex(hexdec($d)+17);
         }
         

         echo '<div style="background:#'.$c.';clear:both;width:140px;float:left;">'.$line['time'].'</div>';
         echo '<div style="background:#'.$c.';width:50px;float:left;">'.$line['y'].'-'.$line['x'].'</div>';
         echo '<div style="background:#'.$c.';width:110px;float:left;overflow:hidden;">'.$line['username'].'</div>';
         echo '<div style="background:#'.$c.';width:60px;float:left;"><a onClick="return wake(false,'.$line['id'].',this)">Väckt ('.$line['status'].')</a></div>';
         echo '<div style="background:#'.$c.';width:40px;float:left;"><a onClick="return wake(true,'.$line['id'].',this)">Vaken</a></div>';
	 if (trim($line['note'])>'')
	 	echo '<div style="background:#'.$c.';clear:both;padding-left:40px;"><i>'.$line['note'].'</i></div>';
		}
     
?>
