
<?php
if (isset($this->news)&&is_array($this->news))
    foreach($this->news as $line) {
        echo "\t\t<div id=\"box\">\n";
        echo "\t\t\t<div id=\"head\">\n";
        echo "\t\t\t\t<div>".timestamp($line['timestamp'])."</div>";
        echo "\t\t\t\t{$line['head']}\n";
        echo "\t\t\t</div>\n";
        echo $line['text']."\n";
        echo "\t\t</div>\n";
    }


?>