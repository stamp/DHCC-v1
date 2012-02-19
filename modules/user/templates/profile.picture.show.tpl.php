<div style="padding:10px;">
<h1>�ndra bild</h1>
<?php
if (isset($this->thumsmade)){
    send(E_NOTICE,'Bilduppladdning lyckades! Miniatyrer skapade.');
}

if (isset($this->fel)){
    echo '<div class="errorbox">';
    if ($this->fel==0) {
        
        send(E_USER_ERROR,'Typfel! Bilden du laddar upp m�ste vara jpeg/png/gif!');
    } elseif ($this->fel==1) {
        switch ($this->error) {
            case UPLOAD_ERR_OK:
                echo 'Felkod saknas';
                send(E_USER_ERROR,'Felkod saknas');
                break;
            case UPLOAD_ERR_INI_SIZE:
                send(E_USER_ERROR,'Max filstorlek �verskriden! [ini]');
                break;
            case UPLOAD_ERR_FORM_SIZE:
                send(E_USER_ERROR,'Max filstorlek �verskriden! [f]');
                break;
            case UPLOAD_ERR_PARTIAL:
                send(E_USER_ERROR,'Bara en del av filen kom fram! F�rs�k igen.');
                break;
            case UPLOAD_ERR_NO_FILE:
                send(E_USER_ERROR,'Ingen fil skickades!');
                break;
            case UPLOAD_ERR_NO_TMP_DIR:
                send(E_USER_ERROR,'Kan inte skriva till TEMP!');
                break;
            case UPLOAD_ERR_CANT_WRITE:
                send(E_USER_ERROR,'Misslyckades att skriva till h�rddisk!');
                break;
            default:
                send(E_USER_ERROR,'Ok�nt fel!');
                break;
        }
    } elseif ($this->fel==2) {
        send(E_USER_ERROR,'Kunde inte �ppna filen f�r omformatering!');
    } else {
        send(E_USER_ERROR,'Ok�nt fel! #'.$this->fel);
    }
    echo '</div>';
}
get();

?>
<div style="width:250px;border: 1px solid #999;float:left;margin-top:20px;margin-right:10px;">
    <div style="padding:5px;border-bottom: 1px solid #999;background:#222;"><b>Bild till pressentation</b></div>
     <?php if (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==0) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Bild saknas!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==1) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Ej �nnu godk�nd av CCO!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==2) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#123A17;">Godk�nnd!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==3) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Underk�nd! D�rf�r bortplockad.</div>
    <?php } ?>
   
    <div style="padding:10px;border-bottom: 1px solid #999;">
        <div style="width:120px;height:160px;padding:5px;border: 1px dashed #999;margin-left:55px;">
            <?php if (isset($this->picture)&&is_array($this->picture)&&$b = user::picture($this->picture['picture'])) { ?>
                <img style="width:120px;height:160px;" src="<?php echo $b; ?>">
            <?php } else { ?>
                <i style="font-size:10px;">V�lj en bild nedan och ladda upp.</i>
            <?php } ?>
        </div>
    </div>
    <?php if (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture']>'') { ?>
    <div style="padding:5px;border-bottom: 1px solid #999;background:#222;text-align:center;"><a href="picture.htm?rm">Ta bort bild</a></div>
    <?php } ?>
    <div style="padding:5px;background:#222;">
    <form enctype="multipart/form-data" method="POST">
        <input class="hidden" type="hidden" name="target" value="picture">
        <input class="hidden" type="hidden" name="MAX_FILE_SIZE" value="3000000" />
        Ladda upp ny bild: <input name="filen" type="file" /><br><br>
        <input type="submit" value="Skicka bild" />
    </form>
    </div>
</div>

<div style="width:250px;border: 1px solid #999;float:left;margin-top:20px;margin-right:10px;">
    <div style="padding:5px;border-bottom: 1px solid #999;background:#222;"><b>Bild till namnskylt</b></div>
    
    <?php if (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==0) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Bild saknas!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==1) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Ej �nnu godk�nd av CCO!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==2) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#123A17;">Godk�nnd!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==3) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Underk�nd! D�rf�r bortplockad.</div>
    <?php } ?>
    
    <div style="padding:10px;border-bottom: 1px solid #999;">
        <div style="width:120px;height:160px;padding:5px;border: 1px dashed #999;margin-left:55px;">
            <?php if (isset($this->picture)&&is_array($this->picture)&&$b2 = user::picture($this->picture['picture2']) ) { ?>
                <img style="width:120px;height:160px;" src="<?php echo $b2; ?>">
            <?php } else { ?>
                <i style="font-size:10px;">V�lj en bild nedan och ladda upp.</i>
            <?php } ?>
        </div>
    </div>
    <?php if (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2']>'') { ?>
    <div style="padding:5px;border-bottom: 1px solid #999;background:#222;text-align:center;"><a href="picture.htm?rm2">Ta bort bild</a></div>
    <?php } ?>

    <div style="padding:5px;background:#222;">
    <form enctype="multipart/form-data" method="POST">
        <input class="hidden" type="hidden" name="target" value="picture2">
        <input class="hidden" type="hidden" name="MAX_FILE_SIZE" value="3000000" />
        Ladda upp ny bild: <input name="filen" type="file" /><br><br>
        <input type="submit" value="Skicka bild" />
    </form>
    </div>
</div>

</div>
<div style="clear:both;padding:20px;">
<I>H�r kan du ladda upp en egen bild p� dig till din egna sida och till din namnskylt. Bilden till din sida st�ller vi inga mer krav p� �n att den ska vara p� dig sj�lv. Om vi ser en bild som inte �r p� dig s� kommer vi att radera den. Bilden till din namnskylt st�ller vi dock h�ga krav p�. Bilden ska f�rest�lla dig sj�lv och ditt ansikte! Ansiktet ska ta upp st�rre delen av bilden och synas tydligt. N�r du laddat upp din bild f�r du m�jlighet att besk�ra den. Om du laddar upp bild till din namnskylt ska du d� bara v�lja ut ditt ansikte!</i>
</div>
