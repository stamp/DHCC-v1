<?php
    function gpicture($src) {
        
        if (strlen($src)==32) 
            $file = 'images/users/thumbs/micro_'.$src.'.jpg';
        else
            $file = 'images/users/old/'.$src;
        
        if (file_exists($file)&&rtrim($file,'/')==$file)
            return '/' . $file;

        return false;
    }
?>
<style>

#searchbox input, #searchbox label {
    display:inline;
}

</style>



<div id="searchbox">Sök
    <?php

    $f = new form('',$_POST);

    echo $f->start();
    echo $f->text('search','',array('id'=>'form_search'));
    echo $f->submit('Sök');
    echo $f->stop();

    ?>
</div>
<script>
$('form_search').focus();
</script>
<?php
if (isset($this->search)&&is_array($this->search)) 
    foreach ($this->search as $line) {
        include('box.tpl');
    }

?>
