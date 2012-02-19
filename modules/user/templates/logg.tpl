<h1>Event logg</h1>
<table width=50%>
    <tr>
        <th>Time</th>
        <th>Event</th>
    </tr>

<?php

if(isset($this->logg))
    foreach($this->logg as $line) {
        echo "\t<tr>\n";
           echo "\t\t<td>{$line['timestamp']}</td>\n";
           echo "\t\t<td>{$line['text']}</td>\n";
        echo "\t</tr>\n";
    }
        

?>
</table>
