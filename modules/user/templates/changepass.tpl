<div style="padding:5px;">
    <h1>Byt l�senord</h1>

    <?php


$f = new form('',$this->vals,$this->errors);
echo $f->start();
if ($this->user == $_SESSION['id']) {

        echo $f->passwd('old','Gammalt l�senord',true);
            echo $f->clear();
                echo $f->clear();
}
echo $f->passwd('new','Nytt l�senord');
echo $f->clear();
echo $f->passwd('new2','Bekr�fta l�senord');
echo $f->clear();
echo $f->submit('Byt');
echo $f->clear();
echo $f->stop();

?>

    </div>
