<?php
        echo "\t\t<form onSubmit=\"return save(this);\" id=\"editf_{$this->data['id']}\">To-do item text:<input type=\"text\" name=\"head\" value=\"{$this->data['head']}\">\n";

        echo "\t\tDescription: <textarea name=\"text\">{$this->data['text']}</textarea>\n";
        
        $val = array('active', 'pending', 'halted', 'aborted', 'finished');
        echo "\t\tStatus: <select name=\"status\">";
        foreach ($val as $line) {
            echo "<option value=\"$line\"";
            if ($line == $this->data['status']) echo " selected";
            echo ">$line</option>";
        }        
        echo "\t\t</select>";

        echo "Process:<input type=\"text\" name=\"process\" value=\"{$this->data['process']}\">";
        echo "Ansvarig:<input type=\"text\" name=\"responsible\" value=\"{$this->data['responsible']}\">";
        
        echo "\t\t<input type=\"submit\" value=\"Add\"> or <a onClick=\"hideEdit(this);\">Cancel</a></form>\n";

?>
