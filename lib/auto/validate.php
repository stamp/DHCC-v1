<?php
/* vim: set expandtab tabstop=3 shiftwidth=3: */

/**
 *
 * Validate methods 
 * 
 * PHP Version 5
 *
 * LICENCE: GPL
 *
 * @category   Validation 
 * @copyright  2005 the honeydew group
 * @link       http://honeydew.se/
 * @author     Joel Hansson <joel@everlast.se>
 * @author     Jonathan Svensson-Köhler <stamp@stamp.se>
 **/

class Validate {

   /**
    * Associative array with error messages
    *
    * @var array
    */
   public $error = array();

   /**
    * Associative array with strings
    * that are to be validated
    *
    * @var array
    */
   public $str;


   // __construct {{{
   /**
    * constructor, set
    * values
    *
    * @param val array  associative array with all values
    */
   public function __construct($val)
   {
      $this->str = $val;
   }

   // }}}
   // char {{{
   /**
    * Checks string for sane characters.
    * return true if the string is valid.
    * else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    maximum amount of chars
    */
   public function char($id, $min, $max)
   {
      $str = $this->str[$id];
      
      if (!preg_match('/^[\w\.\-\*åäöÅÄÖ\s\+,]{'.$min.','.$max.'}$/i', $str)) {
         $this->error[$id] = 'Använd '.$min.' till '.$max.'st vanliga tecken.';
         return false;
      } else return true;
   }
   // }}}
   // numeric {{{
   /**
    * Checks string for numeric characters
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    maximum amount of chars
    */
   public function numeric($id, $min, $max)
   {
      $str = $this->str[$id];

      if (!preg_match('/^[\d\/\-\s\.]{'.$min.','.$max.'}$/', $str) ) {
         $this->error[$id] = 'Använd '.$min.' till '.$max.
                             ' st vanliga siffror, mellanslag, punkt eller '.
                             'bindestreck.';
         return false;
      } else return true;
   }
   // }}}
   // number {{{
   /**
    * Checks string for number only
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    maximum amount of chars
    */
   public function number($id, $min, $max)
   {
      $str = $this->str[$id];
      if(!preg_match('/^[\d]{'.$min.','.$max.'}$/', $str) ) {
         $this->error[$id] = 'Använd endast '.$min.' till '.$max.' '.
                             'st siffror';
         return false;
      }
      else return true;
   }
   // }}}
   // length {{{
   /**
    * Check string for valid length
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    maximum amount of chars
    */
   public function length($id, $min = 0, $max = 65535,$trim = false)
   {
      $str = $this->str[$id];
      if ($trim)
        $str = trim($str);

      if(strlen($str) > $max || strlen($str) < $min) {
         $this->error[$id] = 'Använd '.$min.' till '.$max.' st tecken.';
         return false;
      } else return true;
   }
   // }}}
   // email {{{
   /**
    * Check string for valid email
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param id string  identifier
    * @param int min    if the field is required to be filled
    */
   public function email($id, $min = 1)
   {
      $str = $this->str[$id];

      if(($min == 0) && strlen($str) < 1) {
         return true;
      }

      $pattern = "/^[\w\.\-åäöÅÄÖ]+@[\w\.\-äåö]+\.[a-z]{2,4}$/i";

      if (!preg_match($pattern, $str)) {
         $this->error[$id] = 'Du måste skriva in en korrekt emailaddress';
         return false;
      } else {
         return true;
      }
   }
   // }}}
   // word {{{
   /**
    * Check string to be sane (one word only)
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    if the field is required to be filled
    */
   public function word($id, $min = 1, $max = 40)
   {
      $str = $this->str[$id];

      if(($min == 0) && strlen($str) < 1) return true;

      $pattern = '/^[\w\-_,\.]{'.$min.','.$max.'}$/i';

      if (!preg_match($pattern, $str)) {
         $this->error[$id] = 'Använd endast a-Z,-_0-9';
         return false;
      } else return true;
   }
   // }}}
   // zip {{{
   /**
    * Check for valid zip format
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string  id  identifier
    * @param int     min minimum amount of chars
    */
   public function zip($id, $min = 1)
   {
      $str = $this->str[$id];

      if (($min == 0) && (strlen($str) < 1)) {
         return true;
      }
      if (!preg_match("/^[\d]{3}[-\s][\d]{2}$/", $str)) {
         $this->error[$id] = 'Godkänt postnummer: 000-00 och 000 00';
         return false;
      } else {
         return true;
      }
   }
   // }}}
   // idnr {{{
   /**
    * Checks for id number xxxxxx-xxxx | xxxxxx xxxx
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    */
   public function idnr($id, $min = 1)
   {
      $str = $this->str[$id];

      if (($min == 0) && (strlen($str) < 1)) {
         return true;
      }
      if (!preg_match("/^[\d]{6}[-\s][\d]{4}$/", $str)) {
         $this->error[$id] = 'Godkänt format: 000000-0000 och 000000 0000';
         return false;
      } else return true;
   }
   // }}}
   // dateFormat {{{
   /**
    * Checks for valid date Format xxxx-xx-xx
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    */
   public function dateFormat($id, $min=0)
   {
      $str = $this->str[$id];

      if(($min == 0) && (strlen($str) < 1)) {
         return true;
      }
      if(!preg_match('/^\d{4}\-\d{2}\-\d{2}$/',$str)) {
         $this->error[$id] = 'Du måste använda formatet YYYY-MM-DD';
         return false;
      } else return true;      
   }
   // }}}
   // uri {{{
   /**
    * Check for valid url http://x.x
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    if the field is required to be filled
    */
   public function uri($id, $min, $max)
   {
      $str = $this->str[$id];

      if($this->length($id, $str, $min, $max) === false) {
         return false;
      }
      if(!preg_match('/^http\:\/\/.+$/',$str)) {
         $this->error[$id] = 'Du måste skriva en korrekt url '.
            '(ex http://www.php.net)';
         return false;
      } else {
         return true;
      }
   }
   // }}}
   // sessid {{{
   /**
    * Check for valid session id
    *
    * @param string id  session_id()
    */
   public function sessid($id)
   {

      if(!preg_match('/^[\w\d]{32,32}$/i',$id)) {
         return false;
      } else {
         return true;
      }
   }
   // }}}
   // getAll {{{
   /**
    * Return error array
    */
   public function getAll()
   {
      if (! count($this->error)) {
         return false;
      }
      return $this->error;
   }
   // }}}
   // existErrors {{{
   /**
    * Check if there are any errors
    */
   public function existErrors()
   {
      if(!count($this->error)) {
         return false;
      } else {
         return true;
      }
   }
   // }}}
   // insErr {{{
   /**
    * Insert custom error
    *
    */
   public function insErr($id,$str)
   {
      $this->error[$id] = $str;
      return false;
   }

   // }}}
// phone {{{
   /**
    * Checks string for numeric characters
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param int min    minimum amount of chars
    * @param int max    maximum amount of chars
    */
   public function phone($id,$min=4,$max=65535)
   {
      $str = $this->str[$id];
      if($min == 0)
         return true;

      if (substr($str,0,1)=='+') {
         $str = substr($str,1);
         if(!preg_match('/^[\d\s\-]{'.$min.','.$max.'}$/', $str) ) {
            $this->error[$id] = 'Använd mellan '.$min.' och '.$max.
                                ' siffror, mellanslag och plustecken';
            return false;
         } else return true;
      } else {
        $this->error[$id] = 'Första tecknet måste vara ett \'+46\' eller annan landskod';
        return false;
      }
   }
   // }}}
// phone {{{
   /**
    * Matches to fields that they are the same value
    * Return true if the string is valid.
    * Else return false and put a error mark in $error variable
    *
    * @param string id  identifier
    * @param string id2  identifier
    */
   public function match($id,$id2)
   {
      $str = $this->str[$id];
      $str2 = $this->str[$id2];

      if ($str == $str2) 
          return true;
      
      $this->error[$id] = 'Matchar inte det andra fältet';
   }
   // }}}
   // swepnr {{{
   /**
    * Checks swedish personal number
    *
    */

   function swepnr($id)
   {  
      $str = $this->str[$id];
      
      if ( ( !preg_match("/^[\d]{6}[-\s][\d]{4}$/", $str) && strlen($str)==11) || ( !preg_match("/^[\d]{10}$/", $str) && strlen($str)==10) || strlen($str)< 10 || strlen($str) > 11) {
         $this->error[$id] = 'Godkända format: ååmmddxxxx, ååmmdd-xxxx och ååmmdd xxxx';
         return false;
      } 
      
      if (strlen($str)==11)
         $pnr = substr($str,0,6).substr($str,7);
      else
         $pnr = substr($str,0);

      $n = 2;
      $sum = 0;

      for ($i=0; $i<9; $i++) 
      {
         $tmp = $pnr[$i] * $n;
         ($tmp > 9) ? $sum += 1 + ($tmp % 10) : $sum += $tmp;
         ($n == 2) ? $n = 1 : $n = 2;
      }

      if (( ($sum + $pnr[9]) % 10 )) {
         $this->error[$id] = 'Icke giltigt svenskt personnummer!';
         return false;
      }

      $this->str[$id] = $pnr;

      return true;
   }

}
?>
