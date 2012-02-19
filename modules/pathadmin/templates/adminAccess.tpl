<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="se">

<head>
    <title>Ändra rättigheter</title>
    <script type="text/javascript" src="/modules/aculo/lib/prototype.js"></script>
    <script type="text/javascript" src="/modules/aculo/src/scriptaculous.js"></script>

    <style>
        * {
            margin:0px;
            padding:0px;
            font-family:arial;
        }
        
        a {
            cursor:pointer;
        }
        
        table {
            width:100%;
            color:#000;
        }
        
        #selbox.disabled {
            color:#999;
        }
        
        table th {
            text-align:left;
            font-size:12px;
            border-bottom: 1px solid #999;
            border-right: 1px solid #999;
            padding: 2px 3px; 
        }

        table td {
            border-bottom: 1px solid #999;
            padding: 2px 3px;
            cursor:pointer;
            font-size:11px;
        }
        
        table tr div.img {
            width:11px;
            height:11px;
            background:url('/modules/pathadmin/templates/images/notselected.gif') no-repeat;
        }

        table tr.selected {
            background:#cfc;
        }
        
        table tr.selected div.img {
            background:url('/modules/pathadmin/templates/images/selected.gif') no-repeat;
        }
        
        input {
            width:auto;
            height:auto;
        }
    </style>
    <script language="javascript">
        
       
        
        function setClassName(obj,selected) {
            row = $('row_'+obj);
            
            if (selected) {
                row.addClassName('selected');
            } else {
                row.removeClassName('selected');
            }
        }
        
        function select(obj) {
            id =  obj.id.substr(4,obj.id.length);
            row = $('row_'+id);
            
            if (row.hasClassName('selected')) {
                setClassName(id,false);
                accesslist = accesslist.without(id);
            } else {
                setClassName(id,true);
                accesslist[accesslist.length] = id;
            }
        }
        
        function all(obj) {
            var objs = $('listtable').getElementsByClassName('row');

            if (obj.checked) {
                objs.each(
                    function (val) {
                        //val.checked = true;
                        $('sel_'+obj.id.substr(4,obj.id.length)).src = selected
                        select(val);
                    }
                )
            } else {
                objs.each(
                    function (val) {
                        $('sel_'+obj.id.substr(4,obj.id.length)).src = notselected;
                        select(val);
                    }
                )
            }
        }
        var timerID;

        function keyp() {
            if (timerID)
                clearTimeout(timerID);
            timerID = setTimeout("search($('searchInp').value)",1000);    
        }

        function getSelected() {
            search('!'+accesslist.join());
        }

        function search(value) {
            Element.show('dosearch');
            $('searchInp').disabled=true;
            
            new Ajax.Updater(
                'searchBox', 
                '/modules/pathadmin/helper.php?path=<?php echo $this->cleanpath; ?>&search='+value, 
                {
                    asynchronous:true,
                    onComplete: function() {
                        searchFinished();
                    }
                }
            );
        }

        function searchFinished() {
            Effect.Fade('dosearch');
            $('searchInp').disabled=false; 
            $('searchInp').focus();
            doSelects();
        }

        function doSelects() {
        
            var objs = $('listtable').getElementsByClassName('row');

            list = accesslist.without('');
            objs.each(
                function (val) {
                    for (i=0;i<list.length;i++) {
                        if(val.hasClassName(list[i])) {
                            id =  val.id.substr(4,val.id.length);
                            setClassName(id,true);
                        }
                    }
                }
            );
            
        }
        
        function savelist() {
            accesslist = accesslist.without('');
            savelist = accesslist.join(',');
            if (savelist.substr(savelist.length-1,1)!=',')
                savelist = savelist + ',';
                
            return savelist;
        }

        function save() {
            accesslist = accesslist.without('');
            savelist = accesslist.join(',');
            if (savelist.substr(savelist.length-1,1)!=',')
                savelist = savelist + ',';
        
            if(self.opener.document.getElementById(field).value = savelist) {
                self.window.close();
                return false;
            }
            alert('Misslyckades att spara');
        }
        
        function load(from) {
            from = from.replace(',,',',');
            
            accesslist = from.split(',');

            getSelected();
        }

        function showselected() {
            load(accesslist.join());
        }
        
        function showall() {
            search('');
        }

        var accesslist = [];
        var field = '<?php echo $this->field; ?>';
        
        notselected = '/modules/pathadmin/templates/images/notselected.gif';
        selected = '/modules/pathadmin/templates/images/selected.gif';
    </script>
</head>
<body onLoad="load(self.opener.document.getElementById(field).value);" onUnload="if(!(self.opener.document.getElementById(field).value==savelist())&&confirm('Vill du spara ändringar?')) save();">
    <div style="margin:auto;width:500px;background:#FFF;">
        <div style="background:#eee;border-bottom:1px solid #999;height:50px;font-size:11px;position:relative;">
            <div style="padding-left:20px;padding-top:13px;" id="selbox">
                Sök <input style="padding:2px;margin-top:auto;font-size:11px;margin-bottom:auto;display:inline;border:1px solid #999;" name="search" onKeyPress="keyp();" id="searchInp"> 
                [ <a onClick="showselected();return false;" href="">Visa valda</a> ] 
                [ <a onClick="showall();return false;" href="">Visa alla</a> ]
                [ <a onClick="save();return false;" href="">Spara</a> ]
            </div>
            <div id="dosearch" style="display:none;position:absolute;top:0px;left:0px;width:500px;background:#fcc;padding-left:20px;padding-top:17px;padding-bottom:19px;">
                Söker....
            </div>
        </div>
        <table  border=0 cellpadding=0 cellspacing=0>
            <tr>
                <th width=14><img src="/modules/pathadmin/templates/images/notselected.gif" border=0 onClick="all(this);"></th>
                <th width=50>Type</th>
                <th width=290>Name</th>
                <th style="border-right:0;">Event</th>
            </tr>
        </table>
        <div style="width:500px;height:230px;overflow:auto;" id="searchBox">
            <?php $this->display('table.tpl'); ?>
        </style>
    </div>
