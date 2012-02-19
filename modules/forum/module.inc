<?php

class forum extends module {
    // {{{ __construct
    public function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Forum');
        $this->setDescription('A powerful forum');
        $this->setVersion('1.3.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');
        $this->setDepends('pathadmin tinymce aculo');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }
    // }}}
    // {{{ install
    public function install() {
        $tables = array (
            'forums' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'group',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'desc',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'timestamp',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'topics',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'posts',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'last_post',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'last_poster',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'last_timestamp',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'read',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'write',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'moderator',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'new',
                    'Type'    => 'text'
                )
            ),
            'topics' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'forum',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'owner',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'created',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'new',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'tags',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'last_post',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'last_poster',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'last_timestamp',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'posts',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'sticky',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'lock',
                    'Type'    => "enum('Y', 'N')",
                    'Default' => 'N'
                ),
                array(
                    'Field'   => 'teamlock',
                    'Type'    => "enum('Y', 'N')",
                    'Default' => 'N'
                ),
                array(
                    'Field'   => 'banned',
                    'Type'    => 'text'
                )
            ),
            'posts' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'topic',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'post',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'timestamp',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'new',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'status',
                    'Type'    => "enum('active','removed','hidden')",
                    'Default' => 'active'
                ),
                array(
                    'Field'   => 'points',
                    'Type'    => 'int(10)'
                ),
                array (
                    'Field'   => 'voted',
                    'Type'    => 'text'
                )
            ),
            'abuse' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'ref',
                    'Type'    => 'varchar(255)'
                )
            )
        );
        $this->installTables($tables);

        $tcontent = array(
            'tree' => array(
                array(
                    'parent' => 0,
                    'path'   => 'forum',
                    'type'   => 'extend',
                    'module' => 'forum',
                    'vars'   => '_run'
                )
            )
        );
        
        $this->installTablesContent($tcontent);

        return true;
    }
    // }}}

    // {{{ _run
    public function _run () {
        
        global $tpl,$path;
        $v = new validate($_POST);
        
        if (isset($path->vars['path'][0])) 
            $path->vars['forum'] = $this->fetchSingle("SELECT * FROM forums WHERE head='".$this->escapeStr($path->vars['path'][0])."'");
        if (isset($path->vars['path'][1])) 
            $path->vars['topic'] = $this->fetchSingle("SELECT * FROM topics WHERE forum={$path->vars['forum']['id']} AND head='".$this->escapeStr($path->vars['path'][1])."'");
        

        // Topic level
            if ( isset($path->vars['topic']['id']) && isset($path->vars['forum']['id']) && ($forum1 = $this->getForumOnId($path->vars['forum']['id'])) && ($topic = $this->getTopicOnId($path->vars['topic']['id'])) ) {
                if (isset($_GET['post']))
                    if(!$post = $this->fetchSingle("SELECT * FROM posts WHERE posts.id=".db::escapeStr($_GET['post']))) {                    
                        send(E_USER_ERROR, 'Post not found');
                        return false;
                    }

                $tpl->assign('forum',$forum1);
                $tpl->assign('topic',$topic);
                
                // {{{ security check
                if ($topic['lock']=='Y')
                    $forum1['write'] = false;

                if ($topic['teamlock']=='Y') {
                    if(!($forum1['write']||$forum1['moderator'])) {
                        send(E_USER_ERROR,'Access denied!');
                        return false;
                    }
                }
                if ($topic['lock']=='Y') {
                    if(isset($_GET['action']))
                        if(!($forum1['moderator'])) {
                            send(E_USER_ERROR,'Access denied!');
                            return false;
                        }
                }
                
                if ( isset($_GET['action']) && $_GET['action']=='hide' && !($forum1['moderator']||$topic['own'])) {
                    send(E_USER_ERROR,'Access denied!');
                    return false;
                }

                if ( isset($_GET['action']) && $_GET['action']=='settings' && !($forum1['moderator']||$topic['own'])) {
                    send(E_USER_ERROR,'Access denied!');
                    return false;
                }

                if (!(isset($_GET['action']) && $_GET['action']=='edit'&&isset($post)&&$post['uid']==$_SESSION['id'])) {
                    if (isset($_GET['action']) && ($_GET['action']!='quote'&&$_GET['action']!='reply'&&$_GET['action']!='settings'&&$_GET['action']!='hide') && !$forum1['moderator']) {
                        send(E_USER_ERROR,'Access denied!');
                        return false;
                    }     

                    if (isset($_GET['action']) && (!$forum1['moderator'] && !$forum1['write'])) {
                        send(E_USER_ERROR,'Access denied!');
                        return false;
                    }
                }
                // }}}
                // {{{ Vote
                if (isset($_GET['vote'])&&is_numeric($_GET['vote'])) {
                    if ($forum1['write']||$forum1['moderator']) {
                        if($this->fetchOne("SELECT NOT FIND_IN_SET({$_SESSION['id']},voted) AS voted FROM posts WHERE id=".$_GET['vote'])&&!$voted) {
                            if ($_GET['dir']=='true')
                                $this->query("UPDATE posts SET points=points+1, voted = CONCAT(voted,',',{$_SESSION['id']}) WHERE id=".$_GET['vote']);
                            elseif ($_GET['dir']=='false')
                                $this->query("UPDATE posts SET points=points-1, voted = CONCAT(voted,',',{$_SESSION['id']}) WHERE id=".$_GET['vote']);
                            
                            $p = $this->fetchOne("SELECT points FROM posts WHERE id=".$_GET['vote']);
                            echo "$('vote_{$_GET['vote']}').innerHTML = '($p)';";
                        } else echo 'Failed';
                    } else echo 'Access denied!';
                    return false;
                }
                // }}}

                // {{{ Abuse 
                if (isset($_GET['abuse'])&&is_numeric($_GET['abuse'])) {
                        $this->query("INSERT INTO abuse SET uid={$_SESSION['id']},text='".$_POST->sql('text')."',ref='post:id=".db::escapeStr($_GET['abuse'])."'");
                        logg("Reported abuse post '{$_GET['abuse']}'");
                        echo "$('abuse_{$_GET['abuse']}').innerHTML = '[ <span style=\"color:#f00;\">Anmäld</span> ]';";
                    return false;
                }
                // }}}


                if ( isset($_GET['action']) && $_GET['action'] == 'remove') {
                    $this->removePost($post['id']);
                } elseif ( isset($_GET['action']) && $_GET['action'] == 'hide') {
                    self::hidePost($post['id']);
                } elseif ( isset($_GET['action']) && $_GET['action'] == 'unremove') {
                    $this->unremovePost($post['id']);
                } elseif ( isset($_GET['action']) && $_GET['action'] == 'edit') {
                    if (!isset($_POST['text']) ^ $v->ExistErrors()) {
                        $tpl->assign('action','edit');
                        
                        if (!isset($_POST['text'])&&isset($post)) 
                            $_POST['text'] = str_replace('<br />','',$post['text']);

                        $tpl->assign('vals',$_POST);
                        $tpl->assign('errors',$v->getAll());

                        $this->display('start.tpl.php');
                        $this->display('edit.tpl');
                        return false;
                    } elseif (!$v->ExistErrors()) {
                        $id = $this->addPost($path->vars['topic']['id'],$_POST['text'],$post['post']);
                        echo '<script language="javascript">location.href=\'?\';</script>';
                    }
                } elseif ( isset($_GET['action']) && $_GET['action'] == 'settings' ) {
                    $tpl->assign('action','settings');

                    if (isset($_POST['head'])) {
                        $v->length('head',3,255);
                        $v->numeric('sticky',1,1);

                        if ($_POST['head']!=$topic['head']&&$this->checkTopic($_POST['head'],$path->vars['forum']['id'])) 
                            $v->error['head'] = 'Rubriken är upptagen!';
                        
                    }

                    if (!isset($_POST['head']) ^ $v->ExistErrors()) {
                        if (!isset($_POST['head']))
                            $_POST = $topic;
                        $tpl->assign('vals',$_POST);
                        $tpl->assign('errors',$v->getAll());

                        $this->display('start.tpl.php');
                        $this->display('settings.topic.tpl');
                        return false;
                    } else {
                        if ($forum['moderator']) 
                            $save = array(
                                'head' => $_POST->nohtml('head',false),
                                'tags' => $_POST->nohtml('tags',false),
                                'lock' => $_POST['lock'],
                                'teamlock' => $_POST['teamlock'],
                                'sticky' => $_POST['sticky'],
                                'banned' => $_POST['banned1']
                            );
                        else
                            $save = array(
                                'head' => $_POST->nohtml('head',false),
                                'tags' => $_POST->nohtml('tags',false),
                                'lock' => $_POST['lock'],
                                'teamlock' => $_POST['teamlock'],
                                'sticky' => $_POST['sticky']
                            );

                            
                        $this->update($save,'topics','WHERE id='.$topic['id']);
                        echo '<script language="javascript">location.href=\'?\';</script>';
                    }
                } elseif ( isset($_GET['action']) && ($_GET['action'] == 'reply' || $_GET['action'] == 'quote') ) {
                    if (isset($_POST['text'])) 
                        $v->length('text',1);

                    if (!isset($_POST['text']) ^ $v->ExistErrors()) {
                        $tpl->assign('action','reply');
                        if (isset($_GET['post']) && is_numeric($_GET['post']) && !isset($_POST['quote'])) {
                            if (isset($post)) {
                                if (!isset($_POST['post'])||$_POST['post']=='')
                                    $_POST['text'] = "<quote post={$post['id']}>{$post['text']}</quote>";
                            }
                        }

                        $tpl->assign('vals',$_POST);
                        $tpl->assign('errors',$v->getAll());

                        $this->display('start.tpl.php');
                        $this->display('reply.tpl.php');
                        return false;
                    } elseif (!$v->ExistErrors()) {
                        $id = $this->addPost($path->vars['topic']['id'],$_POST['text']);
                        echo '<script language="javascript">location.href=\'?\';</script>';
                    }
                }   
                

                $tpl->assign('posts',$this->getPosts($path->vars['topic']['id'],0,100));
                $this->display('start.tpl.php');
                get();
                if ( isset($this->page) && is_object($this->page))
                    $this->page->show();
                
                $this->display('thread.tpl.php');
                
                if ( isset($this->page) && is_object($this->page))
                    $this->page->show();
                $this->display('start.tpl.php');

        // Forum level
            } elseif ( isset($path->vars['forum']['id']) && $forum1 = $this->getForumOnId($path->vars['forum']['id']) ) {

                // {{{ security check
                if (!$forum1['moderator'] && isset($_GET['action']) && $_GET['action']=='settings') {
                    send(E_USER_ERROR,'Access denied!');
                    return false;
                }
                // }}}


                if ( !isset($_GET['action']) && isset($_POST['head'])) {
                    $v->length('head',3,255);
                    $v->length('text',1);
                        
                    if ($this->checkTopic($_POST['head'],$path->vars['forum']['id'])) 
                        $v->error['head'] = 'Rubriken är upptagen!';
                    
                    if (!$v->ExistErrors()) {
                        $t = $this->addTopic($path->vars['forum']['id'],$_POST['head']);
                        $this->addPost($t,$_POST['text']);
                    }
                }
                
                if ( isset($_GET['action']) && $_GET['action'] == 'clear' ) {
                    $this->markAllTopicsAsRead($forum1['id']);
                    echo '<script language="javascript">location.href="/forum";</script>';
                }

                if ( isset($_GET['action']) && $_GET['action'] == 'settings' ) {
                    $tpl->assign('action','settings');

                    if (isset($_POST['head'])) {
                        $v->length('head',3,255);
                        $v->length('group',1,255);

                        if ($_POST['head']!=$forum1['head']&&$this->checkTopic($_POST['head'],$path->vars['forum']['id'])) 
                            $v->error['head'] = 'Rubriken är upptagen!';
                        
                    }

                    if (!isset($_POST['head']) ^ $v->ExistErrors()) {
                        if (!isset($_POST['head']))
                            $_POST = $this->fetchSingle("SELECT * FROM forums WHERE id=".$forum1['id']);
                        $tpl->assign('vals',$_POST);
                        $tpl->assign('errors',$v->getAll());

                        $this->display('start.tpl.php');
                        $this->display('settings.forum.tpl');
                        return false;
                    } else {
                        $save = array(
                            'group' => $_POST['group'],
                            'head' => $_POST->nohtml('head',false),
                            'desc' => $_POST->nohtml('desc',false),
                            'read' => $_POST['read'],
                            'write' => $_POST['write'],
                            'moderator' => $_POST['moderator']
                        );
                        $this->update($save,'forums','WHERE id='.$forum1['id']);
                    }
                } elseif ( ( isset($_GET['action']) && $_GET['action'] == 'new' ) || $v->ExistErrors() ) {
                    $tpl->assign('action','?action=save');
                    $tpl->assign('vals',$_POST);
                    $tpl->assign('errors',$v->getAll());
                    $this->display('start.tpl.php');
                    $this->display('newthread.tpl.php');
                    return false;
                }

                // List all topics
                $tpl->assign('forum',$forum1);
                $tpl->assign('topics',$this->getTopics($path->vars['forum']['id'],0,100));
                $this->display('start.tpl.php');

                if ( isset($this->page) && is_object($this->page))
                    $this->page->show();

                $this->display('forum.tpl.php');

                if ( isset($this->page) && is_object($this->page))
                    $this->page->show();

                $this->display('start.tpl.php');

        // Root - list all forums
            } else {
                if ( isset($_GET['action']) && $_GET['action'] == 'clear' ) {
                    $this->markAllAsRead();
                    echo '<script language="javascript">location.href="?";</script>';
                }

                $tpl->assign('forums',$this->getForums());
                $this->display('start.tpl.php');
                $this->display('list.tpl.php');
            }
    }
    // }}}
    
    // {{{ secure
    public function secure ($text,$esc = true) {
        $tags = array('quote','br','em','strong','h1','h2','h3','h4','h5','h6','b','i','a','ul','li','p','strike','pre');
        $attr = array('user','post','href','target');
        $filter = new InputFilter($tags,$attr);
        $text = $filter->process($text);
        if ($esc)
            return $this->escapeStr($text);
        return $text;
    }
    // }}}
    // {{{ getUserAcces
    public function getUserAccess() {
        global $path;

        return $path->getAccess();
    }
    // }}}
    // {{{ url
    public function url ($data) 
    {   
        return path::encode($data);
    }
    // }}}

    // {{{ getForums
    public function getForums () 
    {
        // Get the access groups
        if (!isset($this->groups)) $this->groups = $this->getUserAccess();
        // Get all the forums
        $data = $this->fetchAll(
            'SELECT forums.*,users.username AS last_poster, NOT FIND_IN_SET('.$_SESSION['id'].',new) AS new, '.
                "`read` REGEXP '.*({$this->groups}).*' AS `read`,".
                "`write` REGEXP '.*({$this->groups}).*' AS `write`,".
                "`moderator` REGEXP '.*({$this->groups}).*' AS `moderator` ".
            'FROM forums '.
            'LEFT JOIN users ON forums.last_poster=users.uid '.
            'HAVING `read` OR `write` OR `moderator`'. // Uncomment this line to not show forums that you dont have access to
            'ORDER BY `group`,head'
        );
        
        if (!(isset($data )&&is_array($data)))
            return false;

        foreach ($data as $key => $line) {
            $data[$key]['url'] = $this->url($line['head']);
            $data[$key]['head'] = htmlspecialchars($line['head']);
            $data[$key]['group'] = htmlspecialchars($line['group']);
            if($line['new']&&!$line['write']&&!$line['moderator']) {
                $data[$key]['new'] = 
                    ($this->fetchAll(
                        "SELECT * 
                        FROM topics 
                        WHERE forum = {$line['id']} AND 
                            teamlock = 'N' AND 
                            NOT (`banned` REGEXP '.*({$this->groups}).*') AND 
                            NOT FIND_IN_SET({$_SESSION['id']},new)"
                    ));

            }
        }

        return $data;
    }
    // }}}
    // {{{ getForumOnId
    public function getForumOnId ($id) 
    {
        // Get the access groups
        if (!isset($this->groups)) $this->groups = $this->getUserAccess();

        // Get all the forums
        if ($data = $this->fetchSingle(
                'SELECT forums.*,users.username AS last_poster,new as readlist, NOT FIND_IN_SET('.$_SESSION['id'].',new) AS new, '.
                    "`read` REGEXP '.*({$this->groups}).*' AS `read`,".
                    "`write` REGEXP '.*({$this->groups}).*' AS `write`,".
                    "`moderator` REGEXP '.*({$this->groups}).*' AS `moderator` ".
                'FROM forums '.
                'LEFT JOIN users ON forums.last_poster=users.uid '.
                'WHERE forums.id='.$id.' '.
                'HAVING `read` OR `write` OR `moderator`'.
                'ORDER BY `group`,head'
            )) {
            $data['url'] = $this->url($data['head']);
            $data['head'] = htmlspecialchars($data['head']);
            $data['group'] = htmlspecialchars($data['group']);

            //if ($data['new'])
            //    $this->query("UPDATE forums SET new='".trim($data['readlist'].','.$_SESSION['id'],',')."' WHERE id=".$data['id']);

            return $data;
        } else return false;
    }
    // }}}
    // {{{ createForum
    public function createForum($head,$group,$desc,$read,$write,$moderator)
    {
        // Secure the text
        $head  = $this->escapeStr($head);
        $group = $this->escapeStr($group);
        $desc  = $this->secure($desc);
        $read  = $this->escapeStr($read);
        $write = $this->escapeStr($write);
        $moderator = $this->escapeStr($moderator);

        // Add the new post
        $this->query(
            'INSERT INTO forums SET '.
                'head  = \''.$head.'\', '.
                '`group`  = \''.$group.'\', '.
                '`desc`  = \''.$desc.'\', '.
                '`read`  = \''.$read.'\', '.
                '`write`  = \''.$write.'\', '.
                'moderator  = \''.$moderator.'\', '.
                'timestamp  = now() '
        );

        logg("Created forum '".mysql_insert_id()."'");
    
    }
    // }}}
    // {{{ markForumAsRead
    public function markForumAsRead($forum) {
        $new = $this->fetchOne("SELECT `new` FROM forums WHERE id=$forum");
        $this->query("UPDATE forums SET new='".trim($new.','.$_SESSION['id'],',')."' WHERE id=$forum");
    }
    // }}}
    // {{{ markAllAsRead
    public function markAllAsRead() {
        /*if ($data = $this->fetchAll("SELECT `new`,id FROM forums WHERE NOT FIND_IN_SET({$_SESSION['id']},new)"))
            foreach ($data as $line) 
                $this->query("UPDATE forums SET new='".trim($line['new'].','.$_SESSION['id'],',')."' WHERE id={$line['id']}");
        
        if ($data = $this->fetchAll("SELECT `new`,id FROM topics WHERE NOT FIND_IN_SET({$_SESSION['id']},new)"))
            foreach ($data as $line) 
                $this->query("UPDATE topics SET new='".trim($line['new'].','.$_SESSION['id'],',')."' WHERE id={$line['id']}");

        if ($data = $this->fetchAll("SELECT `new`,id FROM posts WHERE NOT FIND_IN_SET({$_SESSION['id']},new)"))
            foreach ($data as $line) 
                $this->query("UPDATE posts SET new='".trim($line['new'].','.$_SESSION['id'],',')."' WHERE id={$line['id']}"); */

        $this->query("UPDATE forums SET new = trim(',' FROM concat(new,',{$_SESSION['id']}')) WHERE NOT FIND_IN_SET({$_SESSION['id']},new)");
        $this->query("UPDATE topics SET new = trim(',' FROM concat(new,',{$_SESSION['id']}')) WHERE NOT FIND_IN_SET({$_SESSION['id']},new)");
        $this->query("UPDATE posts SET new = trim(',' FROM concat(new,',{$_SESSION['id']}')) WHERE NOT FIND_IN_SET({$_SESSION['id']},new)");
    }
    // }}}

    // {{{ getTopics
    public function getTopics ($forum,$from,$amount) 
    {
        // Safety check
        if (!is_numeric($forum)) return false;

        // Get the access groups
        if (!isset($this->groups)) $this->groups = $this->getUserAccess();

        if(class_exists('paging')&&$this->page = core::load('paging',false)) {
            if(!$all = $this->fetchAllOne("SELECT NOT FIND_IN_SET({$_SESSION['id']},new) AS new FROM topics WHERE forum = $forum ORDER BY sticky DESC,last_timestamp DESC")) {
                $this->page->make( 0 );
                return false;
            }

            $this->page->limit = 20;
            if( !$this->page->make( count($all) ) )
                return false;
            
            // Find the first unread
            foreach ($all as $key => $line) {
                if ($line) {
                    $this->page->startpage(intval($key/$this->page->limit));
                    break;
                }
            }
            
            $data = $this->fetchAll(
                'SELECT topics.*,last.username AS last_poster,owner.username AS owner,'.
                    "`banned` REGEXP '.*({$this->groups}).*' AS `banned`, ".
                    "(topics.owner = {$_SESSION['id']}) as own, ".
                    "NOT FIND_IN_SET({$_SESSION['id']},new) AS new ".
                'FROM topics '.
                'LEFT JOIN users AS owner ON topics.owner=owner.uid '.
                'LEFT JOIN users AS last ON topics.last_poster=last.uid '.
                'WHERE forum = '.$forum.' '.
                'HAVING NOT `banned`'.
                'ORDER BY sticky DESC,last_timestamp DESC '.
                "LIMIT {$this->page->start},{$this->page->limit}"
            );
        } else {
            $data = $this->fetchAll(
                'SELECT topics.*,last.username AS last_poster,owner.username AS owner,'.
                    "`banned` REGEXP '.*({$this->groups}).*' AS `banned`, ".
                    "(topics.owner = {$_SESSION['id']}) as own, ".
                    "NOT FIND_IN_SET({$_SESSION['id']},new) AS new ".
                'FROM topics '.
                'LEFT JOIN users AS owner ON topics.owner=owner.uid '.
                'LEFT JOIN users AS last ON topics.last_poster=last.uid '.
                'WHERE forum = '.$forum.' '.
                'HAVING NOT `banned`'.
                'ORDER BY sticky DESC,last_timestamp DESC '.
                'LIMIT '.$from.','.$amount
            );
        }

        // Get the topics
        if (isset($data)&&is_array($data)) {
        
            foreach ($data as $key => $line) {
                $data[$key]['url'] = $this->url($line['head']);
                $data[$key]['head'] = htmlspecialchars($line['head']);
            }

            return $data;
        } else return false;
    }
    // }}}
    // {{{ getTopicOnId
    public function getTopicOnId ($topic) 
    {
        // Safety check
        if (!is_numeric($topic)) return false;

        // Get the access groups
        if (!isset($this->groups)) $this->groups = $this->getUserAccess();

        // Get the topics
        if($data = $this->fetchSingle(
                'SELECT 
                    topics.*,
                    new as readlist,
                    NOT FIND_IN_SET('.$_SESSION['id'].',new) as new,
                    last.username AS last_poster,
                    (topics.owner = '.$_SESSION['id'].') as own,
                    owner.username AS owner,'.
                    "banned AS `banned1`,
                    `banned` REGEXP '.*({$this->groups}).*' AS `banned` ".
                'FROM topics '.
                'LEFT JOIN users AS owner ON topics.owner=owner.uid '.
                'LEFT JOIN users AS last ON topics.last_poster=last.uid '.
                'WHERE topics.id = '.$topic.' '.
                'HAVING NOT `banned`'.
                'ORDER BY sticky DESC '.
                'LIMIT 1'
            )) {
            $data['url'] = $this->url($data['head']);
            $data['head'] = htmlspecialchars($data['head']);
            
            //if ($data['new'])
            //    $this->query("UPDATE topics SET new='".trim($data['readlist'].','.$_SESSION['id'],',')."' WHERE id=".$data['id']);

            return $data;
        } else return false;
    }
    // }}}
    // {{{ addTopic
    public function addTopic($forum,$head) 
    {
        // Safety check
        if (!is_numeric($forum)) return false;
        
        // Secure the text
        $head = $this->secure($head);

        // Add the new post
        $this->query(
            'INSERT INTO topics SET '.
                'forum = '.$forum.', '.
                'owner = '.$_SESSION['id'].', '.
                'head  = \''.$head.'\', '.
                'created = now(), '.
                'new = \''.$_SESSION['id'].'\', '.
                'last_timestamp = now() '
        );

        $id = mysql_insert_id();

        // Update the topic count
        $this->query(
            'UPDATE forums '.
            'SET '.
                'topics = topics+1, '.
                'new = \'\', '.
                'last_poster = '.$_SESSION['id'].', '.
                'last_timestamp = NOW() '.
            'WHERE id='.$forum.' '.
            'LIMIT 1'
        );
        
        logg("Added forum topic '$id'");

        return $id;
    }
    // }}}
    // {{{ markTopicAsRead
    public function markTopicAsRead($topic) {
        if(!$topic = $this->fetchSingle("SELECT new,forum,id FROM topics WHERE id=$topic"))
            return false;

        $this->query("UPDATE topics SET new='".trim($topic['new'].','.$_SESSION['id'],',')."' WHERE id={$topic['id']}");

        // If all the forum topics are read
        if(!$this->fetchAll("SELECT id FROM topics WHERE forum={$topic['forum']} AND NOT FIND_IN_SET({$_SESSION['id']},new)") ) 
            $this->markForumAsRead($topic['forum']);
    }
    // }}}
    // {{{ markAllTopicsAsRead
    /**
     * If $forum=0 then all forums are being marked as read.. else only the defined forum will be marked
     *
     */
    public function markAllTopicsAsRead($forum = 0) {
        if($forum != 0) {
            if(!$topics = $this->fetchAllOne("SELECT id FROM topics WHERE forum={$forum} AND NOT FIND_IN_SET({$_SESSION['id']},new)") )  
                return false;
        } else {
            if(!$topics = $this->fetchAllOne("SELECT id FROM topics WHERE NOT FIND_IN_SET({$_SESSION['id']},new)") )  
                return false;
        }
            

        foreach ($topics as $line) {
            $this->markTopicAsRead($line);
            $this->markAllPostsAsRead($line);
        }
    }
    // }}}

    // {{{ getPosts
    public function getPosts ($topic) 
    {   
        // Safety check
        if (!is_numeric($topic)) return false;

        $active_posts = $this->fetchAllOne("SELECT max(id) FROM posts WHERE topic = $topic GROUP BY post");

        if(class_exists('paging')&&$this->page = core::load('paging',false)) {
            $all = $this->fetchAllOne("SELECT NOT FIND_IN_SET({$_SESSION['id']},new) AS new FROM posts WHERE topic = $topic AND id IN (".implode($active_posts,',').") GROUP BY post ORDER BY post,id DESC");

            $this->page->limit = 15;

            if( !$this->page->make( count($all) ) )
                return false;
            
            // Find the first unread
            $success = false;
            foreach ($all as $key => $line) {
                if ($line) {
                    $this->page->startpage(intval($key/$this->page->limit));
                    $success = true;
                    break;
                }
            }   

            if (!$success)
                $this->page->startpage('last');
                    
            // Get the posts
            $data = $this->fetchAll(
                'SELECT posts.*,'.
                    '1 as version,
                    users.username as user,
                    users.firstname,
                    users.lastname, 
                    NOT FIND_IN_SET('.$_SESSION['id'].',new) AS new,
                    new as readlist, '.
                    "FIND_IN_SET({$_SESSION['id']},voted) AS voted ".
                'FROM posts '.
                'LEFT JOIN users ON posts.uid=users.uid '.
                'WHERE topic = '.$topic.' AND id IN ('.implode($active_posts,',').') '.
                'ORDER BY post '.
                "LIMIT {$this->page->start},{$this->page->limit}"
            );
            

        } else {
            // Get the posts
            $data = $this->fetchAll(
                'SELECT posts.*,'.
                    'count(posts.id) AS version ,
                    users.username as user,
                    users.firstname,
                    users.lastname, '.
                    'NOT FIND_IN_SET('.$_SESSION['id'].',new) AS new,
                    new as readlist, '.
                    "FIND_IN_SET({$_SESSION['id']},voted) AS voted ".
                'FROM posts '.
                'LEFT JOIN users ON posts.uid=users.uid '.
                'WHERE topic = '.$topic.' '.
                'GROUP BY post '.
                'ORDER BY post,posts.id DESC '
            );
        }

        $last = $this->fetchOne(
            'SELECT post '.
            'FROM posts '.
            'WHERE topic = '.$topic.' '.
            'GROUP BY post '.
            'ORDER BY post DESC,posts.id DESC LIMIT 1'
        );
        

        if (isset($data)&&is_array($data)) {
            foreach ($data as $key => $line) {
                // Mark unread posts as read
                if ($line['new'])
                    $this->query("UPDATE posts SET new='".trim($line['readlist'].','.$_SESSION['id'],',')."' WHERE id=".$line['id']);
                
                if($last==$line['post']) 
                    $this->markTopicAsRead($topic);
                    
                // Process the text
                $data[$key]['text'] = $this->text($line['text']);
                $data[$key]['picture'] = $this->fetchOne("SELECT picture FROM users WHERE uid=".$data[$key]['uid']);
                if ($data[$key]['picture']>'') {
                    if (strlen($data[$key]['picture'])==32) {
                        $data[$key]['picture'] = '/images/users/thumbs/small_'.$data[$key]['picture'].'.jpg';
                    } else {
                        $data[$key]['picture'] = '/images/users/old/'.$data[$key]['picture'];
                    }
                }
            }

            return $data;
        } else return false;
    }
    // }}}
    // {{{ addPost
    public function addPost($topic,$text,$post = 0) 
    {
        // Safety check
        if (!is_numeric($topic)) return false;
        
        // Get the highest postnumber
        if ($post==0) {
            if ($post = $this->fetchOne('SELECT MAX(post) FROM posts WHERE topic ='.$topic)) {
                $post++;
            } else {
                $post = 1;
            }
            $writer = $_SESSION['id'];
            $new = 'new = \'\', ';
        } else {
            $writer = $this->fetchOne('SELECT uid FROM posts WHERE topic ='.$topic.' AND post='.$post);
            $new = '';
        }
    
        // Secure the text
        $text = $this->secure($text);

        if ($this->fetchOne("SELECT * FROM posts WHERE topic = $topic AND text  = '$text' AND uid   = {$_SESSION['id']}")) {
            send(E_USER_ERROR,'Inlägg spärrat! Det finns redan ett inlägg av dig med exakt samma innehåll.');
            return false;
        }

        // Add the new post
        $this->query(
            'INSERT INTO posts SET '.
                'topic = '.$topic.', '.
                'post  = '.$post.', '.
                'text  = \''.$text.'\', '.
                'uid   = '.$writer.', '.
                'timestamp  = now() '
        );

        $id = mysql_insert_id();
        $topic_posts = $this->fetchOne('SELECT count(*) FROM posts WHERE topic='.$topic);

        // Update the topic post count
        $this->query(
            'UPDATE topics '.
            'SET '.
                "posts = $topic_posts, ".
                $new.
                'last_poster = '.$writer.', '.
                'last_timestamp = NOW() '.
            'WHERE id='.$topic.' '.
            'LIMIT 1'
        );

        // Update the forum post count
        $forum = $this->fetchOne('SELECT forum FROM topics WHERE id='.$topic);
        $forum_posts = $this->fetchOne('SELECT count(*) FROM topics,posts WHERE posts.topic=topics.id AND forum='.$forum);

        $this->query(
            'UPDATE forums '.
            'SET '.
                "posts = $forum_posts, ".
                $new.
                'last_poster = '.$writer.', '.
                'last_timestamp = NOW() '.
            'WHERE id='.$forum.' '.
            'LIMIT 1'
        );
        
        logg("Added forum post '$id'");

        return $id;
    }
    // }}}
    // {{{ removePost
    function removePost($id) {
        if (!is_numeric($id))
            return false;
        
        if(!$this->query("UPDATE posts SET status='removed' WHERE id=$id"))
            return false;
        
        logg("Removed forum post '$id'");
    }   
    // }}}
    // {{{ hidePost
    function hidePost($id) {
        if (!is_numeric($id))
            return false;
        
        if(!$this->query("UPDATE posts SET status='hidden' WHERE id=$id"))
            return false;
        
        logg("Hide forum post '$id'");
    }   
    // }}}
    // {{{ unremovePost
    function unremovePost($id) {
        if (!is_numeric($id))
            return false;
        
        if(!$this->query("UPDATE posts SET status='active' WHERE id=$id"))
            return false;
        
        logg("Unremoved forum post '$id'");
    }   
    // }}}
    // {{{ markAllPostsAsRead
    /**
     * If topic = 0 then all topics are being marked as read.. else only the defined topic will be marked
     *
     */
    public function markAllPostsAsRead($topic = 0) {
        
        if($topic != 0) {
            if(!$posts = $this->fetchAll("SELECT id,new FROM posts WHERE topics={$topic} AND NOT FIND_IN_SET({$_SESSION['id']},new)") )  
                return false;
        } else 
            if(!$posts = $this->fetchAll("SELECT id,new FROM posts WHERE NOT FIND_IN_SET({$_SESSION['id']},new)") )  
                return false;

        foreach ($posts as $line) 
            $this->query("UPDATE topics SET new='".trim($posts['new'].','.$_SESSION['id'],',')."' WHERE id={$posts['id']}");

    }
    // }}}
    // {{{  checkTopic
    public function checkTopic ($topic,$forum)
    {
        $topic = $this->escapeStr($topic);
        if ($this->fetchOne("SELECT id FROM topics WHERE head='$topic' AND forum='$forum'")) 
            return true;
        return false;
    }
    // }}}
    // {{{ text
    function text ($data) 
    {

        //$data = preg_replace_callback('$\[(/?)([a-zA-Z0-9]+)\]$','quote',$data);
		
		//aif ( $_SESSION['id'] == 635 )
		//	$data = htmlspecialchars($data);

        global $taggs;
        $taggs = array();

		$data = htmlFilter($data);
		    //$data = preg_replace_callback('$<(/?)([a-zA-Z]+)([^>]*)>$','quote',$data);
        

        foreach ($taggs as $key => $line) 
            if($line > 0)
                $data .= str_repeat('</'.$key.'>',$line);
		
		if ( preg_match_all("/.*((http:\/\/)(\S*)\.(\S*)).*/i",$data,$matches) ) {
			$replace = array();

			foreach ($matches[1] as $key => $line) {
				$replace[$line] = '<a href="'.$line.'" style="color:#00c;" target="_blank">'.$line.'</a>';
			}

			if (count($replace) > 0 )
				$data = str_replace( array_flip($replace),$replace,$data);
		}

        $data = nl2br($data);
        return $data;
    }
    // }}}
}

// {{{ quote
function quote ($data) 
{
    global $db,$taggs;
	//if ( $_SESSION['id'] == 635 )
	//	core::printArr($data);

    // If it is an open or a close tag


    preg_match_all('$([^=]+)=(\"|\'?)([^\=\'\"]+)(\"|\'?)[^\s]$i',$data[3],$match);

	if ( $data[2] == '3' )
		core::printarr($match);

    foreach ($match[1] as $key => $line) {
        if (!isset($attr)) $attr = array();
        $attr[trim($line)] = trim($match[3][$key]);
    }
    
    if ($data[1]!='/') {
        switch (strToLower($data[2])) {
            case 'quote':
                if (!isset($taggs['div']))
                    $taggs['div'] = 0;
                $taggs['div'] += 2;
                $str = '<div style="font-size:10px;">';
                if (isset($attr)) {
                    
                    if (in_array('user',array_keys($attr))) {
                        $str .= '<b><a href="/users/'.path::encode($attr['user']).'.htm">'.$attr['user'].'</a> ';
                        if (isset($attr['time'])&& $time = strtotime($attr['time'])) $str .= '('.timestamp($attr['time']).') ';
                        $str .= 'skrev:</b>';
                    } elseif (in_array('post',array_keys($attr))) {
                        if (is_numeric($attr['post']) && $line = $db->fetchSingle("SELECT username,timestamp FROM posts LEFT JOIN users USING (uid) WHERE posts.id=".$attr['post'])) {
                            $str .= '<b>Ursprungligen skrivet av <a href="/users/'.path::encode($line['username']).'.htm">'.$line['username'].'</a> ('.timestamp($line['timestamp']).'):</b>';
                        }
                    } else 
                        $str .= '<b>citat</b>';
                } else {
                    $str .= '<b>citat</b>';
                }
                return $str . '<div style="border:1px solid #fff;padding:10px;margin-top:3px;background:#ccc">';
            default:
                if (!isset($taggs[strToLower($data[2])]))
                    $taggs[strToLower($data[2])] = 0;
                $taggs[strToLower($data[2])] += 1;
        }
    } else {
        switch (strToLower($data[2])) {
            case 'quote':
                if (isset($taggs['div'])) {
                    $taggs['div'] -= 2;
                    return '</div></div>';
                } else return '';
            default:
                if (isset($taggs[strToLower($data[2])])) {
                    $taggs[strToLower($data[2])] -= 1;
                } else return '';
        }       
    }

    return $data[0];
}
// }}}


function htmlFilter( $value ) {

    $allowed = array(
        "a" => array(
          "href" => array(
            "/^http:.*$/",
            "/^https:.*$/",
            "/^mailto:.*$/"
          )
        ),
        "p" => array(),
        "br" => array(),
        "em" => array(),
        "strong" => array(),
        "u" => array(),
        "strike" => array(),
        "div" => array(
          "class" => array(
            "/.*/"
          ),
          "style" => array(
            "/.*/"
          ),
          "align" => array(
            "/^left$/",
            "/^center$/",
            "/^right$/",
            "/^justify$/"
          )
        ),
        "span" => array(
          "class" => array(
            "/.*/"
          ),
          "style" => array(
            "/.*/"
          )
        ),
        "img" => array(
          "src" => array(
            "/^http:.*$/",
            "/^https:.*$/"
          ),
          "alt" => array(
            "/.*/"
          ),
          "style" => array(
            "/.*/"
          )
        ),
        "ul" => array(),
        "li" => array(),
        "ol" => array(),
        "table" => array(),
        "tr" => array(),
        "td" => array(),
        "thead" => array(),
        "tbody" => array(),
		'b' => array(),
		'i' => array(),
      ); // quote är fulaccepterad, skall inte vara med i denna lista.


    $nohtml = '';
    $outp = '';

    $tagstack = array();

    $arr = preg_split( '/<([^<>]+)>/U', $value, -1, PREG_SPLIT_DELIM_CAPTURE );
    while( count( $arr ) > 0 ) {
        $content = array_shift( $arr );
        $tag = array_shift( $arr );

        // Recode content for unencoded characters.
        $outp .= htmlentities(
                html_entity_decode(
                    $content,
                    ENT_QUOTES, 'UTF-8'),
                ENT_QUOTES, 'UTF-8');

        // Export plain text
        $nohtml .= trim($content);

        if($tag !== NULL) { // Filter tag
            if($tag[0] == '/') { // Endtag
                $tagname = strtolower(trim(substr($tag, 1)));
                if(count($tagstack)>0) {
                    $testtag = array_pop($tagstack);
                    if($tagname == $testtag) {
                        if( $tagname == 'quote' ) {


                            /* Quote end-tag start */
                            $outp .= '</div></div>';
                            /* Quote end-tag end */

                        } else {
                            $outp .= '</'.$tagname.'>';
                        }
                    } else {
                        $outp .= htmlentities('<'.$tag.'>', ENT_QUOTES, 'UTF-8');
                        array_push($tagstack, $testtag);
                    }
                } else {
                    $outp .= htmlentities('<'.$tag.'>', ENT_QUOTES, 'UTF-8');
                }
            } else { // Starttag
                $lastchar = substr($tag,-1);
                if($lastchar == '/') {
                    $tag = substr($tag,0,-1);
                    $endtag = false;
                } else {
                    $endtag = true;
                }

                list($tagname,$params) = array_merge(
                        explode(' ',trim($tag), 2),
                        array('')
                        );
                $tagname = strtolower($tagname);
                $params = ' '.$params.' ';

                if($tagname == 'quote') {

                    /* quote start-tag... ugly fix... */

                    // $params contains the parameter string. has to be parsed... watch below

					preg_match_all('$([^=]+)=(\"|\'?)([^\=\'\"]+)(\"|\'?)[^\s]$i',$params,$match);
					$attr = array();
					foreach ($match[1] as $key => $line) {
						if (!isset($attr)) $attr = array();
						$attr[trim($line)] = trim($match[3][$key]);
					}

					$str = '<div style="font-size:10px;">';
					if (isset($attr)) {
						
						if (in_array('user',array_keys($attr))) {
							$str .= '<b><a href="/users/'.path::encode($attr['user']).'.htm">'.$attr['user'].'</a> ';
							if (isset($attr['time'])&& $time = strtotime($attr['time'])) $str .= '('.timestamp($attr['time']).') ';
							$str .= 'skrev:</b>';
						} elseif (in_array('post',array_keys($attr))) {
							if (is_numeric($attr['post']) && $line = db::fetchSingle("SELECT username,timestamp FROM posts LEFT JOIN users USING (uid) WHERE posts.id=".$attr['post'])) {
								$str .= '<b>Ursprungligen skrivet av <a href="/users/'.path::encode($line['username']).'.htm">'.$line['username'].'</a> ('.timestamp($line['timestamp']).'):</b>';
							}
						} else 
							$str .= '<b>citat</b>';
					} else {
						$str .= '<b>citat</b>';
					}
					$outp .= $str.'<div style="border:1px solid #fff;padding:10px;margin-top:3px;background:#ccc">';

                    array_push( $tagstack, $tagname );
                    /* End of quote start-tag */

                } else if(!array_key_exists($tagname, $allowed)) {
                    $outp .= htmlentities( '<'.$tag.'>', ENT_QUOTES, 'UTF-8' );
                } else {

                    $parallow = $allowed[$tagname];

                    // Parse parameter-string
                    $outparams = array();
                    $paramarr = array();
                    preg_match_all('/ ([a-zA-Z]+)="([^"]*)"/U',$params,$paramarr,PREG_SET_ORDER);
                    foreach($paramarr as $pars) {
                        list( $all, $k, $v ) = $pars;
                        $outparams[strtolower($k)] = $v;
                    }

                    // Build tag
                    $outtag = $tagname;
                    foreach($outparams as $k => $v) {
                        // Filter tags
                        if(array_key_exists($k, $parallow)) {
                            $matches = $parallow[$k];
                            $ok = false;
                            foreach($matches as $re) {
                                if(preg_match($re, $v)) {
                                    $ok = true;
                                    break;
                                }
                            }
                            if($ok) {
                                $outtag .= ' '.$k.'="'.$v.'"';
                            }
                        }
                    }
                    if($endtag) {
                        array_push($tagstack, $tagname);
                    } else {
                        $outtag .= ' /';
                    }
                    $outp .= '<'.$outtag.'>';
                }
            }
        }
    }

    while(count($tagstack)>0) {
        $tagname = array_pop($tagstack);
        if( $tagname == 'quote' ) {


            /* Quote end-tag start */
            $outp .= '</div></div>';
            /* Quote end-tag end */

        } else {
            $outp .= '</'.$tagname.'>';
        }
    }

    /* Test if result is ok.
       $outp contains an filtered and safe html-string.
       $nohtml contains an filtered and stripped txt-string.
       */

    return $outp;
}

?>
