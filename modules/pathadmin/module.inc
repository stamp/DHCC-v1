<?php

class pathadmin extends module {
// {{{ !variables!
    public $vals = array();
// }}}

// {{{ __construct()
    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Path admin');
        $this->setDescription('Administration panel for the site structure');
        $this->setVersion('0.3.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');
        $this->setDepends('path');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');

        parent::module();
    }
// }}}
// {{{ install ()
    function install () {
        $tables = array (
            'tree' => array(
                array(
                    'Field'   => 'tid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'parent',
                    'Type'    => 'int'
                ),
                array(
                    'Field'   => 'path',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'type',
                    'Type'    => "ENUM( 'normal', 'var', 'extend','include')",
                    'Default' => 'normal'
                ),
                array(
                    'Field'   => 'vars',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'module',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'access',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'status',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'redir',
                    'Type'    => 'int(11)',
                    'Null'    => 'YES'
                ),
                array(
                    'Field'   => 'prio',
                    'Type'    => 'int(11)'
                )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }
// }}}
// {{{ uninstall() 
    function uninstall () 
    {
        $this->query("
            DROP TABLE tree
        ");
        parent::uninstall();
    }
// }}}

    // getChilds() {{{
    /**
     * Get the treestructure from database and return an array
     *
     */
    function getChilds($from) {
        if ( $subs = $this->fetchAll("SELECT * FROM tree WHERE parent=$from ORDER BY type='var',prio") ) 
            return $subs;
        return array();
    }
    /// }}}
    // getGrandchilds() {{{
    /**
     * Get the treestructure from database and return an array
     *
     */
    function getGrandChilds($from) {
        if($ret = $this->getChilds($from)) {
            foreach ( $ret as $key => $line ) {
                if ($childs = $this->getGrandChilds($line['tid']))
                    $ret[$key]['childs'] = $childs;
            }
        }
        
        if ( isset($ret) && is_array($ret) )
            return $ret;
        return array();
    }
    /// }}}
    // getChildsList() {{{
    function getChildsList($from) {
        if ( $subs = $this->fetchAll("SELECT tid,tid as val,path as text FROM tree WHERE parent=$from ORDER BY type='var'") ) 
            return $subs;
        return array();
    }
    /// }}}
    // getGrandchildsList() {{{
    function getGrandChildsList($from) {
        if($ret = $this->getChildsList($from)) {
            foreach ( $ret as $key => $line ) {
                if ($childs = $this->getGrandChildsList($line['tid']))
                    $ret[$key]['childs'] = $childs;
            }
        }
        
        if ( isset($ret) && is_array($ret) )
            return $ret;
        return array();
    }
    /// }}}
    // {{{ fixList() 
    function fixList($data,$level=0) {
        $ret = array();
        if (isset($data)&&is_array($data))
            foreach ($data as $line) {
                $ret[] = array(
                    'val' => $line['val'],
                    'text' => str_repeat('&nbsp;-&nbsp;',$level).$line['text']
                );

                if(isset($line['childs'])&&is_array($line['childs']))
                    if ($data2 = $this->fixList($line['childs'],$level+1))
                        $ret = array_merge($ret,$data2);
            }

        return $ret;
    }
    // }}}
    // {{{ moveUp(id)
    function moveUp($obj) {
        if(!is_numeric($obj))
            return false;

        if(!$obj = $this->fetchSingle("SELECT * FROM tree WHERE tid=$obj")) 
            return false;

        if(!$childs = $this->fetchAll("SELECT * FROM tree WHERE parent={$obj['parent']} ORDER BY prio"))
            return false;

        $up = false;
        foreach ($childs as $key => $line) {
            if ($line['tid']==$obj['tid']) {
                if($key-1>=0)
                    $bef1 = array_slice($childs,0,$key-1);
                else
                    $bef1 = array();
                
                if($key-1>=0)
                    $bef2 = array_slice($childs,$key-1,1);
                else
                    $bef2 = array();
                
                $after = array_slice($childs,$key+1);

                $res = array_merge($bef1,array($line),$bef2,$after);

                foreach($res as $key => $line2)
                    $this->query("UPDATE tree SET prio=$key WHERE tid={$line2['tid']}");


                return true;
            }
        }
            
    }
    // }}}
    // {{{ moveDown(id)
    function moveDown($obj) {
        if(!is_numeric($obj))
            return false;

        if(!$obj = $this->fetchSingle("SELECT * FROM tree WHERE tid=$obj")) 
            return false;

        if(!$childs = $this->fetchAll("SELECT * FROM tree WHERE parent={$obj['parent']} ORDER BY prio"))
            return false;

        $up = false;
        foreach ($childs as $key => $line) {
            if ($line['tid']==$obj['tid']) {
                $bef = array_slice($childs,0,$key);
                $after1 = array_slice($childs,$key+1,1);
                
                $after2 = array_slice($childs,$key+2);

                $res = array_merge($bef,$after1,array($line),$after2);

                foreach($res as $key => $line2)
                    $this->query("UPDATE tree SET prio=$key WHERE tid={$line2['tid']}");


                return true;
            }
        } 
    }
    // }}}

// {{{ _admin()
    function _admin() {
        global $tpl;
        
        if (isset($_GET['edit'])&&is_numeric($_GET['edit'])&&!isset($_POST['parent']))
            $v = new validate($this->fetchSingle("SELECT * FROM tree WHERE tid={$_GET['edit']}"));
        else
            $v = new validate($_POST);

        if (isset($_GET['add'])||isset($_GET['edit']))
            $showList = false;

        // saves {{{
        if (isset($_GET['up'])&&is_numeric($_GET['up']))
            $this->moveUp($_GET['up']);
        elseif(isset($_GET['down'])&&is_numeric($_GET['down']))
            $this->moveDown($_GET['down']);
        elseif (isset($_POST['path'])&&isset($_POST['parent'])) {
            
            $v->word('path',1,100);

            if (!is_numeric($_POST['parent']))
                $v->error['parent'] = 'Invalid input';
            
            $path = $this->escapeStr($_POST['path']);
            if (!isset($_GET['edit'])&&$this->fetchOne("SELECT * FROM tree WHERE path='$path' AND parent={$_POST['parent']}")) 
                $v->error['path'] = 'This path is already set';

            if(!$v->ExistErrors()) {
                if (isset($_GET['edit'])&&is_numeric($_GET['edit']))
                    $this->update(array('path'=>$_POST['path'],'parent'=>$_POST['parent'],'type'=>$_POST['type']),'tree',"WHERE tid={$_GET['edit']}");
                else
                    $this->insert(array('path'=>$_POST['path'],'parent'=>$_POST['parent'],'access'=>'G-2,','type'=>$_POST['type']),'tree');
                
                $showList = true;
            }
        } elseif (isset($_POST)&&count($_POST)>0&&!isset($_POST['path'])) {
            $data = array();
            foreach ($_POST as $key => $line) {
                $k = explode('-',$key);
                if (is_numeric($k[1])) {
                    if ($k[0] == 'method') $k[0] = 'vars';

                    if (!isset($data[$k[1]])) 
                        $data[$k[1]] = array();
                    $data[$k[1]][$k[0]] = $line;
                }
            }

            foreach ($data as $key => $line) 
                $this->update($line,'tree','WHERE tid='.$key);
        }
        // }}}

        if (isset($showList)&&$showList || !isset($showList)) {
            $tpl->assign('tree',$this->getGrandChilds(0));
            $this->display('show.tpl');
        } elseif (isset($_GET['add'])||isset($_GET['edit'])&&is_numeric($_GET['edit'])) {

            $tpl->assign('errors',$v->getAll());
            $tpl->assign('vals',$v->str);
            
            $tpl->assign('tree',array_merge(array(array('text'=>'root','val'=>'0')),$this->fixList($this->getGrandChildsList(0),1)));
            $this->display('add.tpl');
        }
    }
// }}}

    function _helper() {
        global $path;
        
        $tpl = new template('modules/pathadmin/templates/');
        
        if (isset($_GET['search'])) {
            $tpl->assign('searchdata',self::helper_search($_GET['search']));
            $tpl->display('table.tpl');
        } elseif(isset($_GET['show'])) {
            $tpl->assign('field',$_GET['show']);
            $tpl->display('adminAccess.tpl');
        }
    }
    
    function helper($id,$name,$access) {
        global $tpl,$path;
        $tpl->helper_id     = $id;
        $tpl->helper_name     = $name;
        $tpl->helper_value = $access;
        $tpl->helper_path   = $path->clean;
        $tpl->display('../../pathadmin/templates/helper_input.tpl');
    }
    
    function helper_search($s) {
        $data = array();

        $special = array(
            array(
                'name'  => 'Ej inloggade besökare',
                'access'=> 'G0'
            ),
            array(
                'name'  => 'Inloggade besökare',
                'access'=> 'G-1'
            ),
            array(
                'name'  => 'Administratörer',
                'access'=> 'G-2'
            ),
            array(
                'name'  => 'Medlem i aktivt event',
                'access'=> 'G-3'
            ),
        );

        if(substr($s,0,1)=='!') {
            $sel = explode(',',substr($s,1));

            $g = array();
            $u = array();

            foreach ($sel as $l) {
                if (substr($l,0,1)=='U')
                    $u[] = substr($l,1);
                if (substr($l,0,1)=='G')
                    $g[] = substr($l,1);
            }

            foreach ($special as $line) {
                if (db::fetchOne("SELECT '{$line['access']}' IN ('".implode($sel,"','")."')"))
                    $data[] = array(
                        'type'  => 'Global',
                        'name'  => $line['name'],
                        'access'=> $line['access']
                    );
            }

            if($user = db::fetchAll("SELECT 'User' as type, concat(firstname,' \"',username,'\" ',lastname) as name,concat('U',uid) as access,'' as event FROM users WHERE uid IN (".implode($u,',').")"))
                $data = array_merge($data,$user);
            if($team = db::fetchAll("SELECT (CASE is_team WHEN 'Y' THEN 'Team' ELSE 'Group' END) as type,structure.name,concat('G',gid) as access,events.shortname as event FROM structure LEFT JOIN events ON events.id=structure.event WHERE structure.gid IN (".implode($g,',').") ORDER BY structure.name,events.name"))
                $data = array_merge($data,$team);
        } else {


            foreach ($special as $line) {
                if (db::fetchOne("SELECT '{$line['name']}' LIKE '%".db::escapeStr($s)."%'"))
                    $data[] = array(
                        'type'  => 'Global',
                        'name'  => $line['name'],
                        'access'=> $line['access']
                    );
            }

            if($user = db::fetchAll("SELECT 'User' as type, concat(firstname,' \"',username,'\" ',lastname) as name,concat('U',uid) as access,'' as event FROM users WHERE concat(firstname,' \"',username,'\" ',lastname) LIKE '%".db::escapeStr($s)."%'"))
                $data = array_merge($data,$user);
            if($team = db::fetchAll("SELECT (CASE is_team WHEN 'Y' THEN 'Team' ELSE 'Group' END) as type,structure.name,concat('G',gid) as access,events.shortname as event FROM structure LEFT JOIN events ON events.id=structure.event WHERE structure.name LIKE '%".db::escapeStr($s)."%' ORDER BY structure.name,events.name"))
                $data = array_merge($data,$team);
        }

        return $data;
    }
}


?>
