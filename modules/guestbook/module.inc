<?php

class guestbook extends module {

    function guestbook ($tot = 0) {
        $this->setId('guestbook');
        $this->setName('Guestbook');
        $this->setDescription('Creates a personal guestbook for each user');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('path');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install () 
    {
       
        $tables = array (
            'guestbook' => array(
                array(
                    'Field'   => 'postid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'gbid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'from',
                    'Type'    => 'int(11)'
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
                    'Type'    => "enum('new','read','answerd')",
                    'Default' => 'new'
                )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }

    
    function _show() {
        global $tpl,$path;
        
        $tpl->assign('owner',$_SESSION['id']==$path->vars['uid']);
        $tpl->assign('username',$this->fetchOne("SELECT username FROM users WHERE uid=".$path->vars['uid']));
        if (isset($_POST['message'])&&$_POST['message']!='') {
            if (isset($_POST['post'])&&is_numeric($_POST['post'])) {
                if ($data = $this->fetchSingle("SELECT `from`,timestamp FROM guestbook WHERE postid={$_POST['post']}")) {
                    $this->query("UPDATE guestbook SET new='answerd',timestamp='{$data['timestamp']}' WHERE postid={$_POST['post']}");
                    $this->fetchPost($data['from']);
                } else
                    send(E_WARNING,'Misslyckades med att spara meddelandet!');
                    
            } else
                $this->fetchPost();
        }

        if (!isset($path->vars['uid'])) {
            send(E_USER_ERROR,"No user id is defined!");
            return false;
        }
        
        $dialog = NULL;
        if(isset($_GET['dialog'])&&is_numeric($_GET['dialog']))
            $dialog = $_GET['dialog'];
        
        if ($posts = $this->getPosts($path->vars['uid'],$dialog)) {
            $tpl->assign('gbid',$path->vars['uid']);
            $tpl->assign('guestbook',$posts);
        }
        if ( isset($this->page) && is_object($this->page))
            $this->page->show();

        $this->display('show.tpl');

        if ( isset($this->page) && is_object($this->page))
            $this->page->show();
    }

    function fetchPost($uid = NULL) {
        global $path,$tpl;

        if (!isset($_POST['message'])||$_POST['message']=='') {
            $this->display('fail.tpl');
            return false;
        }
        
        if ($uid == NULL)
            $uid = $path->vars['uid'];

        if (!is_numeric($uid)) {
            $this->display('fail.tpl');
            echo "'{$uid}'";
            return false;
        }
            
        if(!$this->insert(
                array(
                    'gbid' => $uid,
                    'from' => $_SESSION['id'],
                    'text' => htmlspecialchars($_POST['message']),
                    'new'  => 'new',
                    'timestamp' => date('Y-m-d H:i:s')
                ),
                'guestbook'
            )
        ) {
            $tpl->display('fail.tpl');
            return false;
        }
    }

    function getPosts($gbid,$gbid2 = null) {
        
        if(!is_numeric($gbid))
            return false;

        if ($gbid2 != null) {
            if(!is_numeric($gbid))
                return false;
            
            $where = "WHERE (gbid=$gbid AND `from`=$gbid2) OR (gbid=$gbid2 AND `from`=$gbid)";
        } else
             $where = "WHERE gbid=$gbid";
        
        if(class_exists('paging')&&$this->page = core::load('paging',false)) {
            if(!$this->page->make($this->fetchOne("SELECT count(*) FROM guestbook WHERE gbid=$gbid")))
                return false;

            $posts = $this->fetchAll("SELECT *,(`gbid` = '{$_SESSION['id']}') as owner FROM guestbook $where ORDER BY postid DESC LIMIT {$this->page->start},{$this->page->limit}");
        } else {
            $posts = $this->fetchAll("SELECT *,(`gbid` = '{$_SESSION['id']}') as owner FROM guestbook $where ORDER BY postid DESC");
        }
        
        if (isset($posts)&&is_array($posts))
            foreach ($posts as $key => $line) {

                $posts[$key]['user'] = user::short($posts[$key]['from']);

                if($line['owner']&&$line['new']=='new')
                    $this->query("UPDATE guestbook SET new='read' WHERE postid={$line['postid']} LIMIT 1");
            }   

        return $posts;

    }

    function addPost($gbid,$from,$text) {
        
        if(!is_numeric($gbid))
            return false;

        return $this->insert(
            array(
                'gbid' => $gbid,
                'from' => $from,
                'text' => $text,
                'timestamp' => date('Y-m-d H:i:s')
            ),
            'guestbook'
        );
    }

    function editPost($postid,$newtext) {
            
        if(!is_numeric($postid))
            return false;

        return $this->update(
            array(
                'text' => $text
            ),
            'guestbook',
            'WHERE postid=$postid'
        );
    }

    function removePost($postid) {
            
        if(!is_numeric($postid))
            return false;

        return $this->query(
            "DELETE FROM guestbook WHERE postid=$postid"
        );
    }

}

?>
