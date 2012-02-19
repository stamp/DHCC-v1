<?php
$f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->hidden('action','register');
    echo $f->select('medium','Medium:',array(
        array('text'=>'Telefon','val'=>'Telefon'),
        array('text'=>'Mobil','val'=>'Mobil'),
        array('text'=>'Msn Messenger','val'=>'Msn Messenger'),
        array('text'=>'Skype','val'=>'Skype'),
        array('text'=>'Email','val'=>'Email'),
        array('text'=>'ICQ','val'=>'ICQ'),
        array('text'=>'Gtalk','val'=>'Gtalk'),
        array('text'=>'Jabber','val'=>'Jabber'),
        array('text'=>'IRC','val'=>'IRC')
        ));

    echo $f->clear();
    echo $f->text('text','Text/nummer');
echo $f->submit('Lägg till');
    echo $f->stop();

?>

