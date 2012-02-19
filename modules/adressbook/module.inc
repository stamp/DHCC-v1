<?php

class adressbook extends module {
    
    function __construct() {
        $this->setId(get_class($this));
        $this->setName('Adress book');
        $this->setDescription('A global adressbook');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('path pathadmin safety');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install() {
        $tables = array (
            'adresses' => array(
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
                    'Field'   => 'field',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'owner',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'value',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'type',
                    'Type'    => "enum('global','public','personal')",
                    'Default' => 'personal'
                )
            )
        );

        $this->installTables($tables);
        
        return parent::install();
    }

    function _globalSearch() {
        global $tpl;
    
        if (isset($_POST['search'])) {
            if ( $hits = self::doSearch($_POST->raw('search')) ) {
                $tpl->assign('search',$hits);
            }
            
        }
        
        $this->display('search.tpl');
    }
    
    
    function _privateContacts() {

    }

    function doSearch($search) {
        $search = db::escapeStr($search);

        if (!$sql = self::doSearchSql(self::getAvaliableSearchFields(), $search) )
            return false;
        
        global $path;
        
        if (!$path->write)
            return db::fetchAll("SELECT username as _username,concat(firstname,' ',lastname),city,group_concat(user_contact.text SEPARATOR '<br />') as primaryphone,picture as _picture,username LIKE '%$search%' as _high FROM users LEFT JOIN user_profile USING (uid) LEFT JOIN user_contact ON user_contact.uid=users.uid AND user_contact.medium IN ('Email','Mobil','Telefon') WHERE users.uid IN (".implode($sql,',').") GROUP BY users.uid ORDER BY NOT username LIKE '%$search%',username" );
        return db::fetchAll("SELECT username as _username,username LIKE '%$search%' as _high,concat(firstname,' ',lastname),birthdate,'&nbsp;',concat('<b>ICE:</b> ',ice),email,street,concat(postcode,' ',city),primaryphone,secondaryphone,picture as _picture FROM users LEFT JOIN user_profile USING (uid) WHERE uid IN (".implode($sql,',').")  ORDER BY NOT username LIKE '%$search%',username");
    }
    
    function doSearchSql($fields,$search) {
        if (!is_array($fields))
            return false;
            
        if (trim($search) == '')
            return false;
        
        foreach ($fields as $field) {
            preg_match("#([a-zA-Z0-9_]+)\.#",$field,$fieldx);
                
            if (!isset($where[$fieldx[1]]))
                $where[$fieldx[1]] = "$field LIKE '%$search%'";
            else
                $where[$fieldx[1]] .= " OR $field LIKE '%$search%'";
        }
        
        if (!isset($where)||count($where)==0)
            return false;
        
        $ret = array();
        
        foreach($where as $key => $line)
            if ($data = db::fetchAllOne("SELECT uid FROM $key WHERE $line"))
                $ret = array_merge($ret,$data);
                
        return $ret;
    }

    function getAvaliableSearchFields() {
        return array(
            "concat(users.firstname,'',users.lastname)",
            'users.birthdate',
            'user_profile.city',
            'users.username',
            'user_profile.street',
            'user_profile.primaryphone',
            'user_profile.secondaryphone',
            'user_contact.text',
            'user_eventinfo.car'
        );
    }
}

?>
