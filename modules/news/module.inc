<?php

class news extends module {

    function news ($tot = 0) {
        $this->setId('news');
        $this->setName('News');
        $this->setDescription('News lists ');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install () 
    {
       
        $tables = array (
            'news' => array(
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
                    'Field'   => 'writer',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => "varchar(255)"
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'timestamp',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'new',
                    'Type'    => 'text'
                )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }

    
    function _show() {
        global $tpl,$path;
        
        if(!$list = $this->getList($path->clean))
            $list = 0;

        $tpl->assign('news',$this->getPosts($list));
        
        if ( isset($this->page) && is_object($this->page))
            $this->page->show();
            
        $this->display('show.tpl');        
        
        if ( isset($this->page) && is_object($this->page))
            $this->page->show();
    }

    function getList($path) {
        $path = $this->escapeStr($path);
        
        if ($data = $this->fetchOne("SELECT value FROM news_settings WHERE setting='$path'"))
            return $data;
            
        return 1;
    }

    function getPosts($list) {
        
        if(!is_numeric($list))
            return false;
        

        if(class_exists('paging')) {
            $total = $this->fetchOne("SELECT count(*) FROM news WHERE list=$list");
            $page = core::load('paging',false,$total);
            $posts = $this->fetchAll("SELECT *,NOT FIND_IN_SET({$_SESSION['id']},new) AS new,new as readlist FROM news WHERE list=$list ORDER BY id DESC LIMIT {$page->start},{$page->limit}");
        } else {
            $posts = $this->fetchAll("SELECT *,NOT FIND_IN_SET({$_SESSION['id']},new) AS new,new as readlist FROM news WHERE list=$list ORDER BY id DESC");
        }

        foreach($posts as $key => $line) 
            if ($line['new'])
                $this->query("UPDATE news SET new='".trim($line['readlist'].','.$_SESSION['id'],',')."' WHERE id=".$line['id']);
        
        return $posts;

    }

    // {{{ secure
    public function secure ($text,$esc = true) {
        $tags = array('quote','br','em','strong','h1','h2','h3','h4','h5','h6','b','i','a','ul','li');
        $attr = array('user','post','href');
        $filter = new InputFilter($tags,$attr);
        $text = $filter->process($text);
        if ($esc)
            return $this->escapeStr($text);
        return $text;
    }
    // }}}
    function addPost($list,$head,$text,$access) {
        if(!is_numeric($list))
            return false;
        return $this->insert(
            array(
                'list' => $list,
                'head' => $this->secure($head,false),
                'text' => $this->secure($text,false),
                'access' => $this->escapeStr($access),
                'writer' => $_SESSION['id'],
                'timestamp' => date('Y-m-d H:i:s')
            ),
            'news'
        );
    }

    function editPost($id,array $data) {
            
        if(!is_numeric($id))
            return false;
            
        return $this->update(
            $data,
            'news',
            'WHERE id=$id'
        );
    }

    function removePost($id) {
            
        if(!is_numeric($id))
            return false;

        return $this->query(
            "DELETE FROM news WHERE id=$id"
        );
    }

}

?>
