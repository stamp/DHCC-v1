<style>

.inplaceeditor-empty {

        font-style: italic;
            color: #999;
}
}
</style>
Gl�m inte http:// i urlen
<?php
$f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->text('text','Namn:');
    echo $f->text('cnt','Antal(frivillig):');
    echo $f->clear();
    echo $f->text('url','Url(frivillig):');
    echo $f->text('store','Aff�r(frivillig):');
    echo $f->clear();
    echo $f->submit('L�gg till');
    echo $f->stop();

echo '</div>';


?>
