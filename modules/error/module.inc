<?php

class error extends module {

    public $vars = array();

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Advanced errorhandler');
        $this->setDescription('A more complex way to handle errors');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');


        parent::module();
    }

    function install () 
    {
        parent::install();
        return true;
    }

    function uninstall () 
    {
        parent::uninstall();
    }
    
    function startup() {
        include('errorhandler.php');
    }


    // send() - the message queue handler {{{
    /**
     * the message queue handler, save messages to logg/queue
     *
     */
    function send($level,$msg,$save = true,$display = true) {

        $data = array(
            'level'=>$level,
            'message'=>$msg,
            'uid'=> (isset($_SESSION['id'])) ? $_SESSION['id'] : 0
           // 'backtrace' => serialize(debug_backtrace())
        );
        //if ($save) $db->insert($data,'logg');

        if ($display) {
            if(!isset($this->data)) $this->data = array();
            array_push($this->data,$data); 
        }

    }
    // }}}
    // get() - print out the message queue {{{
    /**
     * print out the message queue
     *
     */
    function get() {
        global $tpl;

        if ( isset($this->data) && is_array($this->data) && count($this->data)>0 ) {
            if (get_class($tpl)=='template') {
               if (isset($tpl->logg)) 
                   $tpl->logg = $this->data;
                else 
                    $tpl->assign('logg',$this->data);

               if (!$this->display('display_errors.tpl'))
                    core::printArr($this->data);

            } else core::printArr($this->data);
        }
        $this->data = array();

    }
    // }}}

}
?>
