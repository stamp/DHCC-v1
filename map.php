<?php
include('config.inc');

// getLatLongOnCity($city) {{{
    function getLatLongOnCity($city) {

        global $long,$lat;
        
        $city = str_replace(array('å','ä','ö'),array('a','a','o'),strToLower($city));

        $xml_parser = xml_parser_create();
        xml_set_element_handler($xml_parser, "startElement", "endElement");
        xml_set_character_data_handler($xml_parser, "characterData");

        $long = 0;
        $lat = 0;

        $lines = file("http://worldkit.org/geocoder/rest/?city=$city,SW");

        foreach ($lines as $data) {
           if (!xml_parse($xml_parser, $data)) {
               die(sprintf("XML error: %s at line %d",
                           xml_error_string(xml_get_error_code($xml_parser)),
                           xml_get_current_line_number($xml_parser)));
           }
        }
        
        xml_parser_free($xml_parser);

        if ($long>0 && $lat>0) return array('long'=>$long,'lat'=>$lat);
        return false;

    }
// }}}
// xml parser functions {{{
    function startElement($parser, $name, $attrs)
    {
        global $type;
        $type = $name;
    }

    function endElement($parser, $name)
    {
        global $type;
        $type = '';
    }

    function characterData($parser, $data) {
        global $type,$long,$lat;
        if ($type=='GEO:LONG') $long = $data;
        if ($type=='GEO:LAT') $lat = $data;
    }

// }}}
// the marker class {{{
class marker {
    protected $tabs = array();
    protected $name = '';
    public $icon = '';
    
    public function __construct($name,$pos) 
    {
        $this->name = $name;
        if (isset($pos['long']) && isset($pos['lat'])) {
            $this->lat = $pos['lat'];
            $this->long = $pos['long'];
        } 
    }

    public function addTab ($head, $text) 
    {
        $head = mysql_real_escape_string($head);
        $text = mysql_real_escape_string($text);
        $this->tabs[] = array('head'=>$head,'text'=>$text);
    }

    public function generate()
    {
        $ret = "var tabs{$this->name} = [\n";
        foreach ($this->tabs as $line) {
            $ret .= "\t new GInfoWindowTab(\"{$line['head']}\",\"{$line['text']}\"),\n";
        }
        $ret .= "];\n\n";

        $ret .= "var {$this->name} = new GMarker(new GLatLng({$this->lat}, {$this->long})";
        if ($this->icon != '') $ret .= ', '.$this->icon;
        $ret .= ");\n\n";

        $ret .= "GEvent.addListener({$this->name}, \"click\", function() {\n";
        $ret .= "\t{$this->name}.openInfoWindowTabsHtml(tabs{$this->name});\n";
        $ret .= "});\n";

        $ret .= "map.addOverlay({$this->name});";
        return $ret;
    }
}
// }}}

$markers = array();

$sql = $db->fetchAll("SELECT * FROM user_profile JOIN users USING(uid) ORDER BY city");

$olc = '';
foreach ($sql as $key => $line) {
    if($line['city']>'') {
        $city = str_replace(array('å','ä','ö'),array('a','a','o'),strToLower($line['city']));
        if (!isset($markers[$city])) {
            $pos = unserialize($line['city_pos']);

            if (!isset($pos['lat'])&&!isset($pos['ignore'])) {
                if ($pos = getLatLongOnCity($line['city'])) {
                    echo "Sökte upp ny stad: {$line['city']}<br>";
                    flush();
                    ob_flush();
                    $db->query("UPDATE user_profile SET city_pos='".serialize($pos)."' WHERE city LIKE '".$line['city']."'");
                } else {
                    $db->query("UPDATE user_profile SET city_pos='".serialize(array('ignore'=>1))."' WHERE city LIKE '".$line['city']."'");
                }  
            }
            if (isset($pos['lat'])) {
                $markers[$city] = new marker ('t'.$line['uid'],$pos);
                $markers[$city]->icon = 'icon';
                $markers[$city]->addTab($line['username'],'<img src=\'http://honeydew.gayhyllan.se/images/users/thumbs/small_'.$line['picture'].'.jpg\' style=\'float:left;margin-right:5px;height:80px;width:60px;\'><b>'.$line['username'].'</b><br>Här bor '.$line['firstname'].' '.$line['lastname'].'<div style=\'clear:both;\'></div>'); 
            }
        } else {
                $markers[$city]->addTab($line['username'],'<img src=\'http://honeydew.gayhyllan.se/images/users/thumbs/small_'.$line['picture'].'.jpg\' style=\'float:left;margin-right:5px;height:80px;width:60px;\'><b>'.$line['username'].'</b><br>Här bor '.$line['firstname'].' '.$line['lastname'].'<div style=\'clear:both;\'></div>'); 
        }
        $olc = $city;
    }
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
    <script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAHKcUEcxldZ6zDf9XPO2ITxQLCBT4aedkkz6_g9HMfD_z8d-V4hS2aYeWd9CPzztP-OWDKoBZLwWWKw"
      type="text/javascript"></script>
    <script type="text/javascript">

    function load() {
      if (GBrowserIsCompatible()) {
        var map = new GMap2(document.getElementById("map"));
        map.addControl(new GSmallMapControl());
        map.addControl(new GMapTypeControl());
        map.setCenter(new GLatLng(62.895217544882044,15.732421875), 5);

        var icon = new GIcon();
        icon.image = "http://labs.google.com/ridefinder/images/mm_20_red.png";
        icon.shadow = "http://labs.google.com/ridefinder/images/mm_20_shadow.png";
        icon.iconSize = new GSize(12, 20);
        icon.shadowSize = new GSize(22, 20);
        icon.iconAnchor = new GPoint(6, 20);
        icon.infoWindowAnchor = new GPoint(5, 1);

        <?
        
        foreach ($markers as $marker) {
            echo $marker->generate();
        }
        
        ?>
      }
    }

    </script>
  </head>
  <body onload="load()" onunload="GUnload()">
    <div id="map" style="width: 350px; height: 700px"></div>
  </body>
</html>
<?php

get();

?>
