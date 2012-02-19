<?php


class template {

    private $path       = '';
    public $skin       = '';
    private $default    = '';

    function template($path = '') {
        if ($path != '') {
            if (!is_dir($path))
                return false;

            $path = trim($path, '/');

            $this->path = $path.'/';
            $this->default = $path.'/';
        }
    }

    function display($file) {
        if ($this->skin != '') {
            $skin = substr($file,0,-3).$this->skin.substr($file,-4);
            if (is_file($this->path.'locales/'.$skin)) 
                return include($this->path.'locales/'.$skin);
        }

        if (is_file($this->path.'locales/'.$file)) 
            return include($this->path.'locales/'.$file);  

        if ($this->skin != '')  {
            if (is_file($this->path.$skin)) 
                return include($this->path.$skin);  
        }

        if (is_file($this->path.$file)) 
            return include($this->path.$file);  
        
        send(E_WARNING,"Template '".$this->path.$file."' is missing!");
        return false;
    }

    function setPath($path) {
        if (!is_dir($path))
            return false;

        $path = trim($path, '/');

        $this->path = $path.'/';
        return true;
    }

    function defaultPath() {   
        if ($this->default != '')
            $this->setPath($this->default);
    }
   // assign {{{
   /**
    * assign a value to a variable
    *
    * @param string key    name of the variable
    * @param string/array  value of the variable
    */
   public function assign($key, $val)
   {
      // we are not allowed to reset a variable
      if (isset($this->$key)) {
          send(E_NOTICE,"The variable '{$this->$key}' is already defined");
          return false;
      }

      // insufficient args
      if ( empty($key) ) return false;

      $this->$key = $val;
      return true;
   }

   // }}}
}

?>
