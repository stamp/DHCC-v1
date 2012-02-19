<script language="javascript">
    id = "<?php echo $this->mail['Id']; ?>";

    function reply() {
        location.href="compose.htm?reply="+id;
    }
    function replyall() {
        location.href="compose.htm?replyall="+id;
    }
    function forward() {
        location.href="compose.htm?forward="+id;
    }
    function rm() {
        location.href="?remove="+id;
    }
</script>
<h1 style="margin-bottom:10px;"><?php echo $this->mail['Subject']; ?></h1>

    <div style="padding:3px;background:#333;margin:0px 10px;">
        <input type="button" value="Svara avsändare" style="display:inline;" onClick="reply();">
        <input type="button" value="Svara alla" style="display:inline;" onClick="replyall();">
        <input type="button" value="Skicka vidare" style="display:inline;" onClick="forward();">
        <input type="button" value="Ta bort" style="display:inline;" onClick="rm();">
    </div>

<?php
    if ($this->mail['Prio']=='High')
        echo '<div class="high">';
    elseif ($this->mail['Prio']=='Low')
        echo '<div class="low">';
    else    
        echo '<div>';

?>
    <table class="boxcontainer" width="100%" id="headers">
        <tr>
            <td style="background:#222;"><b>Från</b></td>
            <td style="background:#222;"><?php echo $this->mail['From']; ?></td>
        </tr>
        <tr>
            <td><b>Till</b></td>
            <td><?php echo $this->mail['To']; ?></td>
        </tr>
        <tr>
            <td style="background:#222;"><b>Prioritet</b></td>
            <td style="background:#222;"><?php echo $this->mail['Prio']; ?></td>
        </tr>
        <tr>
            <td style="background:#222;"><b>Spam</b></td>
            <td style="background:#222;"><?php if(isset($this->mail['Spam'])) echo $this->mail['Spam']; else echo 'no'; ?></td>
        </tr>

        <tr>
            <td><b>Datum & tid</b></td>
            <td><?php echo $this->mail['Date']; ?></td>
        </tr>
    </table>

    <div id="content">
        <?php echo nl2br($this->mail['Content']); ?>
    </div>

    <div style="padding:3px;background:#333;margin:0px 10px;">
        <input type="button" value="Svara avsändare" style="display:inline;" onClick="reply();">
        <input type="button" value="Svara alla" style="display:inline;" onClick="replyall();">
        <input type="button" value="Skicka vidare" style="display:inline;" onClick="forward();">
        <input type="button" value="Ta bort" style="display:inline;" onClick="rm();">
    </div>
</div>
