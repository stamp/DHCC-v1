<div style="padding:5px;">
    <h1>Byt Användarnamn</h1>

    <?php


$f = new form('',$this->vals,$this->errors);
echo $f->start();
echo $f->text('username','Användarnamn');
echo $f->clear();
echo $f->submit('Byt nick');
echo $f->stop();

?>

    </div>
