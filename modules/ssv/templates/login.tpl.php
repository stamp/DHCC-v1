<center><table height="100%" width="160"><tr><td>
<?php
    
    $form = new form('/signin');
    echo $form->start();
    echo $form->text('username','Anv�ndarnamn');
    echo $form->passwd('password','L�senord');
    echo $form->submit('Logga in');
    echo $form->stop();

?>
</td></tr></table></center>
