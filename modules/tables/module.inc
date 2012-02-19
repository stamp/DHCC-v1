<?php

class tables extends module {

    private $link;
    private $sql;
    private $sort;
    public $get;

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Tables manager');
        $this->setDescription('managing and printing tables');
        $this->setVersion('0.0.1');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setRequiredDbApi('2.0');
        $this->setMaintainer('Jonas Falck <jonaz@jonaz.net>');


        parent::module();
    }

    function install () 
    {
        parent::install();
        return true;
    }

    function uninstall () 
    {
        parent::uninstall();
    }

   function init($default, $text_by='sortby', $text_order='sortorder', $alias='')
   {
      $sortby = !isset($_GET[$text_by]) || !array_key_exists($_GET[$text_by], $default)
                ? key($default) : $_GET[$text_by];
      $sortorder = !isset($_GET[$text_order]) || !in_array($_GET[$text_order], array('ASC', 'DESC'))
                   ? current($default) : $_GET[$text_order];

      $sql_alias = array();
      $alias_nr = 0;
      foreach ($default as $key => $val) {
         if (empty($alias)) {
            $sql_alias[$key] = '';
         } elseif (!isset($alias[$alias_nr])) {
            $sql_alias[$key] = $alias[0].'.';
         } else {
            $sql_alias[$key] = empty($alias[$alias_nr]) ? '' : $alias[$alias_nr].'.';
         }
         $alias_nr++;
      }
      unset($default[$sortby]);
      $this->link = array_merge(array($sortby=>$sortorder), $default);
 
      foreach ($this->link as $key => $val) {
         $this->sql[$key] = $sql_alias[$key]."'$key' $val";
      }
      $this->sql = implode(", ", $this->sql);
      $this->link[$sortby] = $this->link[$sortby] == 'ASC' ? 'DESC' : 'ASC';
      $this->sort[$sortby] = $this->link[$sortby] == 'ASC' ? 'DESC' : 'ASC';
   }
   
   function get_sortorder ($sortby) {
      return array_key_exists($sortby, $this->link) ? $this->link[$sortby] : 'ASC';
   }
   
   function get_sqlorder () {
      return $this->sql;
   }

   function create($array,$options = NULL,$id=NULL){
        global $tpl;

        if(isset($options) && is_array($options))
            $tpl->options=$options;

        //Sortera en databas fråga
        if(!is_array($array)){
            if($data = $this->fetchSingle($array)) {
                foreach($data AS $key => $line){
                        $initarray[''.$key.''] = 'ASC'; 

                }
                $this->init($initarray,$id.'by',$id.'order');
                foreach($data AS $key => $line){
                    $this->get[$id.'by'] = $key;
                    $this->get[$id.'order'] = $this->get_sortorder($key);
                    $headsarray[$key] = http_build_query($this->get);
                }

                $tpl->heads=$headsarray ;
                $array .= ' ORDER BY '.$this->get_sqlorder();
                if($data = $this->fetchAll($array))
                    $tpl->data=$data;

                $this->display('create.tpl.php');
                return $data;
            }
        }
        //Sortera en array
        elseif(is_array($array)){
            if (!(isset($array[0])&&is_array($array[0])))
                return false;

            foreach($array[0] AS $key => $line){
                if (!isset($this->initarray[''.$key.'']))
                    $this->initarray[''.$key.''] = 'ASC';

            }

            $this->init($this->initarray,$id.'by',$id.'order');

            foreach($array[0] AS $key => $line){
                $this->get[$id.'by'] = $key;
                $this->get[$id.'order'] = $this->get_sortorder($key);
                $headsarray[$key] = http_build_query($this->get);
                $type[$key] = is_numeric($line) ? 'SORT_NUMERIC' : 'SORT_STRING';
            }

            $tpl->heads=$headsarray;
            
            $res = array();

            // Obtain a list of columns
            foreach ($array as $key => $row)
                foreach ($row as $key2 => $line){
                    if (!isset($res[$key2])) $res[$key2] = array();
                    $res[$key2][$key]  = $line;
                }
            
            
            $run = "array_multisort(";
                foreach ($this->sort as $key => $l)
					if (isset($type[$key]) )
                    	$run .= '$res[\''.$key.'\'], SORT_'.$l.", {$type[$key]},";

            $run .='$array);';
            eval($run);

            $tpl->data=$array;
            
            $this->display('create.tpl.php');

            return $array;
        }
    }
}
?>
