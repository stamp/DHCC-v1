<?php
class listmaker extends module {
   // __construct {{{
    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('List manager');
        $this->setDescription('managing and saving sql-queries to make custom lists');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('tables');
        $this->setMaintainer('Jonas Falck <jonaz@jonaz.net>');
        

        parent::module();
    }
   // }}}

    function _forgroups(){
$validfields= array(
                'username'=>'Nick',
                'CONCAT(firstname,\' \',lastname)'=>'Namn',
                'email'=>'Email',
                'email2'=>'Email2',
                'CONCAT(street,\' <br />\',postcode,\' \',city)'=>'Address',
                'primaryphone'=>'Primär telefon',
                'secondaryphone'=>'Sekundär telefon',
                'medical'=>'Sjukdomar',
                'food'=>'Mat-info(allegier)',
                'were'=>'Sover var',
                'on_what'=>'Sover på',
                'with_who'=>'Sover med',
                'sleep_hard'=>'sover hårt',
                'wakeup'=>'Väcks hur',
                'snore'=>'Snarkar',
                'arrive'=>'Ankommer datum',
                'arrive_time'=>'Ankommer tid',
                'depart'=>'Åker datum',
                'depart_time'=>'Åker tid',
                'car'=>'Bil reg nr',
                'size'=>'T-shirt storlek',
                'gsize'=>'T-shirt storlek PRESENT',
                'logincount'=>'Antal login',
                'birthdate'=>'Personnummer',
                'ice'=>'ICE nummer',
                'latestlogin'=>'Senaste login');
$tables= array("users","membership"=>'uid','structure'=>'gid','user_profile'=>'uid','user_eventinfo'=>'uid');
        $this->run($validfields,$tables);
    }

    function _textpage(){
        global $path;
    $validfields= array(
                'structure.name'=>'Team',
                'username'=>'Nick',
                'CONCAT(firstname,\' \',lastname)'=>'Namn',
                'email'=>'Email',
                'email2'=>'Email2',
                'CONCAT(street,\' <br />\',postcode,\' \',city)'=>'Address',
                'primaryphone'=>'Primär telefon',
                'secondaryphone'=>'Sekundär telefon',
                'medical'=>'Sjukdomar',
                'food'=>'Mat-info(allegier)',
                'were'=>'Sover var',
                'on_what'=>'Sover på',
                'with_who'=>'Sover med',
                'sleep_hard'=>'sover hårt',
                'wakeup'=>'Väcks hur',
                'snore'=>'Snarkar',
                'arrive'=>'Ankommer datum',
                'arrive_time'=>'Ankommer tid',
                'depart'=>'Åker datum',
                'depart_time'=>'Åker tid',
                'car'=>'Bil reg nr',
                'size'=>'T-shirt storlek',
                'gsize'=>'T-shirt storlek PRESENT',
                'logincount'=>'Antal login',
                'birthdate'=>'Personnummer',
                'ice'=>'ICE nummer',
                'gid'=>'gid',
                'uid'=>'uid',
                'latestlogin'=>'Senaste login');
$tables= array("users","membership"=>'uid','structure'=>'gid','user_profile'=>'uid','user_eventinfo'=>'uid');
        $this->run($validfields,$tables,'t-'.$path->tid);
    }
    function run($validfields,$tables,$saveid=NULL){
        
        global $tpl,$path;
        
        if(isset($saveid) && $saveid != NULL)
            $tpl->assign('admin',1);
        if(isset($path->write) && $path->write =1)
            $tpl->assign('write',1);
        
        $select[]=array('text'=>'Ingen','val'=>'0');
        foreach($validfields AS $key=>$val){
            $select[$key] = array('text'=>$val,'val'=>$val);
        }
        if(isset($_POST)){
            $fields = array();
            foreach ($_POST as $key=>$val) {
                if(in_array($val,$validfields) ){
                    foreach($validfields AS $key1=>$val1)
                        if($val1==$val) $key =$key1;
                    $fields[$key]= $val;
                }
            }
        }
        if($saveid== NULL)
            $list = new lista('G'.$path->vars['team'].'-list');
        else
            $list = new lista($saveid);
        $list->tables($tables);
        $list->fields($fields);
        if(isset($_POST['where']))
            $list->where($_POST['where']);
        elseif(isset($path->vars['team']))
            $list->where($path->vars['team']);

        if(isset($_POST['name']))
            $list->save($_POST['name']);
        if(isset($_GET['remove'])){
            $list->remove($_GET['remove']);
        }
        $v = new validate($_POST);
        $tpl->assign('select',$select);
        $tpl->assign('errors',$v->getAll());
        if($path->write == 1){

        if(isset($_GET['edit'])){
            $tpl->assign('show',1);
            $tpl->assign('vals',$list->getpostfields($_GET['edit']));
        }
        else
            $tpl->assign('vals',$_POST);
        $this->display('addlist.tpl');
        }
        $tpl->assign('lists',$list->loadall());
        $this->display('showlist.tpl');

        if(isset($_GET['list']))
            $list->printlist($_GET['list'],array('teamsonly'=>1));


    }



}
class lista {

    private $fields='';
    private $tables='';
    private $where='';
    private $fieldsarray = array();
    private $gid='';

    function __construct($gid) {
        $this->gid=$gid;
    }
    // getChildsList {{{
    function getChildsList($from) {
        if ( $subs = db::fetchAll("SELECT gid,gid as val,name as text FROM structure WHERE (parent='".$from."' AND (event='".$_SESSION['event']."' OR event=0) AND name NOT LIKE '-%') ORDER BY name")) 
            return $subs;
        return array();
    }
   // }}}
   // getGrandChildsList {{{
    function getGrandChildsList($from) {
        if($ret = $this->getChildsList($from)) {
            foreach ( $ret as $key => $line ) {
                if ($childs = $this->getGrandChildsList($line['gid']))
                    $ret[$key]['childs'] = $childs;
            }
        }
        
        if ( isset($ret) && is_array($ret) )
            return $ret;
        return array();
    }
   // }}}
// fixList {{{
    function fixList($data,$level=0,$valsonly=0) {
        $ret = array();
        if (isset($data)&&is_array($data))
            foreach ($data as $key=>$line) {
                if($valsonly ==1)
                    $ret[] = $line['val'];
                else
                $ret[] = array(
                    'val' => $line['val'],
                    'text' => str_repeat('&nbsp;-&nbsp;',$level).$line['text']
                );

                if(isset($line['childs'])&&is_array($line['childs']))
                    if ($data2 = $this->fixList($line['childs'],$level+1,$valsonly))
                        $ret = array_merge($ret,$data2);
            }

        return $ret;
    }
   // }}}
    public function tables($t){
        if(count($t)==0)
            return 0;

        if(count($t)==1){
            return 'FROM '.$t;
        }
        else{
            $i=0;
            $str = '';
            foreach($t AS $key=>$val){
                if(is_numeric($key))
                        $key = $val;
                if($i==0){
                    $str .= 'FROM '.$key;
                }
                else{
                    $str .= ' LEFT JOIN '.$key.' USING ('.$val.')';
                }
                $i++;
            }
            $this->tables= $str;
        }
    }
    public function where($s){
        $str=$s;
        if(is_numeric($s)){

        $str='';
        $root = array($s);
        $array =array_merge($root,$this->fixList($this->getGrandChildsList($s),'',1));
        foreach($array AS $val)
        {
        
                $str .= ' gid='.$val.' OR';
        }
                $str=substr($str,0,-2);
        }
     if(!is_numeric($s) && substr($str, 0, 5)== "WHERE")
        $this->where =$str;
     else
         $this->where =' WHERE (('.$str.') AND name NOT LIKE \'-%\')';

    }
    public function fields($s){
    $str="SELECT gid,";
    foreach($s AS $key=>$val){
                if(!is_numeric($key))
                    $str .= $key.' AS \''.$val.'\', ';
                else
                    $str .= $val.', ';

            }

                   $str=substr($str,0,-2);
        $str.=' ';
        $this->fieldsarray =$s;
        $this->fields = $str;
    }
    private function mkselect(){
        return $this->fields.$this->tables.$this->where;
    }
    public function loadall($gid =NULL){
        if($gid==NULL)
            $gid=$this->gid;
        global $db;
        $data =0;
        $data =read($gid);
        if($data==NULL)
            return array();
        $data = unserialize($data);
        if(count($data)>0)
            return $data;
        return array();
    }
    public function remove($id){
        if(is_numeric($id)){
            $data = array();
            $data = $this->loadall($this->gid);
            unset($data[$id]);
            $data =serialize($data);
            write($this->gid,$data);
        }
    }
    public function save($name,$gid=NULL){
        if($gid==NULL)
            $gid=$this->gid;
        $data = array();
        $data = $this->loadall($gid);
        $edit=-1;
        if(count($data)>0)
        foreach($data AS $key=>$val){
            if($val['name']==$name){
                $edit=$key;
                break;
            }
        }
        if($edit!=-1)
            $data[$edit] = array('name'=>$name,'sql'=>$this->mkselect(),'fields'=>$this->fieldsarray,'where'=>$this->where);
        else
            $data[] = array('name'=>$name,'sql'=>$this->mkselect(),'fields'=>$this->fieldsarray,'where'=>$this->where);
        $data =serialize($data);
        write($gid,$data);
        send(E_USER_NOTICE,"Ny lista skapades framgångsrikt");
    }
    public function getfields($id){
        $all =$this->loadall($this->gid);
            $field=array();
            foreach($all AS $key=>$val){
                if($key==$id)
                    $field=$all[$key]['fields'];
            }
            return $field;
    }
    public function getwhere($id){
        $all =$this->loadall($this->gid);
            $field=array();
            foreach($all AS $key=>$val){
                if($key==$id)
                    $field=$all[$key]['where'];
            }
            return $field;
    }

    public function getpostfields($id){
        $all =$this->loadall($this->gid);
            $field=array();
            foreach($all AS $key=>$val){
                if($key==$id){
                    $i=0;
                    foreach($val['fields'] AS $val1){
                        $field[$i]=$val1;
                        $i++;
                    }
                    $field['name']=$val['name'];
                    $field['where']=$val['where'];
                }
            }
            return $field;
    }
    public function printlist($id=NULL,$options=NULL){
        //echo $this->fields;
        //echo $this->tables;
        if($id!=NULL){
            $fields=$this->getfields($id);
            if(count($fields)>0){

            $this->fields($fields);
            if($this->where=='')
                $this->where=$this->getwhere($id);
            $table = core::load('tables');
            if(isset($options['teamsonly']) && $options['teamsonly']==1){

                $data= db::fetchAll($this->mkselect());
                foreach($data AS $key=>$val){
                    if(isset($data[$key]['Team']) && $tmp=teams::getTeamFromChildGroup($val['gid']))
                        $data[$key]['Team'] = db::fetchOne("SELECT name FROM structure WHERE gid=".$tmp."");

                unset($data[$key]['gid']);
                }
            }
            else
            foreach($data AS $key=>$val)
                unset($data[$key]['gid']);
            $table->create($data,'','l');
            unset($table);
            }
        }elseif(isset($this->fields) && $this->fields >''){
            $table = core::load('tables');
            $table->create($this->mkselect(),'','l');
            unset($table);
        }
    }

}


?>
