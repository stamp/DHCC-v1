<div style="padding:15px;">
<?php if ($this->user['id']==$_SESSION['id']) { ?>
<h1>Evenemangsuppgifter</h1>
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

$arrive = array();
$depart = array();
$ev = db::fetchSingle("SELECT * FROM events WHERE active='Y'");
$time = strtotime($ev['start']);
$time2 = strtotime($ev['end']);

for( $i=-4;$i < 1;$i++) {
	$arrive[] = array(
		'text'	=> date('d M - l',$time+(24*60*60*$i)),
		'val' => date('Y-m-d',$time+(24*60*60*$i))
	);
}

for( $i=-1;$i < 2;$i++) {
	$depart[] = array(
		'text'	=> date('d M - l',$time2+(24*60*60*$i)),
		'val' => date('Y-m-d',$time2+(24*60*60*$i))
	);
}



echo $f->select('arrive','Anl�nder datum *',$arrive);
echo $f->text('arrive_time','Anl�nder tid *');
echo $f->select('depart','�ker datum *',$depart);
echo $f->text('depart_time','�ker tid *');
echo $f->clear();
echo $f->text('car','Eventuellt registreringsnummer till bil ');
echo $f->checkbox('dinner','Jag kommer att n�rvara vid slutmiddagen');

echo $f->clear();
echo $f->select('size','Storlek p� crew T-shirt:',array(
    array('text'=>'V�lj:','val'=>''),
    array('text'=>'Herr XXL','val'=>'Herr XXL'),
    array('text'=>'Herr XL', 'val'=>'Herr XL'),
    array('text'=>'Herr L',  'val'=>'Herr L'),
    array('text'=>'Herr M',  'val'=>'Herr M'),
    array('text'=>'Herr S',  'val'=>'Herr S'),
    array('text'=>'Dam XL',  'val'=>'Dam XL'),
    array('text'=>'Dam L',   'val'=>'Dam L'),
    array('text'=>'Dam M',   'val'=>'Dam M'),
    array('text'=>'Dam S',   'val'=>'Dam S')
),array('ro'=>1));
echo $f->select('gsize','Storlek p� pressent T-shirt:',array(
    array('text'=>'V�lj:','val'=>''),
    array('text'=>'Herr XXL','val'=>'Herr XXL'),
    array('text'=>'Herr XL', 'val'=>'Herr XL'),
    array('text'=>'Herr L',  'val'=>'Herr L'),
    array('text'=>'Herr M',  'val'=>'Herr M'),
    array('text'=>'Herr S',  'val'=>'Herr S'),
    array('text'=>'Dam XL',  'val'=>'Dam XL'),
    array('text'=>'Dam L',   'val'=>'Dam L'),
    array('text'=>'Dam M',   'val'=>'Dam M'),
    array('text'=>'Dam S',   'val'=>'Dam S')
),array('ro'=>1));
echo "<b>Man kan inte l�ngre �ndra sina t-shirt val och best�llningarna �r skickade!</b>";
echo $f->clear();
?>
<style>
	#matt th {
		font-size:12px;
		border-bottom:1px solid #444;
		border-right:1px solid #444;
		padding: 2px 5px;
		background:#222;
	}

	#matt {
		border-left:1px solid #444;
		border-top:1px solid #444;
		width:100%;
		margin-left:10px;
	}

	#matt td {
		border-bottom:1px solid #444;
		border-right:1px solid #444;
		padding: 2px 5px;
	}
</style>
<h2>M�ttabell f�r t-shirtar</h2>
<table id="matt" cellspacing=0 cellpadding=0>
	<tr>
		<th>(cm)</th>
		<th nowrap>L�ngd bak</th>
		<th nowrap>� br�st</th>
		<th nowrap>� nedkant p� tr�ja</th>
		<th nowrap>Arml�ngd fr�n axels�m</th>
	</tr>
	<tr>
		<td nowrap>Herr XXL</td>
		<td>78</td>
		<td>62</td>
		<td>62</td>
		<td>24</td>
	</tr>
	<tr>
		<td nowrap>Herr XL</td>
		<td>78</td>
		<td>58</td>
		<td>58</td>
		<td>23</td>
	</tr>
	<tr>
		<td nowrap>Herr L</td>
		<td>76</td>
		<td>56</td>
		<td>56</td>
		<td>22</td>
	</tr>
	<tr>
		<td nowrap>Herr M</td>
		<td>74</td>
		<td>53</td>
		<td>53</td>
		<td>21</td>
	</tr>
	<tr>
		<td nowrap>Herr S</td>
		<td>72</td>
		<td>50</td>
		<td>50</td>
		<td>20</td>
	</tr>
	<tr>
		<th>(cm)</th>
		<th nowrap>L�ngd bak</th>
		<th nowrap>� br�st</th>
		<th nowrap>� nedkant p� tr�ja</th>
		<th nowrap>Arml�ngd fr�n tr�jans mitt bak</th>
	</tr>
	<tr>
		<td nowrap>Dam XL</td>
		<td>64</td>
		<td>51</td>
		<td>51</td>
		<td>19</td>
	</tr>
	<tr>
		<td nowrap>Dam L</td>
		<td>62</td>
		<td>48</td>
		<td>48</td>
		<td>19</td>
	</tr>
	<tr>
		<td nowrap>Dam M</td>
		<td>60</td>
		<td>45</td>
		<td>45</td>
		<td>18</td>
	</tr>
	<tr>
		<td nowrap>Dam S</td>
		<td>58</td>
		<td>42</td>
		<td>42</td>
		<td>18</td>
	</tr>
</table>

<?php
echo $f->clear();

echo '<h2>Sovinformation</h2><div style="padding-left:20px;">';
echo $f->select('were','Var sover du:',array(
    array('text'=>'Teamet','val'=>'Teamet'),
    array('text'=>'Inte p� elmia','val'=>'Inte p� elmia'),
    array('text'=>'Med pojke/flicka','val'=>'Med pojke/flicka')
));
echo $f->select('on_what','Vad sover du p�:',array(
    array('text'=>'Enb�dds luftmadrass','val'=>'Enb�dds luftmadrass'),
    array('text'=>'Tv�b�dds luftmadrass','val'=>'Tv�b�dds luftmadrass'),
    array('text'=>'Liggunderlag','val'=>'Liggunderlag')
));
echo $f->clear();
echo $f->text('with_who','Vem sover du med? (om n�gon):');
echo $f->text('wakeup','Hur du v�cks l�ttast');
echo $f->select('snore','Snarkar:',array(
    array('text'=>'Ja','val'=>'Ja'),
    array('text'=>'Halvh�gt','val'=>'Halvh�gt'),
    array('text'=>'Ibland','val'=>'Ibland'),
    array('text'=>'Nej','val'=>'Nej'),
    array('text'=>'Vet ej','val'=>'Vet ej')
));

    echo $f->checkbox('sleep_hard','Sover h�rt:');
echo $f->clear();
echo '</div>';





echo $f->clear();
echo $f->submit('Spara');
echo $f->stop();
echo $f->clear();



