<h1>Path admin - Add new</h1>
<a href="?<? if(isset($_GET['page'])) echo '&page='.$_GET['page']; ?>">Path list</a><br>
<br>
<?php

    $f = new form('',$this->vals,$this->errors);
    
    echo $f->start();
    echo $f->text('path','Path Name (One word)');
    echo $f->select('parent','Parent path',$this->tree);
    echo $f->select('type','Type',
        array(
            array(
                'text'  => 'Normal',
                'val'   => 'normal'
            ),
            array(
                'text'  => 'Varabel',
                'val'   => 'var'
            ),
            array(
                'text'  => 'Redirection',
                'val'   => 'redir'
            ),
            array(
                'text'  => 'Extended',
                'val'   => 'extend'
            ),
            array(
                'text'  => 'Include',
                'val'   => 'include'
            )
        )
    );
    echo $f->submit('Add');


?>
