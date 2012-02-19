<h3>Registera ett nytt konto</h3>

<?php
if (isset($this->registrationsuccess)&&$this->registrationsuccess) {
    echo 'Ditt konto är skapat! Nu kan du logga in.';
} else {
    $f = new form('',$this->vals,$this->errors);

    echo $f->start();
    echo $f->hidden('action','register');
    echo $f->text('username','Användarnamn');
    echo $f->clear();
    echo $f->passwd('password','Lösenord');
    echo $f->passwd('password2','Repetera lösenord');
    echo $f->clear();
    echo $f->clear();
    echo $f->text('firstname','Förnamn');
    echo $f->text('lastname','Efternamn');
    echo $f->clear();
    echo $f->text('birthdate','Personnummer (ååmmdd-xxxx)');
    echo $f->clear();
    echo $f->submit('Skapa konto');
    echo $f->stop();
}

?>
