<?php

class evaluate extends module {
    
    function __construct() {
        $this->setId(get_class($this));
        $this->setName('CCO - Utvärdering');
        $this->setDescription('Dreamhack crew event utvärdering');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('safety aculo');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install() {
        $tables = array (
            'evaluation' => array(
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
                    'Field'   => 'qid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'answer',
                    'Type'    => 'text'
                )
            ),'evaluate_questions' => array(
                array(
                    'Field'   => 'qid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'event',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'head',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'definition',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'prio',
                    'Type'    => 'int(11)'
                )
            )
        );

        $this->installTables($tables);
        
        return parent::install();
    }

    function _test() {
        
        if(self::checkEvaluation())
            self::doEvaluation();

            //echo serialize(array('template'=>'horisontal_scroll','min'=>1,'max'=>100,'label-left'=>'Dåligt','label-right'=>'Perfekt','standard'=>50));
            //echo serialize(array('template'=>'text','rows'=>3,'desc'=>'Var maten god? Fick du tillräckligt med mat?'));
    }

    function _showTeamLeaderEvaluation() {
        global $path,$tpl;
        $groups = array($path->vars['team']);

        if($subgroups = self::getGrandChildsList($path->vars['team']))
            $groups = array_merge($groups,$subgroups);

        $members = db::fetchAllOne("SELECT uid FROM membership WHERE gid IN (".implode($groups,',').")");
        
        $eval = db::fetchAllOne("SELECT answer FROM evaluation WHERE uid IN (".implode($members,',').") AND qid=23");

        $tpl->assign('eval',$eval);
        $this->display('showTeamLeaderEvaluation.tpl');
    }

    function _showFSEvaluation() {
        global $path,$tpl;
        $groups = array($path->vars['team']);
        
        $eval = db::fetchAllOne("SELECT answer FROM evaluation WHERE qid=9");

        $tpl->assign('eval',$eval);
        $this->display('showTeamLeaderEvaluation.tpl');
    }

   // getChildsList {{{
    static function getChildsList($from) {
        if ( $subs = db::fetchAllOne("SELECT gid FROM structure WHERE (parent='".$from."' AND (event='".$_SESSION['event']."' OR event=0) AND name NOT LIKE '-%') ORDER BY name")) 
            return $subs;
        return array();
    }
   // }}}
   // getGrandChildsList {{{
    static function getGrandChildsList($from) {
        $ret = array();

        if($ret = self::getChildsList($from)) 
            foreach ( $ret as $line ) 
                if ($childs = self::getGrandChildsList($line))
                    $ret = array_merge($ret,$childs);
        
        if ( isset($ret) && is_array($ret) )
            return $ret;
        return array();
    }
   // }}}

    function checkEvaluation() {
        if (!isset($_SESSION['event'])||!is_numeric($_SESSION['event']))
            return false;

        if (isset($_SESSION['evaluation']))
            return false;

        if (db::fetchAll("SELECT * FROM evaluate_questions JOIN evaluation USING (qid) WHERE uid={$_SESSION['id']} AND event={$_SESSION['event']}")) {
            $_SESSION['evaluation'] = true;
            return false;
        }

        if (!$questions = db::fetchAll("SELECT * FROM evaluate_questions WHERE event = {$_SESSION['event']} ORDER BY prio") ) 
            return false;

        if (isset($_POST) && count($_POST)>0)
            foreach ( $_POST as $key => $line ) 
                if ( substr($key,0,6) == 'field_' && is_numeric(substr($key,6)) )
                    foreach ( $questions as $qk => $q ) 
                        if ( $q['qid'] == substr($key,6) ) {
                            $questions[$qk]['value'] = $line;
                            break;
                       }

        $saved = false;
        foreach ($questions as $key => $line) {
            $questions[$key]['definition'] = unserialize($line['definition']);
            if (isset($questions[$key]['definition']['min'])||isset($questions[$key]['definition']['max'])) {
                $min = isset($questions[$key]['definition']['min']) ? $questions[$key]['definition']['min'] : 0;
                $max = isset($questions[$key]['definition']['max']) ? $questions[$key]['definition']['max'] : 1;

                if(isset($questions[$key]['definition']['standard']))
                    $questions[$key]['definition']['standard'] = ($questions[$key]['definition']['standard'] - $min) / ($max - $min);

                if(isset($questions[$key]['value']))
                    $questions[$key]['value_conv'] = $questions[$key]['value'] * ($max - $min);
            }

            if (isset($_POST['field_'.$line['qid']]) && isset($questions[$key]['value']) && trim($questions[$key]['value']) > '') {
                db::insert(
                    array(
                        'uid'=>$_SESSION['id'],
                        'qid'=>$line['qid'],
                        'answer'=>isset($questions[$key]['value_conv'])?
                            $questions[$key]['value_conv']:
                            $questions[$key]['value']
                        ),
                    'evaluation');
                $saved = true;
            }
        }

        if ($saved)
            return false;
        
        if (!is_array($questions) || count($questions) == 0)
            return false;

        $this->q = $questions;

        return true;
        //return !db::fetchAll("SELECT * FROM evaluation,evaluate_questions WHERE uid={$_SESSION['id']} AND event={$_SESSION['event']} AND evaluate_questions.qid=evaluation.qid");
    }

    function doEvaluation() {
        global $tpl;

        $this->display('head.tpl');
        foreach ($this->q as $line) {
            $tpl->data = $line;
            if (!isset($tpl->data['value'])&&isset($tpl->data['definition']['standard'])) $tpl->data['value'] = $tpl->data['definition']['standard'];
            $this->display('field_'.$line['definition']['template'].'.tpl');
        }
        $this->display('foot.tpl');

        return true;
    }
}
