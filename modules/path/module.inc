<?php

class path extends module {

    public $vars = array();

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Path manager');
        $this->setDescription('managing the viritual paths and urls');
        $this->setVersion('0.3.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');

        parent::module();
    }

    function install () 
    {
       
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
                    'Type'    => "ENUM( 'normal', 'var', 'redir', 'extend')",
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
                    'Field'   => 'write',
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

    function uninstall () 
    {
        parent::uninstall();
    }

    function process() {
        $parent  = 0;
        $redir   = '';
        $newpath = array();
        $path    = $_GET['path'];
        $path    = ($path > '') ?  explode('/',trim($path,'/')) : array('root');
        $this->sq = "access REGEXP '.*(".$this->getAccess().").*'";
        $this->sq2 = "`write` REGEXP '.*(".$this->getAccess().").*'";

        $old = $_GET;
        $_GET = array();
        foreach ($old as $key => $line)
            if ($key != 'path')
                $_GET[$key] = $line;

        // loop the input path
        foreach($path as $key => $line) {
            if($data = $this->fetchSingle(
                    "SELECT *,access AS accessline,{$this->sq} as access,{$this->sq2} as `write` FROM tree WHERE parent=$parent AND NOT (status = 'disabled' OR status = '') AND ( ( path LIKE '".db::escapeStr($line)."' ) OR ( type = 'var' ) ) ORDER BY type"
                )) {
                if ($data['access']) {
                    $newpath[$key]          = $data;
                    $newpath[$key]['value'] = $line;
                    $parent                 = $data['tid'];
                    $redir                  = $data['redir'];
                    $this->module = $data['module'];
                    $this->method = $data['vars'];
                    $this->tid = $data['tid'];
                    $this->write  = $data['write'];
                    $this->accessline = $data['accessline'];
                    if ($data['type']=='extend') {
                        $this->vars['path'] = array_slice($path,$key+1);
                        foreach ($this->vars['path'] as $key => $line)
                            $this->vars['path'][$key] = $this->decode($line);
                        break;
                    }
                    if ($data['type'] == 'var') {
                        $this->fetchExeption($data['vars'],$this->decode($line));
                        $this->sq = "REPLACE(access,'#{$data['vars']}','{$this->vars[$data['vars']]}') REGEXP '.*(".$this->getAccess().").*'";
                        $this->sq2 = "REPLACE(`write`,'#{$data['vars']}','{$this->vars[$data['vars']]}') REGEXP '.*(".$this->getAccess().").*'";
                    }
                } else {
                    $newpath = array(
                        array(
                            'head' => 'Serverfel',
                            'path' => 'error',
                            'tid'  => -1,
                            'type' => 'normal'
                        ),
                        array(
                            'head' => 'Access denied',
                            'path' => 'error',
                            'tid'  => -1,
                            'type' => 'normal'
                        )
                    );
                    $this->module = 'template';
                    $this->method = '403.tpl';
                }
            } else {
                break;
            }
        }
        // do the redirs
        for ($i=0;$i<4 && $redir!='';$i++) {
            if($data = $this->fetchSingle(
                    "SELECT *,access REGEXP '.*(".$this->getAccess().").*' as access,`write` REGEXP '.*(".$this->getAccess().").*' as `write` FROM tree WHERE tid=$redir"
                )) {

                if ($data['type'] == 'var') $this->fetchExeption($data['vars'],'');

                if ($data['access']) {
                    $newpath[]  = $data;
                    $parent     = $data['tid'];
                    $redir      = $data['redir'];
                    $this->module = $data['module'];
                    $this->method = $data['vars'];
                    $this->write  = $data['write'];
                    if ($data['type'] == 'var') {
                        $this->fetchExeption($data['vars'],$this->decode($line));
                        $this->sq = "REPLACE(access,'#{$data['vars']}','{$this->vars[$data['vars']]}') REGEXP '.*(".$this->getAccess().").*'";
                    }
                } else {
                    $newpath = array(
                        array(
                            'head' => 'Serverfel',
                            'path' => 'error',
                            'tid'  => -1,
                            'type' => 'normal'
                        ),
                        array(
                            'head' => 'Access denied',
                            'path' => 'error',
                            'tid'  => -1,
                            'type' => 'normal'
                        )
                    );
                    $this->module = 'template';
                    $this->method = '403.tpl';
                }
            } else break;
        }
        // Path not found 404
        if (count($newpath)==0) {
            $newpath = array(
                array(
                    'head' => 'Serverfel',
                    'path' => 'error',
                    'tid'  => -1,
                    'type' => 'normal'
                ),
                array(
                    'head' => 'Page not found',
                    'path' => 'error',
                    'tid'  => -1,
                    'type' => 'normal'
                )
            );
            $this->module = 'template';
            $this->method = '404.tpl';
        }


        // make the new path
        $clean = '';
        $head = '';
        foreach($newpath as $key => $line) {
            if ($line['type'] == 'var' && !isset($line['value']))
                $newpath[$key]['value'] = $this->menuExeption($line['vars'],$this->vars[$line['vars']]);

            $l = ($line['type'] == 'var') ?  $newpath[$key]['value'] : $line['path'];

            $clean .= '/'.$l;
            if ($line['head']>''&&$line['type'] != 'var') 
                $t = $line['head'];
            else
                if (isset($this->vars[$line['vars']])) 
                    $t = $this->headExeption($line['vars'],$this->vars[$line['vars']]);
                else
                    $t = '';
            if ($t > '') {
                if ($head > '') {
                    $head  .= ' > '.$t;
                } else {
                    $head  = $t;
                }
            }
        }
        
        $this->clean = $clean;
        $this->path = $newpath;
        $this->head = $head;
        return $clean;
    }

    function exeption ($name,$value) {
        switch ($name) {
            case 'uid':
                $def = isset($_SESSION['id']) ? $_SESSION['id'] : false;
                if ($value == '')
                    return $def;
                if ($user = $this->fetchOne("SELECT uid FROM users WHERE username='$value'")) 
                    return $user;
                return $def;
            case 'team':
                if ($team = $this->fetchOne("SELECT gid FROM structure WHERE name='$value' AND NOT name='' AND event='".$_SESSION['event']."'")) 
                    return $team;
                return -1;

            case 'forum':
                if ($data = $this->fetchSingle("SELECT head,id FROM forums WHERE head='".$this->decode($value)."'"))
                    return $data;   
                return false;
            case 'topic':
                if (!isset($this->vars['forum']['id']))
                    return false;

                if ($data = $this->fetchSingle("SELECT head,id FROM topics WHERE forum={$this->vars['forum']['id']} AND head='".$this->decode($value)."'"))
                    return $data;   
                return false;
            default:
                return false;
        }
    }

    function menuExeption ($name,$value) {
        switch ($name) {
            case 'uid':
                if ($user = $this->fetchOne("SELECT username FROM users WHERE uid='$value'"))
                    return $user;

                $def = isset($_SESSION['id']) ? $_SESSION['id'] : false;
                if ($user = $this->fetchOne("SELECT username FROM users WHERE uid='$def'")) 
                    return $user;

                return 'Okänd';
            case 'team':
                if ($team = $this->fetchOne("SELECT name FROM structure WHERE gid='$value'"))
                    return $team;

                return 'Okänd';

            case 'forum':
                return $value['head'];
            case 'topic':
                return $value['head'];
            default:
                return false;
        }
    }

    function headExeption ($name,$value) {
        switch ($name) {
            case 'uid':
                if ($user = $this->fetchOne("SELECT concat(firstname,' \"',username,'\" ',lastname) FROM users WHERE uid='$value'"))
                    return $user;

                $def = isset($_SESSION['id']) ? $_SESSION['id'] : false;
                if ($user = $this->fetchOne("SELECT username FROM users WHERE uid='$def'")) 
                    return $user;

                return 'Okänd';
            case 'team':
                if ($team = $this->fetchOne("SELECT name FROM structure WHERE gid='$value'"))
                    return $team;

                return 'Okänd';
            case 'forum':
                return $value['head'];
            case 'topic':
                return $value['head'];
            default:
                return false;
        }
    }

    function fetchExeption ($name,$value) {
        if($ret = $this->exeption ($name,$value)) 
            $this->vars[$name] = $ret;
    }

    function encode($in) {
        $ut = preg_replace_callback("/[^a-zA-Z0-9\-\*\(\)\,\s]/",'str_encode',$in);
        $ut = str_replace(' ','_',$ut);
        //$ut= db::escapeStr($ut);
        return $ut;
    }

    function decode($ut) {
        $ut = str_replace('_',' ',$ut);
        $ut = preg_replace_callback("/![a-f0-9]{2}+/",'str_decode',$ut);
        $ut= db::escapeStr($ut);
        return $ut;
    }

    function getTreeMenu ($from = 0,$rek = true,$path = NULL) {
        if ($path == NULL) {
            $path = '';
        }

        if ($data = $this->fetchAll("SELECT tid,head,path,type,vars FROM tree WHERE parent=$from AND NOT (status = '' OR status='hidden' OR status='disabled') AND {$this->sq} ORDER BY prio") ) {
            foreach ($data as $key => $line) {
                if ($line['type']=='var') {
                    if(isset($this->vars[$line['vars']]))
                        $data[$key]['head'] = $this->menuExeption($line['vars'],$this->vars[$line['vars']]);
                    else 
                        $data[$key]['head'] = '';
                    $data[$key]['path'] = $this->encode($data[$key]['head']);
                }

                $data[$key]['path'] = $path . '/' . $data[$key]['path'];
                
                if($rek && $childs = $this->getTreeMenu($line['tid'],$data[$key]['path'],true))
                    $data[$key]['childs'] = $childs;
            }

            $ret = array();

            foreach($data as $key => $line)
                if($line['head']>'')
                    $ret[] = $line;
            
            if(count($ret)>0)
                return $ret;
            return false;
        }

        return false;
    }

    function getLevelMenu() {
        $menu = array();

        $menu[] = $this->getTreeMenu(0,'',false);
        $path = '';

        if(isset($this->path)&&is_array($this->path))
            foreach ($this->path as $key => $line) {    
                $prev = ($line['type'] == 'var') ?  $line['value'] : $line['path'];
                if($prev>'') $path .= '/'.$prev;

                if($data = $this->getTreeMenu($line['tid'],false,$path)) {
                    $menu[] = $data;
                }
            }

        return $menu;

    }

    function getAccess() {
        if (isset($this->access)) 
            return $this->access;
            
        $this->access = (isset($_SESSION['access'])) ? $_SESSION['access'] : 'G0,';
        return $this->access;
    }
}

function str_encode ($char) {
    return '!'.str_pad(dechex(ord($char[0])),2,'0',STR_PAD_LEFT);
}
function str_decode ($char) {
    return chr(hexdec((ltrim($char[0],'!'))));
}

?>
