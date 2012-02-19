<?php
if(!isset($post))
    $post = $this->data;

$process = ($post['process']!=0) ? ($post['process']/2) : 0;
echo "\t<div class=\"post todo_{$post['status']}\" id=\"post_{$post['id']}\" >\n";

echo "\t\t<div class=\"process {$post['status']}\">\n";
    echo "\t\t\t<div class=\"text\">{$post['status']}</div>\n";
    echo "\t\t\t<div class=\"meter {$post['status']}\" style=\"width:{$process}px;\"></div>\n";
echo "\t\t</div>\n";

if ($post['responsible']>'')
    echo $post['responsible1'].': ';

if ($this->write)
    echo "\t\t<a onClick=\"load(this);\">".(trim($post['head'])>''?$post['head']:'<span>(emty)</span>')."</a>\n";
else
    echo "\t\t{$post['head']}\n";

if ($post['text']>'') echo "\t\t<div class=\"desc\">{$post['text']}</div>\n";
echo "\t</div>\n\n";

?>
