<?php
/* vim: set expandtab tabstop=3 shiftwidth=3: */

/**
 *
 * Mysql Database layer 
 * 
 * PHP Version 5
 *
 * LICENCE: GPL
 *
 * @category   Database 
 * @copyright  2005 the honeydew group
 * @link       http://honeydew.se/
 * @author     Joel Hansson <joel@everlast.se>
 * @author     Jonathan Svensson-Köhler <stamp@stamp.se>
 **/


class db {

   // Properties {{{
   /**
    * Database connect resource #id
    *
    * @var num
    */
   protected    $linkId;
   const        version = '2.2.0';

   /**
    * Keeps track of query amount
    *
    * @var num
    */
   public      $queryCount=0;
   public      $queryTime=0;
   // }}}
   // __construct {{{
   /**
    * Initiate database connect
    *
    * @param string $host     the hostname
    * @param string $username the username
    * @param string $passwd   the password
    * @param string $dbName   the name of the database
    */
   public function __construct($host, $username, $passwd, $dbName)
   {
      // connect to the database manager
      $this->linkId = mysql_connect($host, $username, $passwd)
         or die('Broken connection to database manager: '.$host.
                "<br/>\n".mysql_error());

      // Select database
      mysql_select_db($dbName, $this->linkId)
         or die('Broken connection to database: '.$dbName.
                "<br/>\n".mysql_error() );
   }

   // }}}
// sqlInfo {{{
/**
 * Add debug info from mysql
 *
 * @param str string
 */
function sqlInfo($q) 
{   
    global $errors;
    //Clean the printout
   $chars = preg_split('/SELECT |UPDATE |INSERT |SET |VALUES |FROM |JOIN |ON |ORDER BY |LIMIT |WHERE /',
    $q, -1, PREG_SPLIT_OFFSET_CAPTURE);
   $chars2 = str_word_count($q, 2);
   $q = '';
   $nl = '';
   foreach ($chars2 as $key => $line2) {
       if (in_array($line2,array('SELECT','UPDATE','INSERT','SET','VALUES','FROM','JOIN','ON','ORDER BY','LIMIT','WHERE'))) {
           $q .= $nl;
           $nl = "<br/>";
           $q .= '<span style="color:#0054A6">'.$line2."</span> ";
           foreach ($chars as $line) {
               if ( $line[1] == ($key+strlen($line2)+1) ) {
                   if ($line2=="WHERE"||$line2=="SET") {
                       $s = preg_replace("/(\w+|`\w+`)(| )(=|<|>|LIKE)(| )(\"(\w+)\"|'(\w+)'|\w+)/i",
                        "<font style='color:#A66600;'>$1</font>$3<font style='color:#A60000;'>$5</font>",
                        $line[0]);
                       $s = preg_replace(array('@ AND @','@ OR @'),"<font style='color:#0054A6;'>$0</font>",$s);
                       $q .= $s;
                   } elseif ($line2=="SELECT") {
                       $q .= preg_replace(array('@COUNT@i','@MAX@i','@MIN@i','@SUM@i'),"<font style='color:#02821A;'>$0</font>",$line[0]);
                   } else {
                       $q .= $line[0];
                   }
               }
           }
       }
   }
    send(E_NOTICE,$q);
}
//}}}
   // {{{ tableExists 
   function tableExists($tbl) {
      if (isset($this)) {
         if ( (!isset($this->tables)&&$this->tables = $this->fetchAllOne("SHOW tables")) || isset($this->tables)){
            if (is_array($this->tables))
             return in_array($tbl,$this->tables);
           else
             return false;
         } else return false;
      } else {
         global $tables;
         if ( (!isset($tables)&&$tables = self::fetchAllOne("SHOW tables")) || isset($tables)){
            if (is_array($tables))
             return in_array($tbl,$tables);
           else
             return false;
         } else return false;   
      }
   }
   // }}}
   // returnError {{{
   /**
    * Print out nice error thingy
    */
   protected function returnError()
   {
      $msg  = '<style> th {text-align:left;}</style>';
      $msg .= '<br/><br/><br/><br/><table>';
      $msg .= '<tr><th>Mysql error:</th><td>'.mysql_error().'</td></tr>';
      foreach(debug_backtrace() as $err) {
         if ( isset($err['file']) ) {
            $msg .= '<tr><th>Fil:</th><td>'.$err['file'].'</td></tr>';
         }
         if ( isset($err['line']) ) {
            $msg .= '<tr><th>Rad:</th><td>'.$err['line'].'</td></tr>';
         }
         if ( isset($err['class']) ) {
            $msg .= '<tr><th>Klass:</th><td>'.$err['class'].'</td></tr>';
         }
         if ( isset($err['function']) ) {
            $msg .= '<tr><th>Funktion:</th><td>'.$err['function'].'</td></tr>';
         }
         $i    = 0;
         foreach($err['args'] as $args) {
            $msg .= "<tr><td>";
            $msg .= "Arg[{$i}]</th><td>{$args}</td></tr>";
            $i++;
         }
         $msg .= '<tr><td colspan="3"><hr></td></tr>';
      }
      $msg .= '</table>';

      return $msg;

   }

   // }}}
   // getErr {{{
   /**
    * Returns the error array. Manily used in subclasses
    */
   public function getErr()
   {
      if(isset($this->err)) {
         return $this->err;
      } else {
         return false;
      }
   }

   // }}}
   // getLinkId {{{
   /**
    * Returns the link identifier #resource
    */
   public function getLinkId()
   {
      return $this->linkId;
   }

   // }}}
   // insert_id {{{
   /**
    * A port of mysql_insert_id()
    *
    * @return  int   mysql_insert_id
    */
   public function insert_id()
   {
      return mysql_insert_id();
   }

   // }}}
   // query {{{
   /**
    * perform a database query 
    * 
    * @param string $sql the sql statement
    */
   public function query($sql)
   {
      if (DEBUG) self::sqlInfo($sql);
      $s = microtime(true);
      if(!$q = mysql_query($sql) ) {
            trigger_error('<b>SQL error:</b> <br><i>'.$sql.'</i><br>'.mysql_error());
      }

      if (isset($this)) {
          $this->queryTime += microtime(true)-$s;
          $this->queryCount++;
      }
      return $q;

   }

   // }}}
   // fetchRow {{{
   /**
    * fetch one row with mysql_fetch_row
    * Returns array or false
    *
    */
   public function fetchRow($qryId = null)
   {
      if ($qryId != null)
         return ($r = mysql_fetch_row($qryId) ) ? $r : false;
      return false;
   }

   // }}}
   // fetchAssoc {{{
   /**
    * Fetch one row with mysql_fetch_assoc
    * Returns associative array or false
    *
    */
   public function fetchAssoc($qryId = null)
   {
      if ($qryId != null)
         return ($r = mysql_fetch_assoc($qryId) ) ? $r : false;
      return false;
   }

   // }}}
   // affectedRows {{{
   /**
    * do mysql_affected_rows
    *
    *
    */
   public function affectedRows()
   {
      return mysql_affected_rows();
   }
   // }}}
   
   // escapeStr {{{
   /**
    * Perform mysql_real_escape_string on $args
    * Returns escaped (string|array)
    *
    * @param (string|array)   $str, array or string 
    */
   public function escapeStr($str)
   {
      $strip = get_magic_quotes_gpc();
      
      if(is_array($str)) {                // if array do loop
         foreach($str as $key => $val) {
            if ($strip) 
               $val = stripslashes($val);
            
            $str[$key] = mysql_real_escape_string($val);
         }
         
      } else {                            // else just escape the string.


         if ($strip) 
            $str = stripslashes($str);
         

         $str = mysql_real_escape_string($str);
      }
      return $str;
   }

   // }}}
   // fetchAll {{{
   /**
    * Perform query.
    * Return multidimensional array or false
    *
    * @param string  $sql sql query string
    */
   public function fetchAll($sql = '')
   {
      $q = self::query($sql);
    
      if ($q) {
         $r = '';
         while($result = self::fetchAssoc($q) ) {
            $r[] = $result;
         }
         //jonaz safemod
         mysql_free_result($q);
         return (count($r) > 0) ? $r : false;
      } else {
         return false;
      }
   }

   // }}}
   // fetchSingle {{{
   /**
    * Perform query.
    * Return array or false
    *
    * @param string  $sql sql query string
    */
   public function fetchSingle($sql = '')
   {
      $q = self::query($sql);
      if($r = self::fetchAssoc($q)){
        //jonaz safemod
        mysql_free_result($q);
        return $r;
      }
      return false;
      
   }

   // }}}
   // fetchOne {{{
   /**
    * Perform query.
    * Return array or false
    *
    * @param string  $sql sql query string
    */
   public function fetchOne($sql = '')
   {
      $q = self::query($sql);
        if($r=self::fetchRow($q)){

        //jonaz safemod
        mysql_free_result($q);
            return $r[0];
        }
      return  false;
      
   }

   // }}}
   // fetchAllOne {{{
   /**
    * Perform query.
    * Return array or false
    *
    * @param string  $sql sql query string
    */
   public function fetchAllOne($sql = '')
   {
      $q = self::query($sql);
      
      if ($q) {
         $r = '';
         while($result = self::fetchRow($q) ) {
            $r[] = $result[0];
         }
         
        //jonaz safemod
        mysql_free_result($q);
         return (count($r) > 0) ? $r : false;
      } else {
         return false;
      }
   
      
   }

   // }}}
   // select {{{
   /**
    * Do a select and return result
    *
    * @param array   $fields  array with the fields that are selected
    * @param string  $form    string with the from clouse
    * @param string  $where   string with the where clouse
    */
   public function select($fields, $from, $where = null,$limit = null,$order = null)
   {

      // Escape special chars on all input.
      $fields  = self::escapeStr($fields);
      $from    = 'FROM '.self::escapeStr($from);
      $order   = self::escapeStr($order);
      if (is_array($where)) { foreach($where as $key => $val) {
         $where[$key] = self::escapeStr($val);
      }}
      
      //
      // Fix select
      //
      foreach ($fields as $s) {
         if (! isset($notFirst)) {
            $notFirst = 'y';
            $f = 'SELECT ';
         } else $f .= ', ';
         $f .= self::escapeStr($s);
      }
      unset($notFirst);

      //
      // Fix where 
      //
      if (is_array($where)) { foreach($where as $s) {
         if (! isset($notFirst)) {
            $notFirst = 'y';
            $w = 'WHERE ';
         } elseif (!isset($s['OR'])) $w .= ' AND ';
         else   $w .= ' OR ';

         $w .= self::escapeStr($s['field']).' = ';
         if (is_int($s['value']) ) {
            $w .= self::escapeStr($s['value']);
         } elseif (is_string($s['value'])) {
            $w .= '"'.self::escapeStr($s['value']).'"';
         } else  return false;

      }} else $w = '';

      //
      // Fix limit
      //
      if (is_array($limit)) {
         $l = intval($limit[0]);
         if (isset($limit[1]) ) {
            $l .= ','.intval($limit[1]);
         }
      } else $l ='';

      //
      // Fix order by
      //
      if (!is_null($order) && !empty($order)) {
         $o = 'ORDER BY '.$order;
      } else $o = '';

      $sql = $f."\n".$from."\n".$w."\n".$o."\n".$l."\n";

      return ($q = self::fetchAll($sql)) ? $q : array();
   }

   // }}}
   // insert {{{
   /**
    * Return select sql statement
    *
    * @param array   $fields  fields to insert
    * @param string  $table   table to do insert on
    */
   public function insert($fields, $table)
   {
      $table  = self::escapeStr($table);
      
      $sql    = sprintf('INSERT INTO %s SET ',$table);
      
      foreach($fields as $key => $val) {
         $key = self::escapeStr($key);
         $val = self::escapeStr($val);

         if (!isset($notFirst)) {
            $notFirst = 'Y';
         } else $sql .= ', ';

         if (is_int($val)) {
            $sql .= sprintf('`%s` = %d', $key, intval($val) );
         } elseif (is_array($val)) {
            $sql .= sprintf('`%s` = %s', $key, $val['txt']); 
         } elseif (is_string($val)) {
            $sql .= sprintf('`%s` = "%s"', $key, $val);
         } else {
            return false;
         }
      }

      if (self::query($sql)) return true;
      else return false;
   }

   // }}}
   // update {{{
   /**
    *
    *
    *
    */
   public function update($fields,$table,$where)
   {
      $table = self::escapeStr($table);
      $sql   = sprintf('UPDATE %s SET ',$table);

      if (is_array($where)) { foreach($where as $key => $val) {
         $where[$key] = self::escapeStr($val);
      }}

      //
      // Fix fields
      //
      foreach($fields as $key => $val) {
         $key = self::escapeStr($key);
         $val = self::escapeStr($val);

         if (!isset($notFirst)) {
            $notFirst = 'Y';
         } else $sql .= ', ';

         if (is_int($val)) {
            $sql .= sprintf('`%s` = %d', $key, intval($val) );
         } elseif (is_string($val)) {
            $sql .= sprintf('`%s` = "%s"', $key, $val);
         } else {
            return false;
         }
      }
      $sql .= ' ';
      unset($notFirst);
      //
      // Fix where  -- old way
      //
      /*if (is_array($where)) { foreach($where as $s) {
         if (! isset($notFirst)) {
            $notFirst = 'y';
            $w = 'WHERE ';
         } elseif (!isset($s['OR'])) $w .= ' AND ';
         else   $w .= ' OR ';

         $w .= $tnhis->escapeStr($s['field']).' = ';
         if (is_int($s['value']) ) {
            $w .= $this->escapeStr($s['value']);
         } elseif (is_string($s['value'])) {
            $w .= '"'.$this->escapeStr($s['value']).'"';
         } else  return false;

      }} else $w = '';*/

      $sql .= $where;

      self::query($sql);
      return true;
   }


   // }}}

   // installTables {{{
   /**
    * Install tables from a definition array.
    * Return true or false
    *
    * @param string  $tables array with table definitions
    */
   public function installTables($tables)
   {
        $todo = array();
        
        // loop tru the definition
        foreach ($tables as $table => $definition) {
            
            // get the table installed
            if (self::tableExists($table)&&$existing = self::fetchAll("DESCRIBE `$table`")) {
                foreach ($definition as $field) {
                    $res = 0; // not found
                    // look for the field, it mabye exists
                    foreach ($existing as $exfield) {
                        if ($field['Field']==$exfield['Field']) {
                            $res = 2; // found and all ok
                            // check if there are a difference
                            foreach ($field as $key => $line) {
                                if(strtolower($exfield[$key]) != strtolower($line)) {
                                    $res = 1; // found but differs
                                    break;
                                }
                            }
                            break;
                        }
                    } 

                    if ($res == 0) { //field dont exist
                        if (!isset($todo['add'])) $todo['add'] = array();
                        if (!isset($todo['add'][$table])) $todo['add'][$table] = array();

                        $todo['add'][$table][$field['Field']] = $field;
                    } elseif ($res == 1) { //field exist but not correct
                        if (!isset($todo['alt'])) $todo['alt'] = array();
                        if (!isset($todo['alt'][$table])) $todo['alt'][$table] = array();

                        $todo['alt'][$table][$field['Field']] = $field;
                    }
                }

            } else {
                if (!isset($todo['create'])) $todo['create'] = array();
                $todo['create'][$table] = $definition;
            }

        }
        
        echo "<pre>Installing tables\n";
        foreach ($todo as $action => $data) {
            if ($action=='create') {
                foreach ($data as $name => $line) {
                    echo "\tAdding $table\n";
                    self::query(self::makeCreateQuery($name,$line));
                }
            } elseif ($action=='add') {
                foreach ($data as $tbl => $fielddef)
                    foreach ($fielddef as $field => $def) {
                        echo "\tAdding $tbl.$field\n";
                        self::query( self::makeAddQuery($tbl,$field,$def) );
                    }
            } elseif ($action=='alt') {
                foreach ($data as $tbl => $fielddef)
                    foreach ($fielddef as $field => $def) {
                        echo "Changing $tbl.$field\n";
                        self::query(self::makeAltQuery($tbl,$field,$def) );
                    }
            }

        }
        echo '</pre>';
   }
    // }}}
    // {{{ makeCreateQuery
    function makeCreateQuery($tname,$def) {
        $rows = array();

        foreach($def as $row) {
            $rows[] = self::makeFieldDefinition($row);
        }

        $ret = "CREATE TABLE `$tname` (\n";
        $ret .= implode($rows,",\n");
        $ret .= ")";

        return $ret;
    }
    // }}}
    // {{{ makeAltQuery
    function makeAltQuery($tname,$fname,$def) {

        $ret = "ALTER TABLE `$tname` CHANGE `$fname` \n";
        $ret .= self::makeFieldDefinition($def);

        return $ret;
    }
    // }}}
    // {{{ makeAddQuery
    function makeAddQuery($tname,$fname,$def) {

        $ret = "ALTER TABLE `$tname` ADD \n";
        $ret .= self::makeFieldDefinition($def);

        return $ret;
    }
    // }}}
    // {{{ makeFieldDefinition
    function makeFieldDefinition($row) {
         if(isset($row['Field'])) {
             $name    = $row['Field'];
             $type    = (isset($row['Type']))                       ? $row['Type']      : 'varchar(255)';
             $default = (isset($row['Default']))                    ? "DEFAULT '{$row['Default']}'"   : '';
             $pri     = (isset($row['Key'])&&$row['Key']=='PRI')    ? 'PRIMARY KEY'     : '';
             $extra   = (isset($row['Extra']))                      ? $row['Extra']     : '';
             $null    = (isset($row['Null'])&&$row['Null']!='NO')   ? 'NULL'            : 'NOT NULL';

             return "`$name` $type $null $extra $pri $default";
         } 
         return false;
    }
    // }}}

   // installTablesContent {{{
   /**
    * Install tables content from a definition array.
    * Return true or false
    *
    * @param string  $tables array with table content definitions
    */
   public function installTablesContent($tables)
   {
        $todo = array();
        
        // loop tru the definition
        foreach ($tables as $table => $definition) {
            foreach ($definition as $line) {
               $fields = array_keys($line);
                   
               $sq = array();
               $line = self::escapeStr($line);
               foreach ($fields as $field) {
                   $sq[] = "`$field` = '{$line[$field]}'";
               }

               // get the table installed
               if (self::tableExists($table)) {
                   if(!$data = self::fetchAll("SELECT `".implode($fields,'`,`')."` FROM `$table` WHERE ".implode($sq,' AND '))) {
                       if (!isset($todo[$table])) $todo[$table] = array();
                       $todo[$table][] = $line;
                   }
               } else return false;
            }
        }
        
        echo "<pre>Installing table content\n";
        foreach ($todo as $table => $def) {
            foreach ($def as $line) {
                echo "\t$table\n";
                $data = array();

                foreach ($line as $k => $d) {
                    echo "\t\t$k = $d\n";
                    $data[] = "`$k` = '$d'";
                }
                
                self::query("INSERT INTO `$table` SET ".implode($data,', '));
            }
        }
        echo "</pre>";

   }
    // }}}
    function version() {
        // return as an array
        return explode('.',self::version);
    }
}

