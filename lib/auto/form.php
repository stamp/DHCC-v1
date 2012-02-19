<?php
/* vim: set expandtab tabstop=3 shiftwidth=3: */
// +--------------------------------------------------------+   
// | PHP Version 5.x                                        |
// +--------------------------------------------------------+
// | Filename: form.class.inc                               |
// +--------------------------------------------------------+
// | Copyright (c) 2004 Joel Hansson                        |
// +--------------------------------------------------------+
// | License: GPL                                           |
// +--------------------------------------------------------+
// | Author: Joel Hansson <joel@everlast.se>                |
// +--------------------------------------------------------+
//
//

class Form {

   // Properties {{{
   public    $action;
   protected $err;
   public    $val;
   protected $id;
   protected $rtearea;

   // }}}
   // __construct {{{
   /**
    * Constructor. Sets the value and error arrays.
    * Define a possible id for the form.
    *
    * @param   string   $f    name of the actionfile *required*
    * @param   array    $v    associative array with all values
    * @param   array    $e    associative array with all errors
    * @param   string   $id   id of the form
    * @return  form object
    */
   public function __construct($f, $v = array(), $e = array(), $id = null)
   {
      $this->action = $f;
      $this->val    = $v;
      $this->err    = $e;

      if (! empty($id) ) {
         $this->id     = ' id="'.$id.'" ';
      } else {
         $this->id = '';
      }
         

      $this->onfocus = ' onfocus="this.className=\'hi\'" ';
      $this->onblur  = ' onblur="this.className=\'blur\';" ';
      return true;
   }

   // }}}
   // text {{{
   /**
    * 
    *
    */
   public function text($name, $disp = null, $extra = null)
   {
      if (! empty($extra['first']) ) {
         $class = 'class="first"';
      } else $class = '';
      if (! empty($extra['ro']) ){
         $ro = ' readonly="readonly" ';
      } else $ro = '';
      if (isset($extra['onclick']) ) {
         $onclick = ' onLoad="'.$extra['onclick'].'" ';
      } else $onclick = '';
      if (isset($extra['id']) ) {
         $id = ' id="'.$extra['id'].'" ';
      } else $id = '';
      
      $str  = '    <label '.$class.'>'.$disp."\n";


      $str .= '      <input type="text" name="'.$name.'"';
      $str .= $id.$this->onfocus.$this->onblur.$ro.$onclick;
      if (isset($this->val[$name])) {
         $str .= 'value="'.$this->val[$name].'" />'."\n";
      } else $str .= 'value="" />'."\n";


      if (isset($this->err[$name]) ) {
         $str .= '      <span>'.$this->err[$name].'</span>'."\n";
      }


      $str .= '    </label>'."\n";

      return $str;
   }

   // }}}
   // passwd {{{
   /**
    *
    *
    *
    */
   public function passwd($name, $disp = null,$incVals = false)
   {
      $str  = '    <label>'.$disp."\n";
      $str .= '      <input type="password"';
      if ($incVals) {
         if (isset($this->val[$name])) {
            $str .= 'value="'.$this->val[$name].'" '."\n";
         } else $str .= 'value="" '."\n";
      }
      $str .= $this->onfocus.$this->onblur;
      $str .= 'name="'.$name.'" />'."\n";

      if (isset($this->err[$name]) ) {
         $str .= '      <span>'.$this->err[$name].'</span>'."\n";
      }

      $str .= '    </label>'."\n";


      return $str;
   }

   // }}}
   // submit {{{
   /**
    *
    *
    *
    */
   public function submit($disp)
   {
      if (isset($this->rtearea)) {
         $onclick = ' onclick="submitForm(\''.$this->rtearea.'\')" ';
      } else $onclick = '';
      //$str  = '    <label class="submit">'."\n";
      $str = '      <input type="submit" name="button" class="submit" ';
      $str .= 'value="'.$disp.'" '.$onclick.'/>'."\n";
      //'    </label>'."\n";
      
      return $str;
   }

   // }}}

   // textarea {{{
   /**
    *
    *
    *
    */
   public function textarea($name, $disp = null, $cols = 30, $rows = 10,$style = '')
   {
      $str  = '<label class="tarea"> '.$disp."\n";
      if (isset($this->err[$name]) ) {
         $str .= '<span>'.$this->err[$name].'</span>'."\n";
      }
      $str .= '<textarea cols="'.$cols.'" rows="'.$rows.'"';
      $str .= $this->onfocus . $this->onblur;
      if ($style>'') $str .= 'style="'.$style.'" ';
      $str .= 'name="'.$name.'">';
      if (isset($this->val[$name])) {
         $str .= $this->val[$name];
      }
      $str .= '</textarea>'."\n";

      $str .= '</label>'."\n";

      return $str;
   }

   // }}}
   // editor {{{
   /**
    *
    *
    *
    */
   public function editor($name,$disp = null,$cols = 30, $rows = 10,$style = '')
   {
      $str  = '<label class="tarea"> '.$disp."\n";
      if (isset($this->err[$name]) ) {
         $str .= '<span>'.$this->err[$name].'</span>'."\n";
      }

      if(!$tinymce = core::load('tinymce'))
         return send(E_ERROR, 'Can´t load module "tinymce"');
      $tinymce->init('wordlike');
      unset($tinymce);


      $str .= '<textarea class="editor" cols="'.$cols.'" rows="'.$rows.'"';
      $str .= $this->onfocus . $this->onblur;
      if ($style>'') $str .= 'style="'.$style.'" ';
      $str .= 'name="'.$name.'">';
      if (isset($this->val[$name])) {
         $str .= $this->val[$name];
      }
      $str .= '</textarea>'."\n";

      $str .= '</label>'."\n";

      return $str;


   }

   // }}}
   // simple {{{
   /**
    *
    *
    *
    */
   public function simple($name,$disp = null,$cols = 30, $rows = 10,$style = '')
   {
      $str  = '<label class="tarea"> '.$disp."\n";
      if (isset($this->err[$name]) ) {
         $str .= '<span>'.$this->err[$name].'</span>'."\n";
      }

      if(!$tinymce = core::load('tinymce'))
         return send(E_ERROR, 'Can´t load module "tinymce"');
      $tinymce->init('simple');
      unset($tinymce);


      $str .= '<textarea class="simple" cols="'.$cols.'" rows="'.$rows.'"';
      $str .= $this->onfocus . $this->onblur;
      if ($style>'') $str .= 'style="'.$style.'" ';
      $str .= 'name="'.$name.'">';
      if (isset($this->val[$name])) {
         $str .= $this->val[$name];
      }
      $str .= '</textarea>'."\n";

      $str .= '</label>'."\n";

      return $str;


   }

   // }}}
   // rtearea {{{
   /**
    *
    *
    *
    */
   public function rtearea($name,$disp = null)
   {
      $this->rtearea = $name;
      if (isset($this->val[$name]) ) {
         $text = preg_replace("/(\n|\r\n|\r)/", '', $this->val[$name]);
      } else $text = '';

      $str  = '<noscript><p><b>javascript must be enabled to use this form.</b>'.
              '</p></noscript>'."\n";
      $str .= '<script language="JavaScript" type="text/javascript">'."\n";
      $str .= "<!--\n";
      $str .= "//Usage: writeRichText(fieldname, html, width, ".
              "height, buttons, readOnly)\n";
      $str .= 'writeRichText(\''.$name.'\', \''.$text.'\', '.
              "400, 400, true, false);\n";
      $str .= "//-->\n";
      $str .= "</script>\n";
      
      if (! empty($this->err[$name]) ) {
         $str .= '<label><span>'.$this->err[$name].'</span></label>'."\n";
      }

      return $str;


   }

   // }}}

   // hidden {{{
   /**
    *
    *
    *
    */
   public function hidden($name,$value = null)
   {
      $str  = '<input type="hidden" class="hidden" ';
      $str .= 'name="'.$name.'" value="';
      if ($value != null) 
        $str .= $value;
      else 
          if (isset($this->val[$name])) $str .=  $this->val[$name];
      $str .= '" />'."\n";
      
      return $str;
   }
   // }}}
   // checkbox {{{
   /**
    *
    *
    *
    */

   public function checkbox($name,$disp = null)
   {
      if(isset($this->val[$name]) && $this->val[$name]) {
         $checked = 'checked="checked"';
      } else $checked = '';

      
      
      
      $str  = '<label>'.$disp."\n";
      $str .= '<input type="checkbox" class="checkbox" ';
      $str .= 'name="'.$name.'" value="1" ';
      $str .= $checked.' />'."\n";
      $str .= "</label>\n";

      return $str;
   }

   // }}}
   // select {{{
   /**
    *
    *
    *
    */
   public function select($name, $disp = null,$fields, $extra = null)
   {
      if (isset($extra['onchange'])) {
         $onchange = ' onchange="'.$extra['onchange'].'" ';
      } else $onchange = '';
      if (isset($extra['class'])) {
         $class = ' class="'.$extra['class'].'" ';
      } else $class = '';

      if (! empty($extra['ro']) ){
         $ro = ' disabled="disabled" ';
      } else $ro = '';
      $str  = '<label'.$class.'> '.$disp."\n";
      if (isset($extra['style'])) {
         $style = ' style="'.$extra['style'].'" ';
      } else $style = '';
      $str .= '<select  name="'.$name.'" '.$onchange.$ro.$class.$style.'>'."\n";
      
      if (! isset($fields) || ! is_array($fields) ) {
         $str .= '</select></label>'."\n";
         return $str;
      }
      
      foreach ($fields as $v) {
         $str .= '<option value="'.$v['val'].'" ';
         
         if (isset($v['style'])) {
            $str .= 'style="'.$v['style'].'" ';
         }        
         if (isset($this->val[$name])&&$v['val']==$this->val[$name]) {
            $str .= 'selected="selected" ';
         }
         if (isset($v['class'])) $str .= ' class="'.$v['class'].'" ';
         $str .= '>';
         $str .= $v['text'].'</option>'."\n";
      }

      $str .= '</select>'."\n";
if (! empty($this->err[$name]) ) {
         $str .= '<span>'.$this->err[$name].'</span>'."\n";
      }
        $str .= '</label>';

      return $str;

   }

   // }}}
   // radio {{{
   /**
    *
    *
    *
    */
   public function radio($name, $disp = null,$fields, $extra = null)
   {
      if (isset($extra['onchange'])) {
         $onchange = ' onchange="'.$extra['onchange'].'" ';
      } else $onchange = '';
      if (isset($extra['class'])) {
         $class = ' class="'.$extra['class'].'" ';
      } else $class = '';

      $str  = '<span '.$class.'> '.$disp."</span>\n";
      
      if (! isset($fields) || ! is_array($fields) ) {
         $str .= ''."\n";
         return $str;
      }
      $str .= '<br>';
      foreach ($fields as $v) {
         $str .= '<input type="radio" name="'.$name.'" value="'.$v['val'].'" ';
         
         $str .= 'style="display:inline;width:auto;height:auto;';
         if (isset($v['style'])) {
            $str .= $v['style'];
         }
         $str .= '" ';
         if (isset($this->val[$name])&&$v['val']==$this->val[$name]) {
            $str .= ' checked ';
         }
         if (isset($v['class'])) $str .= ' class="'.$v['class'].'" ';
         $str .= '>'.$v['text'].'<br>';
      }

      $str .= '</label>'."\n";

      return $str;

   }

   // }}}
   // file {{{
   /**
    *
    *
    *
    */
   public function file($name,$disp = null,$extra = null)
   {
      if (isset($extra['style']) ) {
         $style = ' style="'.$extra['style'].'" ';
      } else $style = '';

      $str  = '<label> '.$disp."\n";
      $str .= '<input type="file" name="'.$name.'" class="chkbox"'.$style.'/>'."\n";
      $str .= '</label>'."\n";

      return $str;
   }

   // }}}
   // start {{{ 
   /**
    *
    *
    *
    */
   public function start($title = null,$extra = NULL)
   {
      if(isset($extra['type'])){
        if($this->action != "NONE")
            $str  = '<form method="'.$type.'"" ';
        else
           $str='<form ';
      }
      else{
        if($this->action != "NONE")
            $str  = '<form method="post"';
        else
           $str='<form ';
      }
      if($this->action != "NONE")
        $str .= ' action="'.$this->action.'" ';
      $str .= $this->id;
      if(isset($extra['onsubmit']))
      $str .= ' onSubmit="'.$extra['onsubmit'].'" ';
      if($this->action != "NONE")
        $str .= 'enctype="multipart/form-data"'."\n";
      $str .= '>';

      //$str .= '  <fieldset>'."\n";

      if (! empty($title) ) {
         $str .= '  <legend>'.$title.'</legend>'."\n";
      }

      return $str;
   }

   // }}}
   // clear {{{
   /**
    * @return string a breaking <br tag
    */
   public function clear()
   {
      $str = '<br class="clear"/>';
      return $str;
   }

   // }}}
   // stop {{{
   /**
    * @return string the endtags of the form
    */
   public function stop()
   {

      // unset all vars
      $this->action = '';
      $this->val    = array();
      $this->err    = array();
      
      $str  = '';
      //$str .= '  </fieldset>'."\n";
      $str .= '</form>'."\n".'<p class="clear"/>'."\n";

      return $str;
   }
   
   // }}}
   // secure {{{
   /**
    * @return string the generated hidden field with the safety hash
    */
    public function secure() {
        if (class_exists('safety')) {
            if (!class_exists('safety'))
                return false;

            $this->val['hash'] = safety::hash();
            return $this->hidden('hash');
        }
    }
   // }}}
}
?>
