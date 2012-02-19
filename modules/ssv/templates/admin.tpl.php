<?php
// Storlek i meter
$sizeX = 60;
$sizeY = 21;

// skala, en meter är så här många pixlar:
$scalex = 15;
$scaley = 15;
?>
<span id="tooltip" style="position:absolute; visibility:hidden;z-index:20"></span>
<script language="Javascript">
var pdata = new Array(<?php echo $sizeX; ?>)
var cfloor = '#444';
var cwall = '#000';
var cdubble = '#448';
var cbed = '#484';
for(n=0; n<pdata.length; n++){
    pdata[n] = new Array(<?php echo $sizeY; ?>)
}

<?php

$types = array();

foreach ($this->ssvdata as $k1 => $l) {
    echo "pdata[{$l['x']}][{$l['y']}] = '{$l['type']}';\n";

    if (!isset($types[$l['x']]))
        $types[$l['x']] = array();

    $types[$l['x']][$l['y']] = $l['type'];
}

?>



function din(id) {
    id.style.border='1px solid #777';
}

function dut(id) {
    id.style.border='1px solid #444';
}

function dclick(id,x,y) {
    for (n=0; n<document.cform.smode.length; n++){
        if (document.cform.smode[n].checked){
            mode = document.cform.smode[n].value;
        }
    }
    if (mode == "floor") {
        id.style.background=cfloor;
        pdata[x][y] = 'floor';
    } else if (mode == "wall") {
        id.style.background=cwall;
        pdata[x][y] = 'wall';
    } else if (mode == "bed") {
        id.style.background=cbed;
        pdata[x][y] = 'bed';
    }
}

function fixa() {
    tx = '';
    for(n=0; n<pdata.length; n++){
        for(m=0; m<pdata[n].length; m++){
            if (pdata[n][m] != 'floor')
               tx = tx + ""+n+";"+m+";"+pdata[n][m]+"|"
        }
    }
    document.sform.text.value = tx;
    document.sform.send();
}

var Xmouse, Ymouse;
var hidden = true;
var preMessage="<div style='background-color: #777;border:solid #333 1px;padding:5px;font-family:Arial;font-size:12px;'>";
var postMessage="</div>";
function tooltip(message) {
    if(message){
        hidden = false;
        if (document.layers){
            with (document["tooltip"].document){
                open();
                write(preMessage + message + postMessage);
                close();
            }
        } else if (document.all) {
            document.all["tooltip"].innerHTML = preMessage + message + postMessage;
        } else if (document.getElementById){
            document.getElementById("tooltip").innerHTML = preMessage + message + postMessage;
        }
        moveLayer("tooltip",Xmouse-30,Ymouse+18);
        if (document.all) {
            document.all["tooltip"].style.visibility = "visible";
        } else if (document.layers){
            document.layers["tooltip"].visibility = "show";
        } else if (document.getElementById){
            document.getElementById("tooltip").style.visibility = "visible";
        }
    } else {
        hidden = true;
        if (document.all) {
            document.all["tooltip"].style.visibility = "hidden";
        } else if (document.layers){
            document.layers["tooltip"].visibility = "hide";
        } else if (document.getElementById){
            document.getElementById("tooltip").style.visibility = "hidden";
        }
        moveLayer("tooltip",0,0);
    }
}
function MoveHandler(evnt) {
    if(document.all) {
        Xmouse = window.event.x + document.body.scrollLeft;
        Ymouse = window.event.y + document.body.scrollTop;
    } else if(document.layers||document.getElementById){
        Xmouse = evnt.pageX;
        Ymouse = evnt.pageY;
    }
    if(!hidden){ moveLayer("tooltip",Xmouse-30,Ymouse+18); }
}
function moveLayer(Id,x,y){
    if (document.all){
        document.all[Id].style.left = x;
        document.all[Id].style.top = y;
    } else if (document.layers){
        document.layers[Id].left = x;
        document.layers[Id].top = y;
    } else if (document.getElementById){
        document.getElementById(Id).style.left = x+'px';
        document.getElementById(Id).style.top = y+'px';
    }
}
if (document.layers){
    document.captureEvents(Event.MOUSEMOVE);
}
document.onmousemove = MoveHandler;
</script>

<div style="position:absolute;left:100px;width:<?php echo $sizeX*$scalex+2; ?>px;z-index:10;">
<?php
for($y=0;$y<($sizeY*$scaley);$y=$y+$scaley) {
    for($x=0;$x<($sizeX*$scalex);$x=$x+$scalex) {
        switch (isset($types[$x/$scalex][$y/$scaley])?$types[$x/$scalex][$y/$scaley]:'') {
            case 'wall':
                $bg='#000';
                break;
            case 'bed':
                $bg='#484';
                break;
            default:
                $bg='#444';    
                break;
            }
        echo "\t\t<div id=\"".($y/$scaley).'-'.($x/$scalex)."\" style=\"height:".($scaley-2)."px;width:".($scalex-2)."px;background:$bg;border:1px solid #444;float:left;position:static\" ".
             "onMouseOver=\"din(this);\" onMouseOut=\"dut(this);\" onClick=\"dclick(this,".($x/$scalex).",".($y/$scaley).");\"></div>\n";
    }
    echo "\t<div style=\"clear:both;\"></div>\n";
}
?>

<div style="position:absolute;top:10px;left:<?php echo $sizeX*$scalex+110; ?>;border:1px solid #0f0;width:150px;">
<form name="cform">
<input type="radio" name="smode" value="floor" style="float:left;display:inline;width:auto;height:auto;">
Golv<br>
<input type="radio" name="smode" value="wall" checked style="float:left;display:inline;width:auto;height:auto;">
Vägg<br>
<input type="radio" name="smode" value="bed" style="float:left;display:inline;width:auto;height:auto;">
Enkelbädd<br>
</form>
</div>
<div style="position:absolute;left:100px;top:<?php echo $sizeY*$scaley+20; ?>px">
<form name="sform" method="post" onSubmit="fixa()">
<input type="hidden" style="display:none;" name="text" value="">
<input type="submit" value="Spara">
</form>
</div>
</div>
