<?php
    if (isset($this->logg)&&is_array($this->logg)) 
        foreach ($this->logg as $line) { 
            switch ($line['level']) {
                case E_ERROR:
                    echo '<div class="Eerror"><div><b>Fel</b><br />';
                    break;
                case E_WARNING:
                    echo '<div class="Ewarning"><div><b>Varning</b><br />';
                    break;
                case E_PARSE:
                    echo '<div class="Eparse">';
                    break;
                case E_NOTICE:
                    echo '<div class="Enotice"><div><b>Meddelande</b><br />';
                    break;
                case E_CORE_ERROR:
                    echo '<div class="Ecoreerror"><div><b>Systemfel</b><br />';
                    break;
                case E_CORE_WARNING:
                    echo '<div class="Ecorewarning"><div><b>System varning</b><br />';
                    break;
                case E_USER_ERROR:
                    echo '<div class="Eusererror"><div><b>Användarfel</b><br />';
                    break;
                case E_USER_WARNING:
                    echo '<div class="Euserwarning"><div><b>Användarvarning</b><br />';
                    break;
                case E_USER_NOTICE:
                    echo '<div class="Eusernotice"><div><b>Användarmeddelande</b><br />';
                    break;
            }
            ?>
    <?php echo $line['message']; ?></div>
</div>
<?php   }

?>
