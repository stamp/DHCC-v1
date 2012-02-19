<center><table height="100%" width="160"><tr><td>
<?php
    
    $form = new form('/signin');
    echo $form->start();
    echo $form->text('username','Användarnamn');
    echo $form->passwd('password','Lösenord');
    echo $form->submit('Logga in');
    echo $form->stop();

?>
</td></tr></table></center>
