<div style="padding:5px;">
    <h1>Byt lösenord</h1>

    <?php


$f = new form('',$this->vals,$this->errors);
echo $f->start();
if ($this->user == $_SESSION['id']) {

        echo $f->passwd('old','Gammalt lösenord',true);
            echo $f->clear();
                echo $f->clear();
}
echo $f->passwd('new','Nytt lösenord');
echo $f->clear();
echo $f->passwd('new2','Bekräfta lösenord');
echo $f->clear();
echo $f->submit('Byt');
echo $f->clear();
echo $f->stop();

?>

    </div>
