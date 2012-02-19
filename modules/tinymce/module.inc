<?php

class tinymce extends module {

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('TinyMCE');
        $this->setDescription('platform independent web based Javascript HTML WYSIWYG editor');
        $this->setVersion('2.0.6');
        $this->setRequiredModuleApi('1.0');
        
        parent::module();
    }
    
    function init($type = 'wordlike') {
        global $mceInit,$tpl;
        if (!$mceInit) {
            $mceInit = true;
            switch($type) {
                case 'wordlike':
                    $this->display('init.word.tpl');
                    break;
                case 'simple':
                    $this->display('init.simple.tpl');
                    break;
            }
        }


    }
    
    function getPath($path,$absolute,$rel = '') {
        $path = rtrim($path,'/');

        if (!is_dir($path))
            return false;

        $content = scandir($path);
        $ret = array();

        foreach ($content as $line) {
            if ($line != '.' && $line != '..') {
                if (is_dir($path.'/'.$line)) {
                    if ($childs = $this->getPath($path.'/'.$line,$absolute.'/'.$line,$rel.$line.'/')) {
                        $ret = array_merge($ret,$childs);
                    }
                } else {
                    $ret[$absolute.'/'.$line] = $rel.$line;
                }
            }
        }
        return $ret;
    }

    function imageslist() {
        global $tpl;
        $tpl->assign('tree',$this->getPath('../../images','/images'));
        $this->display('image_list.tpl');
    }

    function linkslist() {
        global $path,$tpl;

        if (!is_object($path) && get_class($path)!='path')
            $path = core::load('path');
    
        $tpl->assign('tree',$path->getTreeMenu());
        
        $this->display('link_list.tpl');

    }
}

?>
