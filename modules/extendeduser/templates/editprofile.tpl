<div style="padding:10px;">
<?php if ($this->user['uid']==$_SESSION['id']) { ?>
<h1>Personuppgifter</h1>
<p>Fyll i all information noga och kom ihåg att trycka på spara längst ner på sidan.<br>
<b>*</b> betyder att det är obligatoriskt</p> 
<?php
} else {
?>
<h1>Profile of <?php echo $this->user['username']; ?></h1>
<?php
}

$f = new form('',$this->vals,$this->errors,'test');
echo '<form method="post" action="" style="width:520px;" id="test" enctype="multipart/form-data" >';
//echo $f->text('firstname','Förnamn',array('ro'=>!$this->lock));
//echo $f->text('lastname','Efternamn',array('ro'=>!$this->lock));
echo $f->text('email','Primär email *');
echo $f->text('email2','Alternativ email');
echo $f->text('street','Adress *');
echo $f->text('postcode','Postnummer *');
echo $f->text('city','Postort *');
echo $f->select('country','Land *',$this->countrys);

echo $f->clear();
echo '<h2>Physical details</h2><div style="padding-left:20px;">';
//echo $f->text('birthdate','Födelsedatum',array('ro'=>!$this->lock));
echo $f->clear();
echo $f->textarea('medical','Medicinsk information *<br><i>Specificera kroniska sjukdomar och allergier (t.ex. diabetes, astma, epilepsi) som vår sjukvårdspersonal bör känna till. Skriv även speciella åtgärder (saker vi ska göra om du drabbas) och vilka mediciner som du tar för sjukdommen.</i>',73);
echo $f->textarea('food','Mat information *<br><b>Köket måste ha information om Dina eventuella allegier:</b>',73);
echo $f->clear();
echo '</div>';


echo '<h2>Telefonnummer<br><i>Endast till för nödfall (visas inte på din profil)</i></h2><div style="padding-left:20px;">';
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
    echo $f->text('ice','ICE* (In case of emergency, Nummer till anhörig ifall du blir skadad)');
    echo $f->clear();
echo '</div>';


echo '<h2>Privacy</h2><div style="padding-left:20px;">';
    echo $f->checkbox('share','Visa min profil för icke nuvarande crew');
echo '</div>';

echo $f->clear();
echo $f->submit('Spara');
echo $f->stop();
echo $f->clear();



?>


</div>
