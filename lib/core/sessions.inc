<?php
/* vim: set expandtab tabstop=3 shiftwidth=3: */

/**
 *
 * Set_save_session_handler functions 
 * 
 * PHP Version 5
 *
 * @category   sessions 
 * @copyright  2006 the honeydew group
 * @link       http://honeydew.se/
 * @author     Joel Hansson <joel@everlast.se>
 * @author     Jonathan Svensson-Köhler <stamp@stamp.se>
 **/

// if there were no user defined `session table`
if (! defined('SESSION_TBL')) {
   $tmp_tbl = 'sessions';
   define(SESSION_TBL,$tmp_tbl);
}

// Sets default sess_life
//$sess_life = get_cfg_var("session.gc_maxlifetime"); //default from php.ini
$sess_life = 3600; // One hour.

function sess_open($save_path, $session_name)
{
   // we should alread have made the connection to the database
   if (! class_exists('db') ) {
      die('No db-object were present');
      return false;
   }

   // check that the table exists
   if (!db::tableExists(SESSION_TBL)) {
      // install a new table
      db::query(db::makeCreateQuery(SESSION_TBL,
         array(
            array(
                'Field'   => 'sesskey',
                'Type'    => 'varchar(32)',
                'Key'     => 'PRI'
            ),
            array(
                'Field'   => 'expiry',
                'Type'    => 'int(11)',
                'Null'    => 'YES'
            ),
            array(
                'Field'   => 'ip',
                'Type'    => 'varchar(32)',
                'Null'    => 'YES'
            ),
            array(
                'Field'   => 'value',
                'Type'    => 'blob',
                'Null'    => 'YES'
            ),
            array(
                'Field'   => 'uid',
                'Type'    => 'int(11)',
                'Default' => '0'
            ),
            array(
                'Field'   => 'created',
                'Type'    => 'timestamp'
            )
         )
      ));
   }

   return true;
}

function sess_close() 
{
   return true;
}

function sess_read($key) 
{
   $sql = 'SELECT value FROM '.SESSION_TBL.' '.
      'WHERE sesskey = "'.$key.'" '.
      'AND expiry > ' . time().' '.
      'AND ip = "'.$_SERVER['REMOTE_ADDR'].'" '.
      'LIMIT 1';

   if ($result = db::fetchSingle($sql)) {
      return $result['value'];
   }
   db::query('DELETE FROM '.SESSION_TBL.' WHERE sesskey = "'.$key.'"');
   return false;
}

function sess_write($key, $val) 
{
   global $sess_life;

   $expiry = time() + $sess_life;
   $value  = addslashes($val);

   /**
    * is there already a row with the correct sesskey and ip?
    * if so, we should do update, if not, lets do insert
    */
    
   $sql_chk_key = 'SELECT sesskey, expiry '.
      'FROM '.SESSION_TBL.' '.
      'WHERE sesskey="'.$key.'" '.
      'AND ip="'.$_SERVER['REMOTE_ADDR'].'" '.
      'LIMIT 1';

   $result = db::fetchSingle($sql_chk_key);

    $user = isset($_SESSION['id']) ? $_SESSION['id'] : 0;
    
   // destroy expired sessions;
   if (($result['sesskey']) && ($result['expiry'] < time()) ) {
      sess_destroy($result['sesskey']);
      return false;

   } else if ($result['sesskey']) {
      $sql = 'UPDATE '.SESSION_TBL.' SET '.
         'expiry = '.$expiry.', '.
         'uid = '.$user.', '.
         'value = "'.$value.'" '.
         'WHERE sesskey = "'.$key.'" '.
         'AND ip = "'.$_SERVER['REMOTE_ADDR'].'"';

   } else {
      $sql = 'INSERT INTO '.SESSION_TBL.' SET '.
         'sesskey="'.$key.'", '.
         'expiry='.$expiry.', '.
         'uid='.$user.', '.
         'value="'.$value.'", '.
         'ip ="'.$_SERVER['REMOTE_ADDR'].'"';
   }

   $qry = db::query($sql);
   mysql_close();
   return $qry;
}

function sess_destroy($key) 
{
   $time = time();

   $sql = 'DELETE FROM '.SESSION_TBL.' WHERE sesskey = "'.$key.'" OR expiry < ' . $time;   

   $qry = db::query($sql);

   logg('Cleared sessions; '.mysql_affected_rows().' rows removed.');

   return $qry;
}

function sess_gc($sess_life) 
{
   $time = time();

   $sql = 'DELETE FROM '.SESSION_TBL.' WHERE expiry < ' . $time;
   db::query($sql);

   return db::affectedRows();
}

session_set_save_handler(
      "sess_open",
      "sess_close",
      "sess_read",
      "sess_write",
      "sess_destroy",
      "sess_gc");

// Fire it up!
session_start();
?>
