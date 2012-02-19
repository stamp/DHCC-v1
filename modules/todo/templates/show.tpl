<style>

#todo.module {
    padding:10px;
}

#todo.module h1 {
    margin-bottom:5px;
}

#todo.module div.text {
    clear:both;
}

#todo.module div.posts {
    margin-top:10px;
    padding-left:10px;
}

#todo.module div.posts div {
}


#todo.module div.post {
    background: #000;
}


#todo.module div.desc {
   margin-left:75px;
   color:#aaa;
   font-size:10px;
}


#todo.module div.post div.process {
    position:relative;
    border: 1px solid #5F6B2E;
    width:  50px;
    height: 15px;
    float:left;
    margin-right:10px;
    overflow:hidden;
}

#todo.module div.post div.process div.meter {
    height: 15px;
    background: #5F6B2E;
    position:absolute;
    top:0;
}

#todo.module div.post div.process div.meter.pendling {
    background: #6B532E;
}


#todo.module div.post div.process div.meter.halted {
    background: #444;
}


#todo.module div.post div.process div.meter.aborted {
    background: #6B2E2E;
}

#todo.module div.post div.process.pendling {
    border-color: #6B532E;
}

#todo.module div.post div.process.halted {
    border-color: #444;
}

#todo.module div.post div.process.aborted {
    border-color: #6B2E2E;
}

#todo.module div.post div.process div.text {
    text-align:center;
    position:relative;
    left:0px;
    top:0px;
    font-size: 9px;
    padding-top:1px;
    z-index:10;
}

#todo.module div.addpost {
    background:#333;
    padding:5px;
    margin-bottom:10px;
}

#todo.module div.addpost input {
    width:200px;
    margin-bottom:5px;
    margin-top:2px;
    border:0;
    position:static;
}

#todo.module div.addpost textarea {
    font-size:10px;
    margin-top:2px;
    width:200px;
    padding:0px;
    border:0;
}

#todo.module div.addpost input[type="submit"] {
    border:0;
    width:200px;
    background:#fff;
    display:inline;

}

</style>
<?php
if ($this->write) {
?>
<script language="javascript">
    function load(obj) {
        id = obj.parentNode.id.substring(5, obj.parentNode.id.length );

        edit = document.getElementById('edit_'+id);

        if (edit.innerHTML != 'nisse') {
            Element.hide(obj.parentNode);
            Element.show('edit_'+id);
            new Ajax.Updater('edit_'+id,'/fetch.php?path=<?php echo $this->cleanpath; ?>&PG='+id);
        } else {
            Element.show('edit_'+id);
            Element.hide(obj.parentNode);
        }
    }
    
    function save(obj) {
        id = obj.parentNode.id.substring(5, obj.parentNode.id.length );
        obj.parentNode.parentNode.childNodes[1].innerHTML = 'Saving...';
        Element.show(obj.parentNode.parentNode.childNodes[1]);
        Element.hide(obj.parentNode);
        new Ajax.Updater('post_'+id,'/fetch.php?path=<?php echo $this->cleanpath; ?>&PG='+id,{asynchronous:true,parameters:Form.serialize(obj.id)});
        return false;
    }

    function add(obj) {
        id = obj.parentNode.id.substring(5, obj.parentNode.id.length );
        new Ajax.Request('/fetch.php?path=<?php echo $this->cleanpath; ?>&main='+id+'&PG=0',{
            onSuccess:function(t) {
                new Insertion.Before(obj,'<div>'+t.responseText+'</div>');
                obj.getElementsByTagName('input')[0].value='';
                obj.getElementsByTagName('textarea')[0].value='';
                obj.getElementsByTagName('input')[0].focus();
            }
        ,parameters:Form.serialize(obj.id)});
        return false;
    }

    function hideEdit(obj) {
        Element.show(obj.parentNode.parentNode.parentNode.childNodes[1]);
        Element.hide(obj.parentNode.parentNode);
    }
    
    function showAdd(obj) {
        Effect.Fade(obj);
        Effect.BlindDown(obj.parentNode.childNodes[0]);
    }

    function hideAdd(obj) {
        Effect.Appear(obj.parentNode.parentNode.parentNode.childNodes[1]);
        Effect.BlindUp(obj.parentNode.parentNode);
    }

    function showhide(obj,val) {
        var elementList = document.getElementsByClassName(obj,'todo');
        elementList.each(function(node){
            if(val) {
                Element.show(node.id);
            } else {
                Element.hide(node.id);
            }
        });
    }
</script>
<?php
}
?>

<div style="right:0px;position:absolute;width:140px;">
<h3>Show:</h3>
<?php
    $val = array('active', 'pending', 'halted', 'aborted', 'finished');
    foreach($val as $line) 
        echo '<label style="float:left;width:100px;" for="todo_'.$line.'">'.$line.'</label><input onChange="showhide(this.id,this.checked)" type="checkbox" checked="checked" style="float:left;" id="todo_'.$line.'">';
?>
</div>

<div style="float:left;">
<?php
if(isset($this->todo)&&is_array($this->todo))
    foreach ($this->todo as $line) {
        echo "<h1>{$line['head']}</h1>\n";

        echo "<div class=\"text\">".$line['text']."</div>\n";
        echo "<div class=\"posts\" id=\"main_{$line['id']}\">\n";
        $id=$line['id'];
        
        if (isset($line['posts'])&&is_array($line['posts']))
            foreach ($line['posts'] as $post) {
                include('fullpost.tpl');
            }
            
            if ($this->write)
                include('add.tpl');

        echo "</div>\n\n";
    }


?></div>
