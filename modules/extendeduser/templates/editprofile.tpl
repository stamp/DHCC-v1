<div style="padding:10px;">
<?php if ($this->user['uid']==$_SESSION['id']) { ?>
<h1>Personuppgifter</h1>
<p>Fyll i all information noga och kom ih�g att trycka p� spara l�ngst ner p� sidan.<br>
<b>*</b> betyder att det �r obligatoriskt</p> 
<?php
} else {
?>
<h1>Profile of <?php echo $this->user['username']; ?></h1>
<?php
}

$f = new form('',$this->vals,$this->errors,'test');
echo '<form method="post" action="" style="width:520px;" id="test" enctype="multipart/form-data" >';
//echo $f->text('firstname','F�rnamn',array('ro'=>!$this->lock));
//echo $f->text('lastname','Efternamn',array('ro'=>!$this->lock));
echo $f->text('email','Prim�r email *');
echo $f->text('email2','Alternativ email');
echo $f->text('street','Adress *');
echo $f->text('postcode','Postnummer *');
echo $f->text('city','Postort *');
echo $f->select('country','Land *',$this->countrys);

echo $f->clear();
echo '<h2>Physical details</h2><div style="padding-left:20px;">';
//echo $f->text('birthdate','F�delsedatum',array('ro'=>!$this->lock));
echo $f->clear();
echo $f->textarea('medical','Medicinsk information *<br><i>Specificera kroniska sjukdomar och allergier (t.ex. diabetes, astma, epilepsi) som v�r sjukv�rdspersonal b�r k�nna till. Skriv �ven speciella �tg�rder (saker vi ska g�ra om du drabbas) och vilka mediciner som du tar f�r sjukdommen.</i>',73);
echo $f->textarea('food','Mat information *<br><b>K�ket m�ste ha information om Dina eventuella allegier:</b>',73);
echo $f->clear();
echo '</div>';


echo '<h2>Telefonnummer<br><i>Endast till f�r n�dfall (visas inte p� din profil)</i></h2><div style="padding-left:20px;">';
    echo $f->select('primaryphontype','Type',array(
        array('text'=>'Mobil','val'=>'Mobil'),
        array('text'=>'Hem','val'=>'Hem'),
        array('text'=>'Arbete','val'=>'Arbete')
    ));
    echo $f->text('primaryphone','Primaryphone *');
    echo $f->clear();
    echo $f->select('secondaryphonetype','Type',array(
        array('text'=>'Mobil','val'=>'Mobil'),
        array('text'=>'Hem','val'=>'Hem'),
        array('text'=>'Arbete','val'=>'Arbete'),
        array('text'=>'Annat','val'=>'Annat')
    ));
    echo $f->text('secondaryphone','Secondaryphone');
    echo $f->clear();
    echo $f->text('ice','ICE* (In case of emergency, Nummer till anh�rig ifall du blir skadad)');
    echo $f->clear();
echo '</div>';


echo '<h2>Privacy</h2><div style="padding-left:20px;">';
    echo $f->checkbox('share','Visa min profil f�r icke nuvarande crew');
echo '</div>';

echo $f->clear();
echo $f->submit('Spara');
echo $f->stop();
echo $f->clear();



?>


</div>
