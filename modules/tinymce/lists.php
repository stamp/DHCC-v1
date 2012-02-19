<?php
require_once('../../config.inc');

$tpl = new template('templates');

if (isset($_GET['list']))  
    if ($tinymce = core::load('tinymce')) {
        if ($_GET['list']=='images') 
            $tinymce->imageslist();
        
        if ($_GET['list']=='links')
            $tinymce->linkslist();
    }

?>
