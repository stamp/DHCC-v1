<div style="padding:10px;">
<h1>Ändra bild</h1>
<?php
if (isset($this->thumsmade)){
    send(E_NOTICE,'Bilduppladdning lyckades! Miniatyrer skapade.');
}

if (isset($this->fel)){
    echo '<div class="errorbox">';
    if ($this->fel==0) {
        
        send(E_USER_ERROR,'Typfel! Bilden du laddar upp måste vara jpeg/png/gif!');
    } elseif ($this->fel==1) {
        switch ($this->error) {
            case UPLOAD_ERR_OK:
                echo 'Felkod saknas';
                send(E_USER_ERROR,'Felkod saknas');
                break;
            case UPLOAD_ERR_INI_SIZE:
                send(E_USER_ERROR,'Max filstorlek överskriden! [ini]');
                break;
            case UPLOAD_ERR_FORM_SIZE:
                send(E_USER_ERROR,'Max filstorlek överskriden! [f]');
                break;
            case UPLOAD_ERR_PARTIAL:
                send(E_USER_ERROR,'Bara en del av filen kom fram! Försök igen.');
                break;
            case UPLOAD_ERR_NO_FILE:
                send(E_USER_ERROR,'Ingen fil skickades!');
                break;
            case UPLOAD_ERR_NO_TMP_DIR:
                send(E_USER_ERROR,'Kan inte skriva till TEMP!');
                break;
            case UPLOAD_ERR_CANT_WRITE:
                send(E_USER_ERROR,'Misslyckades att skriva till hårddisk!');
                break;
            default:
                send(E_USER_ERROR,'Okänt fel!');
                break;
        }
    } elseif ($this->fel==2) {
        send(E_USER_ERROR,'Kunde inte öppna filen för omformatering!');
    } else {
        send(E_USER_ERROR,'Okänt fel! #'.$this->fel);
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
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Ej ännu godkänd av CCO!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==2) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#123A17;">Godkännd!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture_status']==3) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Underkänd! Därför bortplockad.</div>
    <?php } ?>
   
    <div style="padding:10px;border-bottom: 1px solid #999;">
        <div style="width:120px;height:160px;padding:5px;border: 1px dashed #999;margin-left:55px;">
            <?php if (isset($this->picture)&&is_array($this->picture)&&$b = user::picture($this->picture['picture'])) { ?>
                <img style="width:120px;height:160px;" src="<?php echo $b; ?>">
            <?php } else { ?>
                <i style="font-size:10px;">Välj en bild nedan och ladda upp.</i>
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
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Ej ännu godkänd av CCO!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==2) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#123A17;">Godkännd!</div>
    <?php } elseif (isset($this->picture)&&is_array($this->picture)&&$this->picture['picture2_status']==3) { ?>
        <div style="padding:5px;border-bottom: 1px solid #999;background:#520000;">Underkänd! Därför bortplockad.</div>
    <?php } ?>
    
    <div style="padding:10px;border-bottom: 1px solid #999;">
        <div style="width:120px;height:160px;padding:5px;border: 1px dashed #999;margin-left:55px;">
            <?php if (isset($this->picture)&&is_array($this->picture)&&$b2 = user::picture($this->picture['picture2']) ) { ?>
                <img style="width:120px;height:160px;" src="<?php echo $b2; ?>">
            <?php } else { ?>
                <i style="font-size:10px;">Välj en bild nedan och ladda upp.</i>
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
<I>Här kan du ladda upp en egen bild på dig till din egna sida och till din namnskylt. Bilden till din sida ställer vi inga mer krav på än att den ska vara på dig själv. Om vi ser en bild som inte är på dig så kommer vi att radera den. Bilden till din namnskylt ställer vi dock höga krav på. Bilden ska föreställa dig själv och ditt ansikte! Ansiktet ska ta upp större delen av bilden och synas tydligt. När du laddat upp din bild får du möjlighet att beskära den. Om du laddar upp bild till din namnskylt ska du då bara välja ut ditt ansikte!</i>
</div>
