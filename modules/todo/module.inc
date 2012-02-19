<?php
class todo extends module {

    public $vars = array();

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Todo');
        $this->setDescription('easy-to-use, todo lists with deadlines, status and responsible');
        $this->setVersion('0.0.1');
        $this->setRequiredDbApi('2.0');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
        $this->setDepends('aculo','safety');
        parent::module();
    }

    // {{{ install
    public function install() {
        $tables = array (
            'todo' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'list',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'responsible',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'deadline',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'status',
                    'Type'    => "enum('active','pending','halted','aborted','finished')"
                ),
                array(
                    'Field'   => 'process',
                    'Type'    => 'int(3)'
                )
            ),
            'todo_list' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                )
            )
        );
        $this->installTables($tables);

        return true;
    }
    // }}}
    function _run() {
        global $tpl,$path;
        
        $tpl->assign('write',$path->write);

        if (!isset($_GET['PG'])||!$path->write) {
            if($lists = $this->fetchAll("SELECT * FROM todo_list ORDER BY head"))
                foreach ($lists as $key => $line) 
                    $lists[$key]['posts'] = $this->fetchAll("SELECT todo.*,users.username AS `responsible1` FROM todo LEFT JOIN 
                    users ON users.uid=todo.responsible WHERE list={$line['id']} ORDER BY status='finished',status='aborted',status='halted',status='pending',status='active',process,head");

            $tpl->assign('todo',$lists);
            $this->display('show.tpl');
        } else {
            if (is_numeric($_GET['PG'])) {
                if ($_GET['PG']==0) {
                    if (isset($_POST['text'])&&isset($_POST['head'])&&isset($_GET['main'])&&is_numeric($_GET['main'])) {
                        $head = utf8_decode($_POST->sql('head'));
                        $text = utf8_decode($_POST->sql('text'));
                        $this->query("INSERT INTO todo SET head='{$head}',text='{$text}',list={$_GET['main']}");
                        $id = mysql_insert_id();
                    }
                    $tpl->assign('data',$this->fetchSingle("SELECT * FROM todo WHERE id=$id"));
                    
                    $this->display('fullpost.tpl');
                } else {
                    if (isset($_POST['text'])&&isset($_POST['head'])) {
                        $head = utf8_decode($_POST->sql('head'));
                        $text = utf8_decode($_POST->sql('text'));
                        $status = utf8_decode($_POST->sql('status'));
                        $process = utf8_decode($_POST->sql('process'));
                        $responsible = utf8_decode($_POST['responsible']);
                        $this->query("UPDATE todo SET head='{$head}',text='{$text}',status='{$status}',responsible='{$responsible}',process='{$process}' WHERE id={$_GET['PG']} LIMIT 1");
                    }
                    $tpl->assign('data',$this->fetchSingle("SELECT todo.*,users.username AS `responsible1` FROM todo LEFT JOIN users ON users.uid=todo.responsible WHERE id={$_GET['PG']}"));
                    
                    if (isset($_POST['text'])&&isset($_POST['head'])) {
                        $this->display('post.tpl');
                    } else {
                        $this->display('edit.tpl');
                    }
                }
            }

        }
    }

}




?>
