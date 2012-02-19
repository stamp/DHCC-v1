
<div style="position:absolute;left:100px;width:auto;z-index:10;">
<?php
error_reporting(0);
ini_set('display_errors',0);
    $t= explode('-',$_GET['plats']);
    echo '<form name="f" action="index.php" method="post">';
        echo '<input type="hidden" name="plats" value="'.$_GET['plats'].'">';
        echo '<h1>Boka plats</h1><br><input type="submit" value="Boka">';
            echo '<select name="user" size="27" style="widht:150px;height:auto;float:left;"onChange="changemail(this.options[this.selectedIndex].value,0);">';
            $box = array();
            $teams = array(0 => '');
            $data = file("http://crew.dreamhack.se/passport.asp?kontroll=8qw39m76vq89w2467nv89we6u8smec8c98wnvy5m98dxm5cqa");
            $data = explode('|',$data[0]);
            $c = '';
            foreach($data as $line) {
                $t = explode(';',$line);
                if (count($t)==2) {
                    $c = substr($t[0],1);
                    $teams = array_merge($teams,array($t[1] => $c));
                } else {
                    if (isset($t[1])) {
                        if (!isset($box[$c])||!is_array($box[$c])) $box[$c]=array();
                        $box[$c] = array_merge($box[$c],array($t[0] => $t[2].' ['.$t[1].']'));
                    }
                }
            }
            foreach ($teams as $key => $line) {
                if (!$key == 0) echo '<option value="'.$key.'">'.$line.'</option>';
            }
            echo '</select>'."\n";

            ?>
        <script language="javascript">
            <?

            foreach ($teams as $key => $line) {
                if ($key>0) {
                    echo "I$key = new Array(";
                    $a = 0;
                    foreach ($box[$line] as $key2 => $line2) {
                        if ($a==1) echo ',';
                        echo "'$line2'";
                        $a = 1;
                    }
                    echo ');'."\n";
                    
                    echo "C$key = new Array(";
                    $a = 0;
                    foreach ($box[$line] as $key2 => $line2) {
                        if ($a==1) echo ',';
                        echo "'$key2'";
                        $a = 1;
                    }
                    echo ');'."\n\n";
                }
            }
            
            ?>
            
          function changemail(Grupp,Mottagare) {
           if (Grupp > 0) {
            var arrC = eval('C' + Grupp);
            var arrI = eval('I' + Grupp);

            document.f.person.options.length = 0;
            //document.f.person.options[0] = new Option('----------- Välj team! ----------- ',0);

            selidx = 0;
            for(i = 0; i < arrC.length; i++) {
             if (arrC[i] == Mottagare) { selidx = i + 1; }
             document.f.person.options[i ] = new Option(arrI[i],arrC[i]);
            }

            document.f.person.options.selectedIndex = selidx;
           }
           else if (Grupp == 0) {
            document.f.person.options.length = 0;
            //document.f.person.options[0] = new Option('-------- Välj team först! --------',0);
           }
          }
        </script>
            <?php
            echo '</select>';
            echo '<select size=27 style="width:200px;height:auto;float:left;" name="person">';
            echo '</select>';
        echo '</form>';

?>
