<SCRIPT LANGUAGE="Javascript">

function Browser()
{
 this.isIE    = false;
 this.isNS    = false;
 var ua = navigator.userAgent;
 if (ua.indexOf('MSIE') >= 0) 
  this.isIE = true;
 if (ua.indexOf('Netscape6/') >= 0)
  this.isNS = true;
 if (ua.indexOf('Gecko') >= 0)
  this.isNS = true;
}

var browser = new Browser();
var dragObj = new Object();
dragObj.OffsetX = 0;
dragObj.OffsetY = 0;
dragObj.Ratio = 4/3;

function dragStart(event,id,resizing)
{
 //alert(resizing);
 dragObj.elNode = document.getElementById(id);
 dragObj.elNode2 = document.getElementById(id+'2');
 dragObj.elImage = document.getElementById('image');
 if (browser.isNS) {
  dragObj.cursorStartX = event.clientX + window.scrollX;
  dragObj.cursorStartY = event.clientY + window.scrollY;
 }
 else {
  dragObj.cursorStartX = window.event.clientX + document.documentElement.scrollLeft + document.body.scrollLeft;
  dragObj.cursorStartY = window.event.clientY + document.documentElement.scrollTop + document.body.scrollTop;
 }
 dragObj.elStartLeft = parseInt(dragObj.elNode.style.left, 10);
 dragObj.elStartTop = parseInt(dragObj.elNode.style.top,  10);
 dragObj.elStartWidth = parseInt(dragObj.elNode.style.width, 10);
 dragObj.elStartHeight = parseInt(dragObj.elNode.style.height, 10);
 dragObj.resizing = resizing;
 
 //if(dragObj.cursorStartX > dragObj.elStartLeft && dragObj.cursorStartX < dragObj.elStartLeft + dragObj.elStartWidth && dragObj.cursorStartY > dragObj.elStartTop && dragObj.cursorStartY < dragObj.elStartTop + dragObj.elStartWidth * dragObj.Ratio)
 //{
  document.getElementById('image').style.cursor = 'move';
  if (browser.isNS) {
   document.addEventListener("mousemove", dragGo,   true);
   document.addEventListener("mouseup",   dragStop, true);
   event.preventDefault();
  }
  else {
   document.attachEvent("onmousemove", dragGo);
   document.attachEvent("onmouseup",   dragStop);
   window.event.cancelBubble = true;
   window.event.returnValue = false;
  }
 //}
}

function dragGo(event)
{
 var x, y;
 if (browser.isNS) {
  x = event.clientX + window.scrollX;
  y = event.clientY + window.scrollY;
 }
 else {
  x = window.event.clientX + document.documentElement.scrollLeft + document.body.scrollLeft;
  y = window.event.clientY + document.documentElement.scrollTop + document.body.scrollTop;
 }

 if(dragObj.resizing == true) {
  if (!browser.isNS) {
    x = Math.max(16,Math.min(dragObj.elStartWidth + x - dragObj.cursorStartX, dragObj.elImage.width + dragObj.OffsetX + 4 - dragObj.elStartLeft));
  } else {
    x = Math.max(16,Math.min(dragObj.elStartWidth + x - dragObj.cursorStartX, dragObj.elImage.width + dragObj.OffsetX - 2 - dragObj.elStartLeft));
  }
  y = x * dragObj.Ratio;
  
  if (!browser.isNS) {
   if(y >= dragObj.elImage.height + dragObj.OffsetY + 6 - dragObj.elStartTop) {
    y = dragObj.elImage.height + dragObj.OffsetY + 0 - dragObj.elStartTop;
    x = y / dragObj.Ratio;
   }
  } else {
   if(y >= dragObj.elImage.height + dragObj.OffsetY - 2 - dragObj.elStartTop) {
    y = dragObj.elImage.height + dragObj.OffsetY - 2 - dragObj.elStartTop;
    x = y / dragObj.Ratio;
   }
  }

  dragObj.elNode.style.width = x;
  dragObj.elNode.style.height = y;
  document.getElementById('slid').style.top = y - 14;
  document.getElementById('slid').style.left = x - 14;
  dragObj.elNode2.style.width = x-2;
  dragObj.elNode2.style.height = y-2;

 }
 else {
  if (!browser.isNS) {
   x = Math.min(Math.max((dragObj.elStartLeft + x - dragObj.cursorStartX),dragObj.OffsetX), dragObj.elImage.width + dragObj.OffsetX + 6 - dragObj.elStartWidth);
   y = Math.min(Math.max((dragObj.elStartTop  + y - dragObj.cursorStartY),dragObj.OffsetY), dragObj.elImage.height + dragObj.OffsetY + 0 - dragObj.elStartHeight);
  } else {
   x = Math.min(Math.max((dragObj.elStartLeft + x - dragObj.cursorStartX),dragObj.OffsetX), dragObj.elImage.width + dragObj.OffsetX - 2 - dragObj.elStartWidth);
   y = Math.min(Math.max((dragObj.elStartTop  + y - dragObj.cursorStartY),dragObj.OffsetY), dragObj.elImage.height + dragObj.OffsetY - 2 - dragObj.elStartHeight);
  }
  dragObj.elNode.style.left = x + "px";
  dragObj.elNode.style.top  = y + "px";
 }
 
 if (browser.isNS) {
  event.preventDefault();
 } else {
  window.event.cancelBubble = true;
  window.event.returnValue = false;
 }
}

function dragStop(event) {
  document.getElementById('image').style.cursor = 'default';
 if (browser.isNS) {
  document.removeEventListener("mousemove", dragGo,   true);
  document.removeEventListener("mouseup",   dragStop, true);
 }
 else {
  document.detachEvent("onmousemove", dragGo);
  document.detachEvent("onmouseup",   dragStop);
 }
}

function mark() {
 var ref = document.getElementById('mark');
 var ref2 = document.markform;
 ref2.coordX.value = parseInt(ref.style.left, 10) - dragObj.OffsetX;
 ref2.coordY.value = parseInt(ref.style.top,  10) - dragObj.OffsetY;
 ref2.coordW.value = parseInt(ref.style.width, 10)+2;
 ref2.coordH.value = parseInt(ref.style.height, 10)+2;
 ref2.submit();
}
</SCRIPT>
<div style="padding:10px;">
<h1>Beskär bilden</h1><i>Beskär bilden genom att flytta och ändra storlek på rutan som är i bilden. Det som finns innuti rutan kommer att bli miniatyren på din pressentation.</i><br><br>
<FORM NAME="markform" ACTION="picture.htm" METHOD="post">
<INPUT class="hidden" TYPE="hidden" NAME="coordX" VALUE="">

<INPUT class="hidden" TYPE="hidden" NAME="coordY" VALUE="">
<INPUT class="hidden" TYPE="hidden" NAME="coordW" VALUE="">
<INPUT class="hidden" TYPE="hidden" NAME="coordH" VALUE="">
<INPUT class="hidden" TYPE="hidden" NAME="src" VALUE="<?php echo $this->src; ?>">
<INPUT class="hidden" TYPE="hidden" NAME="img" VALUE="<?php echo $this->img; ?>">
<INPUT class="hidden" TYPE="hidden" NAME="target" VALUE="<?php echo $this->target; ?>">
<div style="border:1px solid #000;width:400px;padding:0px;position:relative;">
    <img id="image" style="float:left;width:400px;" src="<?php echo $this->img; ?>" width="400" onmousedown="dragStart(event,'mark');" class="border">
    
    <div id="mark" style="position:absolute;border:1px black solid;left:10;top:10;width:150px;height:199px">
     <div id="mark2" style="border:1px white dashed;width:148px;height:197px;cursor:move;" onmousedown="dragStart(event,'mark');">
      <div id="slid" style="position:absolute;text-align:right;left:136;top:185;" onmousedown="dragStart(event,'mark',true);event.cancelBubble = true;" onmouseover="this.style.cursor = 'se-resize';">
        <img src="/images/system/set_photo_resize.gif" width=10 height=10>

      </div>
     </div>
    </div>
<div style="clear:both;"></div>
</div>
<input type="button" style="margin-top:5px;border-left:0px;" onClick="mark()" value="Beskär och spara">
</FORM>
</div>
