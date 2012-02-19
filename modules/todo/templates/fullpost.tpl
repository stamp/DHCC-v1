<?php

echo "<div>";
    include('post.tpl');
    echo "\t\t<div id=\"edit_{$post['id']}\" class=\"addpost\" style=\"display:none;\"><img src=\"/modules/todo/templates/images/indicator.gif\"></div>\n";
echo '</div>';

?>
