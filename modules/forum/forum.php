<?php

require_once ('modules/forum.class.php');
require_once('include/validate.class.inc');


$forum = new forum();
$v = new Validate($_POST);

// Topic level
    if ( isset($path->topic) && isset($path->forum) && ($forum1 = $forum->getForumOnId($path->forum)) && ($topic = $forum->getTopicOnId($path->topic)) ) {
        $tpl->assign('forum',$forum1);
        $tpl->assign('topic',$topic);

        if (isset($_POST['text'])) {
            $v->length('text',3);

            if (!$v->ExistErrors()) {
                $id = $forum->addPost($path->topic,$_POST['text']);
                if (isset($_POST['post'])) 
                    $forum->addQuote($id, $_POST['post'], $_POST['text']);
            }
        }

        if ( ( isset($_GET['action']) && ($_GET['action'] == 'reply' || $_GET['action'] == 'quote') ) && ($forum1) || $v->ExistErrors() ) {
            $tpl->assign('action','reply');
            if (isset($_GET['post']) && is_numeric($_GET['post']) && !isset($_POST['quote'])) {
                if ($data = $db->fetchSingel("SELECT posts.id,text,username FROM posts JOIN users ON posts.uid=users.id WHERE posts.id=".$_GET['post'])) {
                    $_POST['quote'] = $data['text'];
                    $_POST['user'] = $data['username'];
                    $_POST['post'] = $data['id'];
                }
            }

            $tpl->assign('vals',$_POST);
            $tpl->assign('errors',$v->getAll());

            $tpl->display('forum/start.tpl.php');
            $tpl->display('forum/reply.tpl.php');
        } else {
            $tpl->assign('posts',$forum->getPosts($path->topic,0,100));
            $tpl->display('forum/start.tpl.php');
            $tpl->display('forum/thread.tpl.php');
        }

// Forum level
    } elseif ( isset($path->forum) && $forum1 = $forum->getForumOnId($path->forum) ) {

        if (isset($_POST['head'])) {
            $v->length('head',3,255);
            $v->length('text',3);
                
            if ($forum->checkTopic($_POST['head'],$path->forum)) {
                $v->error['head'] = 'Rubriken är upptagen!';
            }
            
            if (!$v->ExistErrors()) {
                $t = $forum->addTopic($path->forum,$_POST['head']);
                $forum->addPost($t,$_POST['text']);
            }
        }

        if ( ( isset($_GET['action']) && $_GET['action'] == 'new' ) || $v->ExistErrors() ) {
            $tpl->assign('action','?action=save');
            $tpl->assign('vals',$_POST);
            $tpl->assign('errors',$v->getAll());
            $tpl->display('forum/newthread.tpl.php');
        } else {
                // List all topics
                $tpl->assign('forum',$forum1);
                $tpl->assign('topics',$forum->getTopics($path->forum,0,100));
                $tpl->display('forum/start.tpl.php');
                $tpl->display('forum/forum.tpl.php');
        }

// Root - list all forums
    } else {
        $tpl->assign('forums',$forum->getForums());
        $tpl->display('forum/start.tpl.php');
        $tpl->display('forum/list.tpl.php');
    }
?>
