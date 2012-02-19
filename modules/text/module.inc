<?php

class text extends module {

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Textpage');
        $this->setDescription('managing and editing textpages with a WYCIWUG editor');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');

        parent::module();
    }

    function install () 
    {

        $tables = array (
            'page' => array(
                array(
                    'Field'   => 'pid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'tid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
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
    
    function _run() {
        
        global $path,$tpl;
        
        $tpl->assign('write',$path->write);

        if(!$text = $this->fetchOne("SELECT text FROM page WHERE tid=".$path->tid)) { 
            $this->query('INSERT INTO page SET tid='.$path->tid);
            $tpl->assign('content','');
        } else 
            $tpl->assign('content',$text);

        if (isset($_GET['edit']) && $path->write) {
            if (isset($_POST['text'])) {
                $text = $this->escapeStr($_POST['text']);
                if ($this->query("UPDATE page SET text='$text' WHERE tid=".$path->tid)) {
                    $tpl->content = $_POST['text'];
                    $this->display('show.tpl');
                    return false;
                }
            }

            $this->display('edit.tpl');
            return false;
        } 

        
        $this->display('show.tpl');
    
    }
}
?>
