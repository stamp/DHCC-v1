<b>Plats <?php echo $this->x; ?> - <?php echo $this->y; ?> �r ledig!</b>

<br><br><form name="faas" onSubmit="
if (document.f.person.value>0) {
    upd(<?php echo $this->x; ?>,<?php echo $this->y; ?>,document.f.person.value);
} else {
    alert('Du m�ste v�lja en person i listan!');
}return false; ">
<input type="submit" value="Boka in person p� sovplats"></form>
