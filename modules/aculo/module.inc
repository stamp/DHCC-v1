<?php
class aculo extends module {

    public $vars = array();

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('script.aculo.us');
        $this->setDescription('easy-to-use, compatible and, ultimately, totally cool JavaScript libraries');
        $this->setVersion('0.1.0');
        $this->setNoInstall();

        parent::module();
    }
}
