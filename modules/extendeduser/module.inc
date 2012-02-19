<?php

class extendeduser extends module {

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('Extended user manager');
        $this->setDescription('Extended managing users and user accounts');
        $this->setMaintainer('Jonas Falck <jonaz@jonaz.net>');
        $this->setVersion('0.0.2');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('user tables');

        parent::module();
    }

    function install () 
    {

        $tables = array (
            'user_contact' => array(
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
                    'Field'   => 'medium',
                    'Type'    => 'varchar(255)'
                    ),
                array(
                    'Field'   => 'text',
                    'Type'    => 'varchar(255)'
                    ),
                ),
            'user_profile' => array(
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'valid',
                    'Type'    => "enum('Y', 'N')"
                ),
                array(
                    'Field'   => 'share',
                    'Type'    => 'int(2)'
                ),
                array(
                    'Field'   => 'email',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'email2',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'ice',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'country',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'street',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'city',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'city_pos',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'postcode',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'age_at_event',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'primaryphone',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'primaryphontype',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'secondaryphone',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'secondaryphonetype',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'press',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'food',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'medical',
                    'Type'    => 'text'
                ),
                array(
                    'Field'   => 'timezone',
                    'Type'    => 'varchar(255)'
                ),
                array(
                    'Field'   => 'validcontract',
                    'Type'    => "enum('Y', 'N')"
                    ),
                array(
                    'Field'   => 'changedby',
                    'Type'    => 'int(11)'
                    )
                ),
            'user_eventinfo' => array(
                array(
                    'Field'   => 'valid',
                    'Type'    => "enum('Y', 'N')"
                    ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                    ),
                array(
                    'Field'   => 'were',
                    'Type'    => 'varchar(255)'
                    ),
                array(
                    'Field'   => 'on_what',
                    'Type'    => 'varchar(255)'
                    ),
                array(
                    'Field'   => 'with_who',
                    'Type'    => 'varchar(255)'
                    ),
                array(
                    'Field'   => 'sleep_hard',
                    'Type'    => 'smallint(1)'
                    ),
                array(
                    'Field'   => 'wakeup',
                    'Type'    => 'varchar(255)'
                    ),
                array(
                    'Field'   => 'size',
                    'Type'    => "varchar(200)"
                    ),
                array(
                    'Field'   => 'snore',
                    'Type'    => "enum('Ja', 'Halvhögt', 'Ibland', 'Nej', 'Vet ej')"
                    ),
                array(
                    'Field'   => 'gsize',
                    'Type'    => "varchar(200)"
                    ),
                array(
                    'Field'   => 'arrive',
                    'Type'    => 'date'
                    ),
                array(
                    'Field'   => 'arrive_time',
                    'Type'    => 'varchar(10)'
                    ),
                array(
                    'Field'   => 'depart',
                    'Type'    => 'date'
                    ),
                array(
                    'Field'   => 'depart_time',
                    'Type'    => 'varchar(10)'
                    ),
                array(
                    'Field'   => 'car',
                    'Type'    => 'varchar(20)'
                    ),
                array(
                    'Field'   => 'dinner',
                    'Type'    => 'smallint(1)'
                    ),
                array(
                    'Field'   => 'changedby',
                    'Type'    => 'int(11)'
                    ),
                array(
                    'Field'   => 'checkedin',
                    'Type'    => 'datetime'
                    ),
                array(
                    'Field'   => 'checkedout',
                    'Type'    => 'datetime'
                    )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }

    function uninstall () 
    {
        //$this->query("
        //    DROP TABLE user_contact
        //");
        parent::uninstall();
    }
   
    function _manageContacts(){
        global $tpl,$path;
        
        //$admin = in_array('|G73', explode(',', $_SESSION['access']));
        if($_SESSION['id'] == $path->vars['uid'])
            $admin =1;
        else
            $admin = $path->write;
        $v = new validate($_POST);
        if($admin) {
            $userid = $path->vars['uid'];
        }
        else{
            $userid = $_SESSION['id'];
        }

        if (isset($_POST['action'])&&$_POST['action']=='register') {
        $v->length('medium',2,100,true);
        $v->length('text',2,100,true);
        if($this->fetchAll("SELECT id FROM user_contact WHERE medium='".$_POST->sql('medium')."' AND uid='{$userid}'"))
            $v->error['medium']='Du kan bara ha ett av varje medium';
        
        if(!$v->existErrors()) 
	        $this->query("INSERT INTO user_contact (uid,medium,text) VALUES ({$userid},'".$_POST->html('medium')."','".$_POST->html('text')."')");
        }
        elseif (isset($_GET['id'])&&is_numeric($_GET['id'])) 
	        $this->query("DELETE FROM user_contact WHERE id=".$this->escapeStr($_GET['id'])." AND uid={$userid} LIMIT 1");


        $tpl->assign('errors',$v->getAll());
        $tpl->assign('vals',$_POST);
        get();

        $this->display('managecontacts.head.tpl');
        $this->printContacts($userid,$admin);
        $this->display('managecontacts.tpl');
        $this->display('managecontacts.foot.tpl');

    }

     function printContacts($uid,$admin=0){
        $table = core::load('tables');
        if($admin !=0)
            $conc= ", CONCAT('<a href=\"?id=',id,'\">[Ta bort]</a>') AS ' '";
        else
            $conc = '';
        $table->create("SELECT medium AS Medium, text AS Address $conc FROM user_contact WHERE uid='{$uid}'",'','id1');
        unset($table);
    }


    function _editProfile(){
        global $tpl,$path;
        //fixa write här
    //$admin = in_array('|G73', explode(',', $_SESSION['access']));
            $admin = $path->write;
    $v = new validate($_POST);
    
    if(isset($_SESSION['validprofile']) && $_SESSION['validprofile']!='Y') {
        $userid = $_SESSION['id'];
    } elseif($admin) {
        $userid = $path->vars['uid'];
    }
    else{
        $userid = $_SESSION['id'];
    }
    
    $exists = $this->fetchOne('SELECT uid FROM user_profile WHERE uid='.$userid);
    if (isset($_POST)&&count($_POST)>0&&!isset($_POST['username'])) {

        $v->email('email');
        $v->length('street',5,255);
        $v->length('city',2,255);
        $v->length('postcode',3,15);
        $v->phone('primaryphone');
        $v->phone('secondaryphone',0,100);
        $v->phone('ice');
    
        // list of valid fields
        $validfields = array('ice','city','medical','food','share','email','email2','street','postcode','country','primaryphontype','primaryphone','secondaryphonetype','secondaryphone');
        //if ($admin || !$exists) {
        //    $validfields = array_merge($validfields, array('firstname','lastname'));
        //}
        if(!isset($_POST['share']))
            $_POST['share']= 0;
        // change only the valid fields
        $vals2 = array();
        foreach ($validfields as $va) {
            $vals2[$va] = $_POST[$va];
        }
        $vals2['changedby'] = $_SESSION['id'];
    
        // if there are no errors update the database
        if(!$v->ExistErrors()) {
            $tags = array('br','em','strong','h1','h2','h3','h4','h5','h6','b','i');
            $filter = new InputFilter($tags);
            $vals2 = $this->escapeStr($filter->process($vals2));

            if ($exists) {
                $vals2['valid'] = 'Y';
                $this->update($vals2,'user_profile','WHERE uid='.$userid);
                if ($_SESSION['validprofile']!='Y') {
                    $_SESSION['validprofile']='Y';
                    echo '<script language="javascript">document.location.href=\'/\';</script>';
                }
            } else {
                $vals2['uid'] = $userid;
                $vals2['valid'] = 'Y';
                $this->insert($vals2,'user_profile');
                if ($_SESSION['validprofile']!='Y') {
                    $_SESSION['validprofile']='Y';
                    echo '<script language="javascript">document.location.href=\'/\';</script>';
                }
            }
            // re-fetch values from database
            send(E_USER_NOTICE,"Ändringar sparade!");
            $vals = $this->fetchSingle("SELECT * FROM users JOIN user_profile USING (uid) WHERE uid=".$userid);
        } else {
            // display user-entered values
            send(E_USER_ERROR,"Du har inte fyllt i alla obligatoriska fält! Vänligen fyll i alla rödmarkerade fält");
            $vals = $_POST;
        }
    } else {
        // fetch values from database
        $vals = $this->fetchSingle("SELECT * FROM users JOIN user_profile USING (uid) WHERE uid=".$userid);
    }

    $tpl->assign('countrys',$this->fetchAll("SELECT CASE `flag` WHEN '' THEN 'padding-left:33px;' ELSE CONCAT(\"background: url('/template/images/flags/\",flag,\"') no-repeat;padding-left:33px;height:16px;\") END AS style,name as text,id as val FROM countrys ORDER BY name"));
    $tpl->assign('user',$this->fetchSingle('SELECT username,uid FROM users WHERE uid='.$userid));
    $tpl->assign('errors',$v->getAll());
    $tpl->assign('vals',$vals);
    $tpl->assign('lock',($userid!=$_SESSION['id']||!$exists));
    get();
    $this->display('editprofile.tpl');
    }

    
    function _editEventinfo(){
        global $tpl,$path;
        //fixa write här 
    //$admin = in_array('|G73', explode(',', $_SESSION['access']));
    $admin = $path->write;
    $v = new validate($_POST);
    
    if(isset($_SESSION['valideventinfo'])&&$_SESSION['valideventinfo']!='Y') {
        $userid = $_SESSION['id'];
    } elseif($admin ) {
        $userid = $path->vars['uid'];
    }
    else{
        $userid = $_SESSION['id'];
    }

    
    $exists = $this->fetchOne('SELECT uid FROM user_eventinfo WHERE uid='.$userid);


    if (isset($_POST)&&count($_POST)>0&&!isset($_POST['username'])) {
        $v->dateFormat('arrive',1);
        $v->dateFormat('depart',1);
        $v->length('arrive_time',1,10);
        $v->length('depart_time',1,10);
        $v->length('car',0,10);
        if(isset($_POST['size']))
            $v->length('size',1,10);
        if(isset($_POST['gsize']))
            $v->length('gsize',1,10);
        //$v->length('wakeup',2,100);
        $v->length('with_who',0,100);
    
        // list of valid fields
        $validfields = array('size','gsize','were','car','arrive','depart','arrive_time','depart_time','sleep_hard','on_what','with_who','wakeup','snore','dinner');
        if(!isset($_POST['dinner']))
            $_POST['dinner']= 0;
        if(!isset($_POST['sleep_hard']))
            $_POST['sleep_hard']= 0;
        // change only the valid fields
        $vals2 = array();
        foreach ($validfields as $va) {
            if(isset($_POST[$va]))
            $vals2[$va] = $_POST[$va];
        }
        $vals2['changedby'] = $_SESSION['id'];
    
        // if there are no errors update the database
        if(!$v->ExistErrors()) {
            $tags = array('br','em','strong','h1','h2','h3','h4','h5','h6','b','i');
            $filter = new InputFilter($tags);
            $vals2 = $this->escapeStr($filter->process($vals2));
            if ($exists) {
                $vals2['valid'] = 'Y';
                $this->update($vals2,'user_eventinfo','WHERE uid='.$userid);
                if ($_SESSION['valideventinfo']!='Y') {
                    $_SESSION['valideventinfo']='Y';
                    echo '<script language="javascript">document.location.href=\'/\';</script>';
                }
            } else {
                $vals2['uid'] = $userid;
                $vals2['valid'] = 'Y';
                $this->insert($vals2,'user_eventinfo');
                if ($_SESSION['valideventinfo']!='Y') {
                    $_SESSION['valideventinfo']='Y';
                    echo '<script language="javascript">document.location.href=\'/\';</script>';
                }
            }
            // re-fetch values from database
            send(E_USER_NOTICE,"Ändringar sparade!");
            $vals = $this->fetchSingle("SELECT * FROM users JOIN user_eventinfo USING (uid) WHERE uid=".$userid);
        } else {
            send(E_USER_ERROR,"Du har inte fyllt i alla obligatoriska fält! Vänligen fyll i alla rödmarkerade fält");
            // display user-entered values
            $vals = $_POST;
        }
    } else {
        // fetch values from database
        $vals = $this->fetchSingle("SELECT * FROM users JOIN user_eventinfo USING (uid) WHERE uid=".$userid);
    }

    $tpl->assign('user',$this->fetchSingle('SELECT username,uid AS id FROM users WHERE uid='.$userid));
    $tpl->assign('errors',$v->getAll());
    $tpl->assign('vals',$vals);
    $tpl->assign('lock',($userid!=$_SESSION['id']||!$exists));
    get();
    $this->display('editeventinfo.tpl');

    }

    function _showProfile(){
        global $tpl,$path;
        $tpl->assign('write',$path->write);
        
        if ($tpl->write) {
            if (isset($_GET['checkin'])) {
                db::query("UPDATE user_eventinfo SET checkedin=NOW() WHERE uid=".$path->vars['uid']);
                logg("Checked in user ".$path->vars['uid']);
                send(E_USER_NOTICE,"Användare Incheckad!");
            } elseif (isset($_GET['checkout'])) {
                db::query("UPDATE user_eventinfo SET checkedout=NOW() WHERE uid=".$path->vars['uid']);
                logg("Checked out user ".$path->vars['uid']);
                send(E_USER_NOTICE,"Användare Utcheckad!");
            }
        }

        $data = $this->fetchSingle("SELECT * FROM users WHERE uid=".$path->vars['uid']);
        $tpl->assign('userinfo',$data);
        $tpl->assign('userhead',user::short($path->vars['uid']));
        
        if (strlen($tpl->userinfo['picture'])==32) {
            $tpl->userinfo['picture'] = '/images/users/thumbs/thumb_'.$tpl->userinfo['picture'].'.jpg';
        } else {
            $tpl->userinfo['picture'] = '/images/users/old/'.$tpl->userinfo['picture'];
        }

        $tpl->assign('user_profile',$this->fetchSingle("SELECT * FROM user_profile WHERE uid=".$path->vars['uid']));
        $history = $this->fetchAll("SELECT structure.gid,structure.name,events.shortname,is_team 
                    FROM membership 
                    JOIN structure ON structure.gid = membership.gid 
                    JOIN events ON structure.event = events.id AND event!='".$_SESSION['event']."' 
                    WHERE uid={$path->vars['uid']} AND structure.name NOT LIKE '-%' ORDER BY events.start");
        $array=array();
        if(isset($history) && is_array($history))
        foreach($history AS $key=>$line){
            if($gid =teams::getTeamFromChildGroup($line['gid'])){
                $history[$key]['name'] = $this->fetchOne("SELECT name FROM structure WHERE gid=".$gid."");
                if($status=$this->fetchOne("SELECT name from structure JOIN membership ON structure.gid=membership.gid AND membership.uid='".$path->vars['uid']."' WHERE parent='".$gid."' AND name LIKE '-%'"))
                    $history[$key]['level'] = substr($status,1,strlen($status)); 
                }
                else
                    unset($history[$key]);
            }

        $tpl->assign('history',$history);

        $data = $this->fetchAll("SELECT structure.gid FROM structure
                    JOIN membership ON structure.gid = membership.gid AND uid={$path->vars['uid']} 
                    WHERE event='{$_SESSION['event']}' GROUP BY gid");
        $array=array();
        if(isset($data) && is_array($data))
        foreach($data AS $key=>$line){
            if($gid =teams::getTeamFromChildGroup($line['gid'])){
                if(!in_array($gid,$array))
                    array_push($array,$gid);
            }
        }
        foreach($array AS $key=>$line){
                $data1[$key]['name'] = $this->fetchOne("SELECT name FROM structure WHERE gid=".$line."");
                if($status=$this->fetchOne("SELECT name from structure JOIN membership ON structure.gid=membership.gid AND membership.uid='".$path->vars['uid']."' WHERE parent='".$line."' AND name LIKE '-%'"))
                    $data1[$key]['level'] = substr($status,1,strlen($status)); 
        }
        if(isset($data1) && is_array($data1))
        $tpl->assign('memberteams',$data1);

        $tpl->assign('contact',$this->fetchAll("SELECT * FROM user_contact WHERE uid={$path->vars['uid']}"));
        $tpl->assign('eventinfo',$this->fetchSingle("SELECT * FROM user_eventinfo WHERE uid={$path->vars['uid']}"));
        $this->display('profile.show.tpl');

    }
    
    function _search(){
        if(isset($_GET['ajax']) && $_GET['ajax']== 1){
            global $tpl;
            $k = trim($this->escapeStr($_POST['value']));
            $tpl->assign('string',$k);
            $data = $this->fetchAll("SELECT username,firstname,lastname,city,uid 
                            FROM users 
                            LEFT JOIN user_profile 
                            USING (uid)
                            WHERE (username LIKE '%".utf8_decode($k)."%' 
                                OR CONCAT(firstname,' ', lastname) LIKE '%".utf8_decode($k)."%' 
                                OR city LIKE '%".utf8_decode($k)."%')
                            ORDER BY username");
            foreach($data AS $key=>$line){
                $data[$key]['uid'] = user::short($line['uid']);
            }

            $tpl->assign('data', $data);
    
            $this->display('autocomplete.tpl');
        }
        elseif(isset($_GET['ajaxsearch'])){

            if(isset($_POST['find']))
                $_GET['find'] = $_POST['find'];
            elseif(isset($_GET['find']))
                $_POST['find'] = $_GET['find'];
            if(isset($_POST['this']))
                $_GET['this'] = $_POST['this'];
            elseif(isset($_GET['this']))
                $_POST['this'] = $_GET['this'];
        
            if(isset($_POST['find'])){
                $k = $this->escapeStr($_POST['find']);
                $table = core::load('tables');
                if(isset($_POST['this']))
                    $and = "AND event='".$_SESSION['event']."'";
                else
                    $and = '';
                //GROUP_CONCAT(IF(event=".$_SESSION['event'].",'<b>',''),structure.name,'(',shortname, IF(event=".$_SESSION['event'].",'</b>',''),')' ORDER BY events.start SEPARATOR '<br />') AS Team
                if($data = $this->fetchAll("SELECT uid AS Nick,CONCAT(firstname,' ',lastname) AS Namn,email AS 'Primär E-post',city  AS 'Från',username,
                            GROUP_CONCAT(structure.gid ORDER BY events.start)
                            AS 'Team'
                            FROM users 
                            LEFT JOIN user_profile 
                            USING (uid)
                            LEFT JOIN membership
                            USING(uid)
                            LEFT JOIN structure
                            ON structure.gid=membership.gid
                            LEFT JOIN events
                            ON events.id = structure.event
                            WHERE ((username LIKE '%".utf8_decode($k)."%' 
                            OR CONCAT(firstname,' ', lastname) LIKE '%".utf8_decode($k)."%' 
                            OR city LIKE '%".utf8_decode($k)."%')$and) GROUP BY uid ")){

                    if(count($data) == 1)
                        echo '<script language="javascript">document.location.href=\'/users/'.path::encode($data['0']['username']).'.htm\';</script>';
                        //header("Location: /users/".path::encode($data['0']['Nick'])."");
                $cnt=0;
                    foreach($data AS $key=>$line){
                        $data[$key]['Namn'] = '<a href="/users/'.path::encode($line['username']).'.htm">'.$line['Namn'].'</a>';
                        $data[$key]['Nick'] = user::short($line['Nick']);
                        $data[$key]['Team'] =NULL;
                        $array=array();
                        foreach(explode(',',$line['Team']) AS $line4){
                            if($gid =teams::getTeamFromChildGroup($line4)){
                                if(!in_array($gid,$array))
                                    array_push($array,$gid);
                            }
                        }
                        if(count($array) >1){

                            $data[$key]['Team'] .= '<a href="#" onClick="showhide(\'u-'.$line['Nick'].'\',\'b-'.$line['Nick'].'\'); return false;"><div style="font-size:10px;" id="b-'.$line['Nick'].'">» Visa gamla</div></a>';
                        $data[$key]['Team'] .='<div id="u-'.$line['Nick'].'" style="display:none;">';
                        }
                        foreach($array AS $line1){
                            if($status=$this->fetchOne("SELECT name from structure JOIN membership ON structure.gid=membership.gid AND membership.uid='".$line['Nick']."' WHERE parent='".$line1."' AND name LIKE '-%'"))
                                $status = ' ('.substr($status,1,strlen($status)).')'; 
                            else 
                                $status = '';
                            $data[$key]['Team'] .= $this->fetchOne("SELECT CONCAT(IF(event=".$_SESSION['event'].",'</div><b>',''),structure.name,'".$status."(',shortname,')',IF(event=".$_SESSION['event'].",'</b>','')) FROM structure JOIN events ON structure.event=events.id WHERE gid='".$line1."' GROUP BY structure.name").'<br />';

                        }
                        //fixa bort sista br 
                        $data[$key]['Team']= substr($data[$key]['Team'],0,-6);
                        unset($data[$key]['username']);
                        $cnt=$cnt+1;
                    }
                $table->create($data,'','id1');
                echo "Hittade $cnt användare";
                
                }
                else
                    echo "Inga användare hittades";

            }

        }
        else
            $this->display("searchform.tpl");
                echo '<script type="text/javascript">';
                echo 'function showhide(gid,button){
                if (document.getElementById(gid).style.display==\'none\'){
                        new Effect.BlindDown(gid,{duration:0.3});
                        new Element.update(button,\'» Göm gamla\');
                    }
                    else{
                        new Effect.BlindUp(gid,{duration:0.3});
                        new Element.update(button,\'» Visa gamla\');
                    } return false;}';
                echo '</script>';

    }

    function _stats(){
        global $tpl;
        
        if ($crew = db::fetchAllOne("SELECT uid FROM events,structure,membership WHERE active='Y' AND structure.event=events.id AND membership.gid=structure.gid GROUP BY uid"))
            $crew = "uid IN (".implode($crew,',').") ";
        else
            $crew = 'TRUE=FALSE';
        

        $data = array(
            'Crew' => array(
               'Endast antaget crew'=>array() ,
                'Åldersfördelning' => array(
                    '< 18'   => 0,
                    '18-20' => 0,
                    '21-25' => 0, 
                    '26-30' => 0,
                    '> 31'   => 0
                )
            ),
            'Crew Corner' => array(
                'Alla användare' => array(),
                'Åldersfördelning' => array(
                    '< 18'   => 0,
                    '18-20' => 0,
                    '21-25' => 0, 
                    '26-30' => 0,
                    '> 31'   => 0
                )
            ),
            'Forumet' => array()
        );


        $data['Crew']['Könsfördeling']['Killar'] = $this->fetchOne("SELECT count(users.uid) FROM user_profile JOIN users USING(uid) WHERE NOT substring(users.birthdate,9,1) IN (0,2,4,6,8) AND users.$crew").' st';
        $data['Crew']['Könsfördeling']['Tjejer'] = $this->fetchOne("SELECT count(users.uid) FROM user_profile JOIN users USING(uid) WHERE substring(users.birthdate,9,1) IN (0,2,4,6,8) AND users.$crew").' st';
        
        $t = $data['Crew']['Könsfördeling']['Killar'] + $data['Crew']['Könsfördeling']['Tjejer'];
        $data['Crew']['Könsfördeling']['Totalt'] = $t.' st';

        $data['Crew']['Könsfördeling']['Killar'] .=  ' ('.round($data['Crew']['Könsfördeling']['Killar']*100/$t,2).'%)'; 
        $data['Crew']['Könsfördeling']['Tjejer'] .=  ' ('.(100-round($data['Crew']['Könsfördeling']['Killar']*100/$t,2)).'%)';

        $data['Crew Corner']['Könsfördeling']['Killar'] = $this->fetchOne("SELECT count(users.uid) FROM user_profile,users WHERE users.uid=user_profile.uid AND NOT substring(users.birthdate,9,1) IN (0,2,4,6,8)").' st';
        $data['Crew Corner']['Könsfördeling']['Tjejer'] = $this->fetchOne("SELECT count(users.uid) FROM user_profile,users WHERE users.uid=user_profile.uid AND substring(users.birthdate,9,1) IN (0,2,4,6,8)").' st';

        $t = $data['Crew Corner']['Könsfördeling']['Killar'] + $data['Crew Corner']['Könsfördeling']['Tjejer'];
        $data['Crew Corner']['Könsfördeling']['Totalt'] = $t.' st';

        $data['Crew Corner']['Könsfördeling']['Killar'] .=  ' ('.round($data['Crew Corner']['Könsfördeling']['Killar']*100/$t,2).'%)'; 
        $data['Crew Corner']['Könsfördeling']['Tjejer'] .=  ' ('.(100-round($data['Crew Corner']['Könsfördeling']['Killar']*100/$t,2)).'%)';

        $d = $this->fetchAll("SELECT dinner,arrive,depart,birthdate,city,CASE size WHEN '' THEN '-Ej vald' ELSE size END as size,user_eventinfo.valid as eventinfo_valid,user_profile.valid as profile_valid FROM user_profile JOIN users USING(uid) JOIN user_eventinfo USING (uid) WHERE $crew");
        $adr = array();
        $adr1 = 0;
        foreach ($d as $line) {
            $age = age($line['birthdate']);
                if ($age<18) {
                    $data['Crew']['Åldersfördelning']['< 18'] += 1; 
                } elseif ($age>17&&$age<21) {
                    $data['Crew']['Åldersfördelning']['18-20'] += 1;
                } elseif ($age>20&&$age<26) {
                    $data['Crew']['Åldersfördelning']['21-25'] += 1;
                } elseif ($age>25&&$age<31) {
                    $data['Crew']['Åldersfördelning']['26-30'] += 1;
                } elseif ($age>30) {
                    $data['Crew']['Åldersfördelning']['> 31'] += 1;
                } 
            if ($line['city']>'') { 
                
                $k = strtoupper(substr($line['city'],0,1)).strToLower(substr($line['city'],1));
                
                if (!isset($adr[$k])) $adr[$k] = 0;
                $adr[$k] += 1;
            } else {
                $adr1 += 1;
            }
            
            if ($line['eventinfo_valid']=='N') {
                if (!isset($data['Crew']['T-shirt (preliminärt)']))
                    $data['Crew']['T-shirt (preliminärt)'] = array();

                if (!isset($data['Crew']['T-shirt (preliminärt)'][$line['size']]))
                    $data['Crew']['T-shirt (preliminärt)'][$line['size']] = 0;

                $data['Crew']['T-shirt (preliminärt)'][$line['size']] += 1;
                
                if (!isset($data['Crew']['Ankomst (preliminärt)']))
                    $data['Crew']['Ankomst (preliminärt)'] = array();

                if (!isset($data['Crew']['Ankomst (preliminärt)'][$line['size']]))
                    $data['Crew']['Ankomst (preliminärt)'][$line['arrive']] = 0;
                    
                $data['Crew']['Ankomst (preliminärt)'][$line['arrive']] += 1;

                if (!isset($data['Crew']['Hemresa (preliminärt)']))
                    $data['Crew']['Hemresa (preliminärt)'] = array();

                if (!isset($data['Crew']['Hemresa (preliminärt)'][$line['depart']]))
                    $data['Crew']['Hemresa (preliminärt)'][$line['depart']] = 0;
                    
                $data['Crew']['Hemresa (preliminärt)'][$line['depart']] += 1;
            } else {
                if (!isset($data['Crew']['T-shirt']))
                    $data['Crew']['T-shirt'] = array();

                if (!isset($data['Crew']['T-shirt'][$line['size']]))
                $data['Crew']['T-shirt'][$line['size']] = 0;

                $data['Crew']['T-shirt'][$line['size']] += 1;

                if (!isset($data['Crew']['Ankomst']))
                    $data['Crew']['Ankomst'] = array();

                if (!isset($data['Crew']['Ankomst'][$line['arrive']]))
                    $data['Crew']['Ankomst'][$line['arrive']] = 0;
                    
                $data['Crew']['Ankomst'][$line['arrive']] += 1;

                if (!isset($data['Crew']['Hemresa']))
                    $data['Crew']['Hemresa'] = array();

                if (!isset($data['Crew']['Hemresa'][$line['depart']]))
                    $data['Crew']['Hemresa'][$line['depart']] = 0;
                    
                $data['Crew']['Hemresa'][$line['depart']] += 1;

		$line['dinner'] = ($line['dinner']) ? 'Ja' : 'Nej';

		if (!isset($data['Crew']['Slutmiddag']))
                    $data['Crew']['Slutmiddag'] = array();

		if (!isset($data['Crew']['Slutmiddag'][$line['dinner']]))
                    $data['Crew']['Slutmiddag'][$line['dinner']] = 0;
		$data['Crew']['Slutmiddag'][$line['dinner']] += 1;
            }
        }   
        ksort($adr);

        foreach ($data['Crew'] as $key => $line)
            ksort($data['Crew'][$key]);

        $adr['<span style="color:#f69300"><b>Okänt</b><span>'] = $adr1;
        $data['Crew']['Geografi'] = $adr;


        $d = $this->fetchAll("SELECT birthdate,city FROM user_profile JOIN users USING(uid)");
        $adr = array();
        $adr1 = 0;
        foreach ($d as $line) {
            $age = age($line['birthdate']);
                if ($age<18) {
                    $data['Crew Corner']['Åldersfördelning']['< 18'] += 1; 
                } elseif ($age>17&&$age<21) {
                    $data['Crew Corner']['Åldersfördelning']['18-20'] += 1;
                } elseif ($age>20&&$age<26) {
                    $data['Crew Corner']['Åldersfördelning']['21-25'] += 1;
                } elseif ($age>25&&$age<31) {
                    $data['Crew Corner']['Åldersfördelning']['26-30'] += 1;
                } elseif ($age>30) {
                    $data['Crew Corner']['Åldersfördelning']['> 31'] += 1;
                } 
        }

        $tpl->assign('stats',$data);
        $this->display('stats.tpl');

    }
// _contract {{{
    function _contract(){
        global $path,$tpl;
        if ($path->write &&isset($_POST['press'])) {
            $tags = array('quote','br','em','strong','h1','h2','h3','h4','h5','h6','b','i','a','ul','li');
            $attr = array('user','post','href');
            $filter = new InputFilter($tags,$attr);
            $txt = $filter->process($_POST['press']);
            $txt = nl2br($txt);
            write('Contract',$txt);
            //$this->query("UPDATE user_profile SET press='$txt' WHERE uid=".$userid);
            send(E_USER_NOTICE,"Avtal godkännt!");
        }

        echo '<h1>Avtal</h1>';
        echo read('Contract');
        if($path->write){
            $v = new validate($_POST);
            //  $v->error['text'] = "sljtalt";
            $d['press'] = read('G'.$path->vars['team'].'-press'); 
            $tpl->assign('head','Ändra pressentation');
            $tpl->assign('vals',$d);
            $tpl->assign('errors',$v->getAll());
            $this->display('editPress.tpl');
        }

    }
   // }}}

}
?>
