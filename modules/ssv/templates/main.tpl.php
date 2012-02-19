<div style="background:#000; position:absolute;z-index:20;width:100%;height:100%;left:0;">
<?php
// Storlek i meter
$sizeX = read('ssv_x',20,'px');
$sizeY = read('ssv_y',20,'px');

// skala, en meter är så här många pixlar:
$scale = read('ssv_scale',15,'px/box');
?>
<div id="tooltip" style="position:absolute; display:none;z-index:20"></div>

<div id="box" style="position:absolute;left:0px;top:0px;width:400px;z-index:20;background:#111;border:1px solid #666;;color:#666;display:none;padding:5px;">
[ <a href="#" onClick="this.parentNode.style.display='none';return false;">stäng</a> ]
<div id="box_txt" style="clear:both;color:#fff;padding:5px;">
</div>
</div>
<script language="Javascript"  type="text/javascript">

var pdata = new Array(<?php echo $sizeX+1; ?>)

for(n=0; n<pdata.length; n++){
    pdata[n] = new Array(<?php echo $sizeY+1; ?>)
}

<?php
if (isset($this->ssvdata)&&is_array($this->ssvdata))
foreach ($this->ssvdata as $k1 => $l) {
    foreach ($l as $k2 => $g) 
    echo "pdata[{$k1}][{$k2}] = '{$g['type']}';\n";
}

?>

var cfloor = '<?php echo read('ssv_color_floor','#444','Hex color'); ?>';
var cwall = '<?php echo read('ssv_color_wall','#00','Hex color'); ?>';
var cbed = '<?php echo read('ssv_color_bed','#484','Hex color'); ?>';
var cfree = '<?php echo read('ssv_color_bed_free','#666','Hex color'); ?>';
var cwarn = '<?php echo read('ssv_color_bed_alarm','#A39C30','Hex color'); ?>';
var calarm = '<?php echo read('ssv_color_bed_warning','#A33030','Hex color'); ?>';
var csleep = '<?php echo read('ssv_color_bed_sleep','#5630a3','Hex color'); ?>';

var dontUpdate = false;

function updatetbl() {
   if( !dontUpdate ) {
      new Ajax.Updater(
         'upcomming',
         '/fetch.php?path=<?php echo $this->cleanpath; ?>&action=list',
         {
            onComplete:function(t){ 
               new Effect.Highlight('upcomming');
               setTimeout("updatetbl()",10000);
            },
            asynchronous:true, 
            evalScripts:true
         }
      );
   } else {
      setTimeout("updatetbl()",10000);
   }
}

var ax,ay;

function din(id,x,y) {
    box = document.getElementById('box');
    if ((box.style.display=="none")&&!(pdata[x][y]=='floor'||pdata[x][y]=='wall')) {
        ax = x;
        ay = y;
        tooltip("<b>Plats "+y+"-"+x+"</b>");
        id.style.border='1px solid #777';
        timer = setTimeout("updateTooltip(ax,ay)",200);
    }
}

function dut(id) {
    id.style.border='1px solid #444';
    tooltip();
    clearTimeout(timer);
}

function updateTooltip(x,y) {
    if ($('box').style.display=="none") {
      tooltip("<b>Plats "+y+"-"+x+"</b><br><span id=txt></span>");
      

		new Ajax.Updater(
			'txt',
			'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=simple&plats='+x+'-'+y,
			{
				asynchronous:true, 
				evalScripts:true,
			}
		);	
    }
}


function dclick(id,x,y) {
    box = document.getElementById('box');
    if (box.style.display=="none"&&!(pdata[x][y]=='floor'||pdata[x][y]=='wall')) {
        box.style.display="block";

      $('box_txt').innerHTML = 'Laddar...';

		new Ajax.Updater(
			'box_txt',
			'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=advanced&plats='+x+'-'+y,
			{
				asynchronous:true, 
				evalScripts:true,
			}
		);	
    }
}

function wake(finalwake, id, link) {
      link.disabled = true;
      link.style.color = '#ccc';

		new Ajax.Updater(
         'upcomming',
			'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=wake',
			{
            method: 'post',
            postBody: 'wake='+id+'&final='+finalwake,
				asynchronous:false, 
				evalScripts:true,
			}
		);	
}

function bokatid(form,x,y) {
      $('box_txt').innerHTML = 'Sparar...';

		new Ajax.Updater(
			'box_txt',
			'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=advanced&plats='+x+'-'+y,
			{
            method: 'post',
            postBody: Form.serialize(form),
				asynchronous:true, 
				evalScripts:true,
			}
		);	
}

function obokatid(id,x,y) {
      $('box_txt').innerHTML = 'Sparar...';

		new Ajax.Updater(
			'box_txt',
			'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=advanced&plats='+x+'-'+y,
			{
            method: 'post',
            postBody: 'booking='+id,
				asynchronous:true, 
				evalScripts:true,
			}
		);	
}

function upd(x,y,data) {
	new Ajax.Updater(
		'box_txt',
		'/fetch.php?path=<?php echo $this->cleanpath; ?>&action=book&uid='+data+'&plats='+x+'-'+y,
		{
			asynchronous:true, 
			evalScripts:true,
		}
	);
}

function fixa() {
    tx = '';
    for(n=0; n<pdata.length; n++){
        for(m=0; m<pdata[n].length; m++){
            tx = tx + ""+n+";"+m+";"+pdata[n][m]+"|"
        }
    }
    document.sform.text.value = tx;
    document.sform.send();
}

var searchdata = '';
var ttactive = false;
var selIndex = new Array(2);

function tooltip(message) {
	var preMessage="<div style='background-color: #111;width:400px;border:solid #333 1px;padding:5px;font-family:Arial;font-size:12px;'>";
	var postMessage="</div>";
   
    if(message){
      if (!ttactive) {
         searchdata = $('search').innerHTML;
         selIndex[0] = $('teamlist').selectedIndex;
         selIndex[1] = $('person').selectedIndex;
         ttactive = true;
      }

      $('search').innerHTML = message;

        //$("tooltip").innerHTML = preMessage + message + postMessage;
        //moveLayer("tooltip", Event.pointerX -30, Event.pointerY +18);
		//$("tooltip").style.display = 'block';
    } else {
      $('search').innerHTML = searchdata;
      $('teamlist').selectedIndex = selIndex[0];
      $('person').selectedIndex = selIndex[1];
      ttactive = false;

		//$("tooltip").style.display = 'none';
      //  moveLayer("tooltip",0,0);
    }
}

function moveLayer(Id,x,y){
	$(Id).style.left = x + 'px';
 	$(Id).style.top = y + 'px';
}
</script>

<div style="position:absolute;left:100px;width:<?php echo $sizeX*$scale+2; ?>px;z-index:10;">
<?php
for($y=0;$y<($sizeY*$scale);$y=$y+$scale) {
    for($x=0;$x<($sizeX*$scale);$x=$x+$scale) {
        $bg='#444';
       
        echo "\t\t<div name=\"".(isset($this->data[$x/$scale][$y/$scale]['user'])?$this->data[$x/$scale][$y/$scale]['user']:'')."\" id=\"".($x/$scale).'-'.($y/$scale)."\" style=\"height:".($scale-2)."px;width:".($scale-2)."px;background-color:$bg;border:1px solid #444;float:left;position:static\" ".
             "onMouseOver=\"din(this,".($x/$scale).','.($y/$scale).");\" onMouseOut=\"dut(this);\" onClick=\"dclick(this,".($x/$scale).",".($y/$scale).");\"></div>\n";
    }
    echo "\t<div style=\"clear:both;\"></div>\n";
}
?>

<div id="upcomming" onMouseOver="this.style.border = '3px solid #F00;';dontUpdate = true;" onMouseOut="this.style.border = '3px solid #444;';dontUpdate = false;" style="height:auto;width:400px;background-color:#444;float:left;margin-right:20px;margin-top:20px;padding:5px;border:3px solid #444;">&nbsp;</div>
<div id="search" style="height:auto;width:400px;background-color:#444;float:left;margin-top:20px;padding:5px;">

<?php
echo '<b>Hantera användare</b><form name="f" method="post" action="?pg=">';
        echo '<input type="text" name="team" value="" style="display:none;">';

            echo '<select size="25" name="teamlist" id="teamlist" style="widht:150px;height:auto;float:left;" onChange="changemail(this.options[this.selectedIndex].value,0);">';	
            if (isset($this->teams))
	    foreach ($this->teams as $key => $line) {
                echo '<option value="'.$line['gid'].'">'.$line['name'].'</option>';
            }
            echo '</select>'."\n";
            
            echo '</select>';
            echo '<select size=25 style="width:240px;height:auto;float:left;" name="person" id="person" onClick="changeuser(this.options[this.selectedIndex].value,0);">';
            echo '<option value="0">-------- Välj team först! --------</option>';
            echo '</select></form>';
?>
</div>

        <script language="javascript">
            <?
	    
	    if (isset($this->teams))
            foreach ($this->teams as $key => $line) {
                echo "I{$line['gid']} = new Array(";
                $a = 0;
                if ($line>'')
                foreach ($this->box[$line['gid']] as $key2 => $line2) {
                    if ($a==1) echo ',';
                    echo "'{$line2['username']}'";
                    $a = 1;
                }
                echo ');'."\n";
                
                echo "C{$line['gid']} = new Array(";
                $a = 0;
                if ($line>'')
                foreach ($this->box[$line['gid']] as $key2 => $line2) {
                    if ($a==1) echo ',';
                    echo "'{$line2['uid']}'";
                    $a = 1;
                }
                echo ');'."\n\n";
            }
            
            ?>
          
          var selGroupName;
          function changemail(Grupp,Mottagare) {
            selIndex[0] = $('teamlist').selectedIndex;
            selIndex[1] = 0;
           if (Grupp > 0) {
            var arrC = eval('C' + Grupp);
            var arrI = eval('I' + Grupp);
            document.f.team.value = document.f.teamlist.options[document.f.teamlist.options.selectedIndex].text;
            document.f.person.options.length = 0;

            selidx = 0;
            for(i = 0; i < arrC.length; i++) {
             if (arrC[i] == Mottagare) { selidx = i + 1; }
             document.f.person.options[i] = new Option(arrI[i],arrC[i]);
            }

            document.f.person.options.selectedIndex = selidx;
           }
           else if (Grupp == 0) {
            document.f.person.options.length = 0;
           }
          }
          var oldobj;
          function changeuser(user,act) {
            selIndex[0] = $('teamlist').selectedIndex;
            selIndex[1] = $('person').selectedIndex;
            user = 'U' + user
            if (oldobj) oldobj.style.border = "1px solid #555";
            if (obj = document.getElementsByClassName(user)[0]) {
                obj.style.border = "1px solid #FF0";
                if (oldobj == document.getElementsByClassName(user)[0]) {
                    if (oldobj) oldobj.style.border = "1px solid #555";
                    oldobj = null;
                    if (obj = document.getElementsByClassName(user)[0].id){
                        data = explodeArray(obj,'-');
                        dclick('as',data[0],data[1]);
                    }
                } else {
                    oldobj = obj;
                }
            } else {
                oldobj = null;
            }            
          }

            function showuserp(user) {
                 if (obj = document.getElementsByName(user)[0].id){
                    data = explodeArray(obj,'-');
                    dclick('as',data[0],data[1]);
                }
            }

            function explodeArray(item,delimiter) {
              tempArray=new Array(1);
              var Count=0;
              var tempString=new String(item);

              while (tempString.indexOf(delimiter)>0) {
                tempArray[Count]=tempString.substr(0,tempString.indexOf(delimiter));
                tempString=tempString.substr(tempString.indexOf(delimiter)+1,tempString.length-tempString.indexOf(delimiter)+1); 
                Count=Count+1
              }

              tempArray[Count]=tempString;
              return tempArray;
            }
        
          
        setTimeout("updatetbl()",500);
        </script>

<div style="clear:both;"></div>
</div>
