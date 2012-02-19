<?php
/* vim: set expandtab tabstop=3 shiftwidth=3: */

/**
 *
 * User defined error handler 
 * 
 * PHP Version 5
 *
 * LICENCE: GPL
 *
 * @category   Error handling 
 * @copyright  2005 the honeydew group
 * @link       http://honeydew.se/
 * @author     Joel Hansson <joel@everlast.se>
 * @author     Jonathan Svensson-Köhler <stamp@stamp.se>
 **/


$errors = array();

// errorHandler {{{
/**
 *
 *
 *
 */
function errorHandler($errno, $errstr, $errfile, $errline, $vars)
{
   global $errors;
   $errortype = array (
               E_ERROR           => "Error",
               E_WARNING         => "Warning",
               E_PARSE           => "Parsing Error",
               E_NOTICE          => "Notice",
               E_CORE_ERROR      => "Core Error",
               E_CORE_WARNING    => "Core Warning",
               E_COMPILE_ERROR   => "Compile Error",
               E_COMPILE_WARNING => "Compile Warning",
               E_USER_ERROR      => "User Error",
               E_USER_WARNING    => "User Warning",
               E_USER_NOTICE     => "User Notice",
               E_STRICT          => "Runtime Notice"
               );
   $errstr = str_replace("href='","href='http://se.php.net/manual/en/",$errstr);

   
   $str = "<strong>[".$errortype[$errno]."]</strong>\n";
   $str .=  $errstr."<br/>\n";
   $str .= 'In file: <strong>'.$errfile.'</strong> on <strong>'.
   $errline."</strong>\n";

   if  ($errno == E_ERROR||$errno == E_CORE_ERROR||$errno == E_COMPILE_ERROR||$errno == E_USER_ERROR||$errno == E_PARSE) {
        send(E_CORE_ERROR,$str,false);
   } elseif ($errno == E_WARNING||$errno == E_CORE_WARNING||$errno == E_COMPILE_WARNING||$errno == E_USER_WARNING) {
        send(E_CORE_WARNING,$str,false);
   } elseif ($errno == E_USER_NOTICE||$errno == E_NOTICE) {
        send(E_NOTICE,$str,false);
   }
}

// }}}
// debugInfo {{{
/**
 * Add debug info
 *
 * @param str string
 */
function debugInfo($str) 
{   
    global $errors;
    array_push($errors,array($str,'','Info'));
}
//}}}
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

    array_push($errors,array($q,'','Info'));
}
//}}}
// sqlError {{{
/**
 * Fetch mysql errors
 *
 */
function sqlError($q) 
{   
    global $errors;
    
    $str = "<strong>[MYSQL]</strong>\n";
    $str .= mysql_error().'<br />';
    $str .= '<i>'.$q.'</i>';
    send(E_CORE_ERROR,$str,false);
    return false;
}
//}}}
// errorLog {{{
/**
 *
 *
 *
 */
function errorLog($str, $file)
{
   if (!$handle = @fopen('logs/'.$file.'.log.html','a') ) {
      return false;
   }

   if (fwrite($handle,$str) === FALSE) {
      return false;
   }

   @fclose($handle);

   return true;

   
}

// }}}
// errorDisplay {{{
/**
 * Print out all errors
 * 
 */
function errorDisplay()
{
    global $errors;
    if (count($errors)>0) {
       echo '<div id="errorBox"><strong>Debug INFO</strong><br />';
       foreach ($errors as $line) {
           echo '<p class="error'.$line[2].'">';
           echo $line[0]."<br />\n";
           
           //echo '< style="font-size:11px;">';
           // Debug backtrace print out
           if (isset($line[3])) {
               //echo '<p class="errorBacktrace">';
               foreach($line[3] as $err) {
                  if ( isset($err['file']) ) {
                     echo "Fil:\t".$err['file']."<br />\n";
                  }
                  if ( isset($err['line']) ) {
                     echo "Rad:\t<strong>".$err['line']."</strong><br />\n";
                  }
                  if ( isset($err['class']) ) {
                     echo "Class:\t".$err['class']."<br />\n";
                  }
                  if ( isset($err['function']) ) {
                     echo "Funkt:\t".$err['function']."<br />\n";
                  }
                  $i    = 0;
                  if (isset($err['args'])) {
                     //echo '<p class="errorBacktraceArgs">';
                     foreach($err['args'] as $args) {
                        echo "Arg[{$i}]<i>{$args}</i><br />\n";
                        $i++;
                     }
                     //echo '</p>'; 
                  }
                  echo "<br />\n";
                  //echo '</p>';
               }
               //echo '</p>';
           }
            
           // Vars print out
           if (is_array($line[1])) {
               print_r($line[1]);
           }

           echo '</pre>';
           echo '</p>';
       }
       echo '</div>';
    }
}
// }}}

// Lets use our error handler
set_error_handler('errorHandler');

?>
