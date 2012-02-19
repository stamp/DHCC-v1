var tinyMCELinkList = new Array(
<?php

unset($ist);
function show($data) {
    global $level,$ist;

    foreach ($data as $line) {
        if (!isset($ist)) 
            $ist = true;
        else
            echo ",\n";

        echo "[\"".str_repeat('- ',$level)."{$line['head']}\", \"{$line['path']}\"]";
        if (isset($line['childs'])) {
            $level++;
            show($line['childs']);
            $level--;
        }
    }
}

show($this->tree);

?>
);
