<?php
    echo "\t<form onSubmit=\"return add(this);return false;\" id=\"addform$id\"><div style=\"display:none;\"><div class=\"addpost\">\n";
        
        echo "\t\tTo-do item text:<input type=\"text\" name=\"head\">\n";

        echo "\t\tDescription: <textarea name=\"text\"></textarea>\n";
        
        echo "\t\t<input type=\"submit\" value=\"Add\"> or <a onClick=\"hideAdd(this);\">Cancel</a>\n";

    echo "\t</div></div><a onClick=\"showAdd(this)\">Add a new to-do post</a></form>\n\n";
?>
