<?php

class events extends module {


    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Event handler');
        $this->setDescription('events');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');


        parent::module();
    }

    function install () 
    {
        $tables = array (
            'events' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => "int(11)",
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),

                array(
                    'Field'   => 'name',
                    'Type'    => "varchar(255)",
                    'Default' => ''
                ),
                array(
                    'Field'   => 'shortname',
                    'Type'    => "varchar(255)",
                    'Default' => ''
                ),
                array(
                    'Field'   => 'start',
                    'Type'    => "date",
                    'Default' => '0000-00-00'
                ),
                array(
                    'Field'   => 'end',
                    'Type'    => "date",
                    'Default' => '0000-00-00'
                ),
                array(
                    'Field'   => 'description',
                    'Type'    => "text",
                    'Default' => ''
                ),
                array(
                    'Field'   => 'active',
                    'Type'    => "enum('Y','N')",
                    'Default' => 'Y'
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

    function changeEvent($event){
            $_SESSION['event'] = $event;
            return $_SESSION['event'];
    }
    
    // getEvent() {{{
    function getEvent($event = 0){
        if($event == 0 && isset($_SESSION['event']))
            $event = $_SESSION['event'];
        $event= $this->escapeStr($event);
        if($sql = $this->fetchSingle("SELECT id FROM events WHERE id='".$event."'"))
                $this->changeEvent($sql['id']);
        else {
           if($sql = $this->fetchSingle("SELECT id FROM events WHERE active='Y' ORDER BY start DESC")) {
                $this->changeEvent($sql['id']);
           } else {
                $sql = $this->fetchSingle("SELECT id FROM events ORDER BY start DESC");
                $this->changeEvent($sql['id']);
           }}
    }
    // }}}

// getEventSelect() {{{
    function getEventSelect()
    {
        
        global $tpl;
        if (isset($_GET['event'])) {
            $this->getEvent($_GET['event']);
        } else
            $this->getEvent();
        if ($sql = $this->fetchAll("SELECT name,id FROM events ORDER BY start")) {
            $tpl->assign('events',$sql);
        } 
    
    }
    // }}}


}
?>
