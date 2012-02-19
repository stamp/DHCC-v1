<?php

if(isset($this->options['width']))
        echo '<table class="boxcontainer" width="'.$this->options['width'].'">';
    else
        echo '<table class="boxcontainer">';

echo "\t<tr>\n";
    if (isset($this->heads)&&is_array($this->heads))
        foreach($this->heads AS $name => $link){
            echo "\t\t<td class=\"boxhead\"><b><a href=\"?$link\">$name</a></b></td>\n";
        }
        echo '</tr>';
$i=1;
foreach($this->data AS $row => $line){
   if(is_int($i/2))
       echo '<tr style="background:#222;">';
   else
        echo '<tr class="row_'.$row.'">';
            foreach($line AS $name => $value){
                if(isset($this->options['encode']) && $this->options['encode'] == $name)
                    echo '<td class="cell_'.$name.'">'.path::encode($value).$name.'</td>';
                else
                    echo '<td class="cell_'.$name.'">'.$value.'</td>';

            }
        echo '</tr>';
        $i++;
    }
echo "</table>";
?>
