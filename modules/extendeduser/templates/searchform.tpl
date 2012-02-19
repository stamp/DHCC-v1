<script language="javascript">
function showAdd(obj,url,upd)
	        {
                new Effect.Opacity(upd,{
                 duration: 0.2, Tansition: Effect.Transitions.linear, from: 1.0, to: 0.0,
                 afterFinish: function (effect) {
            new Ajax.Updater(upd, url, 
            {
                onComplete:function(request)
                {
                    if(document.getElementById(upd).style.display=='none')
                        Effect.BlindDown(upd,{duration:0.3})
                }, 
        parameters:Form.serialize(obj.id),evalScripts:true, asynchronous:true
            }
            )
                             new Effect.Opacity(upd,{
                       duration: 0.3, Tansition: Effect.Transitions.linear, from: 0.0, to: 1.0 });
                                }
                            })

            }
</script>
<?php
echo '<div style="padding:10px;">';
$f = new form('','','','searchform');
echo $f->start('',array('onsubmit'=>"showAdd(this,'/fetch.php?path=".$this->cleanpath."&ajaxsearch=1','result');return false;"));
echo $f->text('find','Sök:',array('id'=>'autofind'));
echo $f->checkbox('this','Sök bara i nuvarande event');
?>
<div id="list" class="list" style="display:none;z-index:10"></div>
<script type="text/javascript">
new Ajax.Autocompleter('autofind','list','/fetch.php?path=/search&ajax=1', {
paramName: "value"})
</script>
<?php
echo '<script language="javascript">document.getElementById(\'searchform\').find.focus()</script>';
echo $f->clear();
echo $f->submit('Sök:');
echo $f->stop();
echo '</div>';
?>
<div id="result" style="display:none;"></div>
