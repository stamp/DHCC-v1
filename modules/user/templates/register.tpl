<h3>Registera ett nytt konto</h3>

<?php
if (isset($this->registrationsuccess)&&$this->registrationsuccess) {
    echo 'Ditt konto �r skapat! Nu kan du logga in.';
} else {
    $f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->hidden('action','register');
    echo $f->text('username','Anv�ndarnamn');
    echo $f->clear();
    echo $f->passwd('password','L�senord');
    echo $f->passwd('password2','Repetera l�senord');
    echo $f->clear();
    echo $f->clear();
    echo $f->text('firstname','F�rnamn');
    echo $f->text('lastname','Efternamn');
    echo $f->clear();
    echo $f->text('birthdate','Personnummer (��mmdd-xxxx)');
    echo $f->clear();
    echo $f->submit('Skapa konto');
    echo $f->stop();
}

?>
