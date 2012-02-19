<?php

class logistik extends module {

    function __construct () {
        $this->setId(get_class($this));
        $this->setName('Logistik');
        $this->setDescription('All logistics functions');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setMaintainer('Jonas Falck <jonaz@jonaz.net>');
    }

    function install () 
    {
       
        $tables = array (
            'logistik_order' => array(
                array(
                    'Field'   => 'oid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'event',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'gid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'cnt',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'url',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'store',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'responsible',
                    'Type'    => 'varchar(255)',
                ),
                array(
                    'Field'   => 'timestamp',
                    'Type'    => 'timestamp',
                ),
                array(
                    'Field'   => 'status',
                    'Type'    => "enum('new','purchased','denied')",
                    'Default' => 'new'
                )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }

    function _show(){
        $this->show();
    }
    function _showall(){
        $this->show(1);
    }
    
    function show($admin=0) {
        global $path,$tpl;
        if(isset($_GET['editorder'])){
            if($_POST->is_set('text')){
                $name = $this->escapeStr($_POST['text']);
                if($this->query("UPDATE logistik_order SET text='".$name."' WHERE oid='".$this->escapeStr($_POST['id'])."'"))
                echo $name;
            }
            if($_POST->is_set('cnt')){
                $desc = $this->escapeStr($_POST['cnt']);
                if($this->query("UPDATE logistik_order SET `cnt`='".$desc."' WHERE oid='".$this->escapeStr($_POST['id'])."'"))
                    echo $desc;
            }
            if($_POST->is_set('url')){
                $desc = $this->escapeStr($_POST['url']);
                if($this->query("UPDATE logistik_order SET `url`='".$desc."' WHERE oid='".$this->escapeStr($_POST['id'])."'"))
                    echo $desc;
            }
            if($_POST->is_set('store')){
                $desc = $this->escapeStr($_POST['store']);
                if($this->query("UPDATE logistik_order SET `store`='".$desc."' WHERE oid='".$this->escapeStr($_POST['id'])."'"))
                    echo $desc;
            }
            if($_POST->is_set('responsible')){
                $desc = $this->escapeStr($_POST['responsible']);
                if($this->query("UPDATE logistik_order SET `responsible`='".$desc."' WHERE oid='".$this->escapeStr($_POST['id'])."'"))
                    echo $desc;
            }

        }
        else{


        $v = new validate($_POST);
            if($_POST->is_set('text')){
                $v->length('text',1,255);
            }


        if(!$v->ExistErrors() && $_POST->is_set('text')) {
            foreach($_POST AS $key=>$val){
               if($val==''){
                   $_POST[$key]="Editme";
               }

            }
                       $this->insert(
                array(
                    'text'=>$_POST->sql('text'),
                    'cnt'=>$_POST->sql('cnt'),
                    'store'=>$_POST->sql('store'),
                    'url'=>$_POST->sql('url'),
                    'gid'=>$path->vars['team'],
                    'uid'=>$_SESSION['id'],
                    'responsible'=>'Editme',
                    'event'=>$_SESSION['event']
                ),
                'logistik_order'
            );
            send(E_USER_NOTICE,'Beställning inlagd');
        }
        $tpl->assign('errors',$v->getAll());
        $tpl->assign('vals',$_POST);
        if(isset($_GET['remove']) && is_numeric($_GET['remove'])){
            $id = $this->escapeStr($_GET['remove']);
            $job = $this->fetchOne("SELECT text FROM logistik_order WHERE oid=".$id."");
            if($this->query("DELETE FROM logistik_order WHERE oid='".$id."' LIMIT 1"))
                send(E_USER_NOTICE,$job.' borttagen');
        }
        if(isset($_GET['denied']) && is_numeric($_GET['denied'])){
            $id = $this->escapeStr($_GET['denied']);
            $job = $this->fetchOne("SELECT text FROM logistik_order WHERE oid=".$id."");
            if($this->query("UPDATE logistik_order SET status='denied' WHERE oid='".$id."' LIMIT 1"))
                send(E_USER_NOTICE,$job.' Nekad');
        }
        if(isset($_GET['purchased']) && is_numeric($_GET['purchased'])){
            $id = $this->escapeStr($_GET['purchased']);
            $job = $this->fetchOne("SELECT text FROM logistik_order WHERE oid=".$id."");
            if($this->query("UPDATE logistik_order SET status='purchased' WHERE oid='".$id."' LIMIT 1"))
                send(E_USER_NOTICE,$job.' Köpt');
        }
        echo "<h2>Beställ prylar</h2>";
        $table = core::load('tables');
        if($admin == 1){

            $show= "WHERE structure.event='".$_SESSION['event']."'";
            $team="name AS Team,";
            $more="CONCAT('<a id=\"add\" href=\"?denied=',logistik_order.oid,'\" onClick=\"if(!confirm(\'Säker på att du vill NEKA ',logistik_order.text,' ?\')) return false;\">» Neka</a>') AS 'Neka',
                    CONCAT('<a id=\"add\" href=\"?purchased=',logistik_order.oid,'\" onClick=\"if(!confirm(\'Säker på att du vill ta Köpa in ',logistik_order.text,' ?\')) return false;\">» Köpt</a>') AS 'Köpt' ,
            CONCAT('<p id=\"r-',oid,'\">',logistik_order.responsible,'</p><script type=\"text/javascript\">
        new Ajax.InPlaceEditor(\'r-',oid,'\',\'/fetch.php?path=".$path->clean."&editorder\', {
                 callback: function(form, value) {
                  return \'id=',oid,'&responsible=\' + escape(value) }})
</script>') AS Ansvarig,";
        }
        else{
            $this->display("addorder.tpl");
            $show= "WHERE gid='".$path->vars['team']."' AND structure.event='".$_SESSION['event']."'";
            $team='';
            $more='';
        }
        $table->create("SELECT $team CONCAT('<p id=\"n-',oid,'\">',logistik_order.text,'</p><script type=\"text/javascript\">
        new Ajax.InPlaceEditor(\'n-',oid,'\',\'/fetch.php?path=".$path->clean."&editorder\', {
                 callback: function(form, value) {
                  return \'id=',oid,'&text=\' + escape(value) }})
</script>') AS Text,
                CONCAT('<p id=\"d-',oid,'\">',logistik_order.cnt,'</p><script type=\"text/javascript\">
        new Ajax.InPlaceEditor(\'d-',oid,'\',\'/fetch.php?path=".$path->clean."&editorder\', {
                 callback: function(form, value) {
                  return \'id=',oid,'&cnt=\' + escape(value) }})
</script>') AS Antal,
IF(url!=''&& url!='Editme',CONCAT('<a href=\"',logistik_order.url,'\" target=\"_new\">Open in new window</a>'),'') AS Url,
CONCAT('<p id=\"s-',oid,'\">',logistik_order.store,'</p><script type=\"text/javascript\">
        new Ajax.InPlaceEditor(\'s-',oid,'\',\'/fetch.php?path=".$path->clean."&editorder\', {
                 callback: function(form, value) {
                  return \'id=',oid,'&store=\' + escape(value) }})
</script>') AS Affär,status AS Status,$more
                CONCAT('<a id=\"add\" href=\"?remove=',logistik_order.oid,'\" onClick=\"if(!confirm(\'Säker på att du vill ta bort ',logistik_order.text,' ?\')) return false;\">» Ta bort</a>') AS '' 
                FROM logistik_order JOIN structure USING(gid) $show",'','p');
        


        }
    }

   
}
?>
