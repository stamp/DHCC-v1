</div>
<script type="text/javascript">
function showhide(gid,button){
     if (document.getElementById(gid).style.display=='none'){
            new Effect.BlindDown(gid,{duration:0.3});
            new Element.update(button,'» Göm gamla');
        }
       else{
            new Effect.BlindUp(gid,{duration:0.3});
              new Element.update(button,'» Visa gamla');
          } return false;}
</script>

