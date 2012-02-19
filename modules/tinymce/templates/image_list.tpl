var tinyMCEImageList = new Array(
<?php

unset($ist);
function show($data) {
    global $ist;

    foreach ($data as $path => $type) {
        if (!isset($ist)) 
            $ist = true;
        else
            echo ",\n";

        echo "[\"".str_repeat('- ',$level)."{$type}\", \"{$path}\"]";
    }
}

show($this->tree);

?>
);
