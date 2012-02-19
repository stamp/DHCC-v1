<?php

class accesscard extends module {
    
    function __construct() {
        $this->setId(get_class($this));
        $this->setName('Access cards');
        $this->setDescription('A global adressbook');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('path pathadmin safety');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install() {
        return parent::install();
    }
    
    function _printOut() {
        
        if (isset($_POST['search'])) {
            $search = explode(',',$_POST['search']);
            
            $hits = array();

            foreach ($search as $line) {
                if($users = self::users($line))
                    $hits = array_merge($hits,$users);
            }
            
            if ($hits) {
                $hits = db::fetchAll("SELECT * FROM users JOIN user_eventinfo USING (uid) WHERE uid IN (".implode($hits,',').")");

                global $tpl;
                $tpl->assign('hits',$hits);
                self::makePdf($hits);
            }
        }

        $this->display('search.tpl');


    }

    function makePdf($users) {
        $doc = new pdf();

        $doc->create('P','cm',array(5.39,8.57));
        $doc->SetFillColor(200);
        $doc->SetTextColor(255,255,255);
        
        $doc->Rect(0,0,5.39,8.57,'F');
        
        $doc->Cell(5.39,1,'SUPPORT',0,0,'C');

        $doc->save('test.pdf');

    }

    function users($line) {
        switch(substr($line,0,1)) {
            case 'U':
                if (substr($line,0,2) == 'U-')
                    return false;

                if (is_numeric(substr($line,1)))
                    return array(substr($line,1));

                return false;
            case 'G':
                if (is_numeric(substr($line,1))) 
                    if($groups = self::getChilds(substr($line,1))) {
                        return db::fetchAllOne("SELECT uid FROM membership WHERE gid IN (".implode($groups,',').")");
                    }

                return false;
            case 'F':
        }
    }

    function getChilds($gid,$add = true) {
        if ($groups = db::fetchAllOne("SELECT gid FROM structure WHERE parent=$gid AND NOT (LEFT(name,1)='-')")) 
            foreach ($groups as $line)
                if ($childs = self::getChilds($line,false))
                    $groups = array_merge($groups,$childs);

        if ($add)
            if (is_array($groups))
                $groups = array_merge($groups,array($gid));
            else
                $groups = array($gid);

        return $groups;
    }
}

?>
