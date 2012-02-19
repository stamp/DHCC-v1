<?php

class module extends db {

    const version = '1.0.0';

    public function module() {
    
    }

    public function install() {
		return true;
    }

    public function uninstall() {
        return true;
    }

    public function display( $template ) {
        global $tpl;
            
            $tpl->setPath('modules/'.get_class($this).'/templates/');
            $res = $tpl->display($template);
            $tpl->defaultPath();

            return $res;
            
    }

    function version($retArr=true) {
        if($retArr) {
            return explode('.',self::version);
        }
        return self::version;
    }

    public function setId($id) {
	    $this->_id = $id;
    }

    public function getId() {
        if (isset($this->_id))
	        return $this->_id;
        return false;
    }

    public function setName($name) {
	    $this->_name = $name;
    }

    public function getName() {
        if (isset($this->_name))
	        return $this->_name;
        return 'Unknown';
    }

    public function setDescription($desc) {
	    $this->_desc = $desc;
    }

    public function getDescription() {
        if (isset($this->_desc))
	        return $this->_desc;
        return false;
    }

    public function setVersion($version) {
	    $this->_version = $version;
    }

    public function getVersion() {
        if (isset($this->_version))
	        return $this->_version;
        return false;
    }

    public function setRequiredModuleApi($requirement) {
	    $this->_requiredModuleApi = $requirement;
    }

    public function getRequiredModuleApi($retArr = true) {
        if ($retArr) {
            if (isset($this->_requiredModuleApi))
	            return explode('.',$this->_requiredModuleApi);
            return false;
        }
    
        if (isset($this->_requiredModuleApi))
            return $this->_requiredModuleApi;
        return false;
    }

    public function setRequiredCoreApi($requirement) {
	    $this->_requiredCoreApi = $requirement;
    }

    public function getRequiredCoreApi($retArr = true) {
        if ($retArr) {
            if (isset($this->_requiredCoreApi))
    	        return explode('.',$this->_requiredCoreApi);
            return false;
        }

        if (isset($this->_requiredCoreApi))
            return $this->_requiredCoreApi;
        return false;
    }

    public function setRequiredDbApi($requirement) {
	    $this->_requiredDbApi = $requirement;
    }

    public function getRequiredDbApi($retArr = true) {
        if ($retArr) {
            if (isset($this->_requiredDbApi))
	            return explode('.',$this->_requiredDbApi);
            return false;
        }
    
        if (isset($this->_requiredDbApi))
            return $this->_requiredDbApi;
        return false;
    }

    public function setMaintainer($m) {
	    $this->_maintainer = $m;
    }

    public function getMaintainer($retHtml = true) {
        if (isset($this->_maintainer)) {
        	if ($retHtml) 
	        	return htmlspecialchars($this->_maintainer);
	        else
	        	return $this->_maintainer;
	    }
        return false;
    }

    public function setDepends($d) {
	    $this->_depends = $d;
    }

    public function getDepends() {
        if (isset($this->_depends))
    	    return $this->_depends;
        return false;
    }

    public function setNoInstall() {
        $this->_noInstall = true;
    }

    public function getInstall() {
        return !(isset($this->_noInstall)&&$this->_noInstall);
    }
}   

?>
