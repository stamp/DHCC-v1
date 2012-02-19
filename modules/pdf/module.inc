<?php

class pdf extends module {
    
    function __construct() {
        $this->setId(get_class($this));
        $this->setName('PDF');
        $this->setDescription('A module for making pdf files');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('safety');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }
    
    function create ($orientation = 'P', $unit = 'mm', $format = 'A4') {
        if (!in_array($orientation,array('Portrait','P','Landscape ','L')))
            return send(E_ERROR,'Failed creating PDF document.. Orientation must be Portrait or Landscape!');

        if (!in_array($unit,array('pt','mm','cm','in')))
            return send(E_ERROR,'Failed creating PDF document.. Unit must be pt, mm, cm or in<br><i>A point equals 1/72 of inch, that is to say about 0.35 mm (an inch being 2.54 cm)</i>');

        if (is_string($format) && !in_array($format,array('A3', 'A4', 'A5', 'Letter', 'Legal')))
            return send(E_ERROR,'Failed creating PDF document.. Format must be A3, A4, A5, Letter, Legal or an two-element array containing the width and the height (expressed in the unit given by unit)');

        if (is_array($format) && count($format) != 2)
            return send(E_ERROR,'Failed creating PDF document.. Format array must have 2 elements!');
        
        if (!include_once('fpdf.php'))
            return send(E_ERROR,'Failed creating PDF document.. Could not include pdf subclass!');
        
        $this->pdf = new fpdf($orientation, $unit, $format);
    }
    
    function save($filename) {
        return $this->pdf->Output($filename,'F');
    }

    function send($filename) {
        return $this->pdf->Output($filename,'D');
    }

    function display($filename = 'report.pdf') {
        return $this->pdf->Output($filename,'I');
    }

    function flush() {
        return $this->pdf->Output('','S');
    }

    function __call($funct,$args) {
        return call_user_func_array(array(&$this->pdf,$funct),$args);
    }
}

?>
