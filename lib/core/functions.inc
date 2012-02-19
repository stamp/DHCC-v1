<?php
/**
 *
 * Basic functions  
 * 
 * PHP Version 5
 *
 * @category   Mixed
 * @copyright  2006 the honeydew group
 * @author     Joel Hansson <joel@everlast.se>
 * @author     Jonathan Svensson-Köhler <stamp@stamp.se>
 **/

    // send() - the message queue handler {{{
    /**
     * the message queue handler, save messages to logg/queue
     *
     */
    function send($level,$msg,$save = true,$display = true) {
        global $error;

        if(isset($error)&&get_class($error)=='error')
            $error->send($level,$msg,$save,$display);
        else
            trigger_error($msg);
    }
    // }}}
    // get() - print out the message queue {{{
    /**
     * print out the message queue
     *
     */
    function get() {    
        global $error;
        
        if(isset($error)&&get_class($error)=='error')
            core::run($error,'get');

    }
    // }}}
// timestamp {{{
/**
 * Print out a mysql timestamp on a more easyread way
 *
 * @param string $tmp   the timestamp from mysql
 *
 */
    function timestamp($tmp,$sufix = true,$prefix = true) {
        
        if (date_default_timezone_get()!='Europe/Stockholm') date_default_timezone_set('Europe/Stockholm');

        $time = strtotime($tmp);
        if (isset($_SESSION['timezone'])) date_default_timezone_set($_SESSION['timezone']);
        
        $now = time();
        $diff = $now - $time;

        $ret = '';
        $ad = '';
        $cnt=0;
            $ad = ' ';
            $pd = '';

        // vänd på värdena om talet är negativt
        if ($diff < 0 ) {
            $diff = $time - $now;
            $pd = 'Om ';
        }

        $week = intval($diff / (60*60*24*7));
        $day = intval(($diff -(60*60*24*7*$week)) / (60*60*24));
        $hour = intval(($diff -(60*60*24*7*$week) - (60*60*24*$day))/ (60*60));
        $min = intval(($diff - (60*60*24*7*$week) - (60*60*24*$day) - (60*60*$hour)) / (60));
        $sec = intval(($diff - (60*60*24*7*$week) - (60*60*24*$day) - (60*60*$hour)  - (60*$min)));    
            


        if (date('y',$time)==date('y') && date('m',$time)==date('m') && date('d',$time)==date('d')) {
            $ret .='[<font color=#00CC00>Idag</font>] ';
            $ad = '';
        } elseif (date('y',$time)==date('y') && date('m',$time)==date('m') && date('d',$time)==(date('d')-1)) {
            $cnt++;
            $ret .='[<font color=#CCCC00>Igår</font>] ';
            $ad = '';
        } elseif (date('y',$time)==date('y') && date('m',$time)==date('m') && date('d',$time)==(date('d')-2)) {
            $cnt++;
            $ret .='[<font color=#CC0000>I förrgår</font>] ';
            //$ad = '';
            $pd = '';
        } else {
            if ($week==1) {
                $ret .= $week . ' vecka ';
                $cnt++;
            } elseif ($week>1) {
                $ret .= $week . ' veckor ';
                $cnt++;
            }
            
            if ($day==1 && $cnt<1) {
                $ret .= $day . ' dag ';
                $cnt++;
            } elseif ($day>1 && $cnt<1) {
                $ret .= $day . ' dagar ';
                $cnt++;
            }
        }    
        
        if ($hour==1 && $cnt<1) {
            $ret .= $hour . ' timme ';
            $cnt++;
        } elseif ($hour>1 && $cnt<1) {
            $ret .= $hour . ' timmar ';
            $cnt++;
        }  
 
        if ($min==1 && $cnt<1) {
            $ret .= $min . ' minut ';
            $cnt++;
        } elseif ($min>1 && $cnt<1) {
            $ret .= $min . ' minuter ';
            $cnt++;
        }

        if ($sec==1 && $cnt<1) {
            $ret .= $sec . ' sekund ';
            $cnt++;
        } elseif ($sec>1 && $cnt<1) {
            $ret .= $sec . ' sekunder ';
            $cnt++;
        }

        if ($sufix) $ret .= $ad;
        if ($prefix) $ret = $pd . $ret;

        //$ltime = $tmp[0].$tmp[1].'-'.$tmp[2].'-'.$tmp[3].' '.$tmp[4].':'.$tmp[5].':'.$tmp[6];
        $ltime = date('Y-m-d H:i:s',$time);
        $ret = "<span onMouseOver=\"this.innerHTML='$ltime';\" onMouseOut=\"this.innerHTML='$ret';\">$ret</span>";
            return $ret;
        
    }
    // }}}
    // age {{{
    /**
     * return the age of a persons birthdate
     *
     */
    function age($a) {
         $y = intval(substr($a,0,2));
         $m = intval(substr($a,2,2));
         $d = intval(substr($a,4,2));
         //echo "$y - $m - $d <br>";
         //echo "$a <br>";
         list($now_d,$now_m,$now_y) = explode('.',date('d.m.Y'));
         if ($y>50) {
            $y = '19'.$y;
         } else {
            $y = '20'.$y;
         }

         if(($m > $now_m) || ($m == $now_m && $now_d < $d)) {
             $age = $now_y - $y  - 1;
         } else {
             $age = $now_y - $y;
         }
            
         if ($age<100&&$age>0) {
            return $age;
         } else {
            return 0;
         }
    }
    // }}}
// logg {{{
    function logg($text) {
        global $db;

        if (!$db->tableExists('logg')) 
            $db->installTables(
                array(
                    'logg' => array(
                        array(
                            'Field'   => 'id',
                            'Type'    => 'int(11)',
                            'Key'     => 'PRI',
                            'Extra'   => 'auto_increment'
                        ),
                        array(
                            'Field'   => 'uid',
                            'Type'    => 'int(11)',
                        ),
                        array(
                            'Field'   => 'text',
                            'Type'    => 'varchar(255)'
                        ),
                        array(
                            'Field'   => 'info',
                            'Type'    => 'text'
                        ),
                        array(
                            'Field'   => 'timestamp',
                            'Type'    => 'timestamp'
                        )
                    )
                )
            );
        $debug = debug_backtrace();

        if (isset($debug[1]['class']) )
            $text = '<b>'.$debug[1]['class'] . ':</b> '.$text;

        $report = array(
            'text'  =>  $text,
            'info'    =>  serialize(array('ip' => $_SERVER["REMOTE_ADDR"],'agent' => $_SERVER["HTTP_USER_AGENT"],'querystring' => $_SERVER["QUERY_STRING"]))
        );

        if (isset($_SESSION['id']))
            $report['uid'] = $_SESSION['id'];

        $db->insert($report,'logg');
    }
// }}}
// read {{{
function read($field,$default = NULL){
    global $db;

    if(!$db->tableExists('settings') ){
        $tables = array (
                'settings' => array(
                    array(
                        'Field'   => 'id',
                        'Type'    => 'int(11)',
                        'Key'     => 'PRI',
                        'Extra'   => 'auto_increment'
                    ),
                    array(
                        'Field'   => 'field',
                        'Type'    => "varchar(255)"
                    ),
                    array(
                        'Field'   => 'value',
                        'Type'    => 'blob'
                    )
                )
            );
            $db->installTables($tables);
    }
    if(isset($field)){
        return $db->fetchOne("SELECT value FROM settings WHERE field='".$db->escapeStr($field)."'");
    }
    return false;
}
// }}}
function write($field,$value){
    global $db;
    if(!$db->tableExists('settings') ){
        $tables = array (
                'settings' => array(
                    array(
                        'Field'   => 'id',
                        'Type'    => 'int(11)',
                        'Key'     => 'PRI',
                        'Extra'   => 'auto_increment'
                    ),
                    array(
                        'Field'   => 'field',
                        'Type'    => "varchar(255)"
                    ),
                    array(
                        'Field'   => 'value',
                        'Type'    => 'blob'
                    )
                )
            );
            $db->installTables($tables);
    }
    if(isset($field) && isset($value)){
        $field1 = $db->escapeStr($field);
        $value1 = $db->escapeStr($value);
        if($db->fetchOne("SELECT field FROM settings WHERE field='". $field1."'")){
            //uppdatera databasen
            $db->query("UPDATE settings SET value='".$value1."' WHERE field='".$field1."'");
        }else{
            //sätt in i databasen
            $db->insert(array('field'=>$field,'value'=>$value),'settings');
        }


    }
}
// }}}

?>
