<h3>Logga in</h3>

<?php

$f = new form('',$this->vals,$this->errors);

echo $f->start();
echo $f->text('signin_username','Användarnamn');
echo $f->passwd('signin_password','Lösenord');
echo $f->submit('Logga in');
echo $f->stop();

?>
