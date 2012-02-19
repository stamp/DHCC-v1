<?php
class user extends module {

    public $noUpload = 1;

    function __construct() 
    {
        $this->setId(get_class($this));
        $this->setName('User manager');
        $this->setDescription('managing users and user accounts with groups');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        parent::module();
    }

    function install () 
    {

        $tables = array (
            'users' => array(
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Default' => '',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'username',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'password',
                    'Type'    => 'varchar(40)'
                ),
                array(
                    'Field'   => 'firstname',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'lastname',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'birthdate',
                    'Type'    => 'varchar(10)'
                ),
                array(
                    'Field'   => 'latestlogin',
                    'Type'    => 'timestamp'
                ),
                array(
                    'Field'   => 'logincount',
                    'Type'    => 'int(11)'
                )
            ),
            'structure' => array(
                array(
                    'Field'   => 'gid',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Default' => '',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'parent',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'safename',
                    'Type'    => 'varchar(100)'
                ),
                array(
                    'Field'   => 'name',
                    'Type'    => 'varchar(100)'
                )
            ),
            'membership' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Default' => '',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'gid',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                )
            )
        );

        $this->installTables($tables);
        
        parent::install();
        
        return true;
    }

    function uninstall () 
    {
        $this->query("
            DROP TABLE users
        ");
        parent::uninstall();
    }
    
    function makeAccessStr($uid) {
        
        $str = "G-1,|U$uid,";

        if ($groups = $this->fetchAllOne("SELECT gid FROM membership WHERE uid=$uid")) {

            foreach ($groups as $line) {
                if ($childs = $this->getGroupChilds($line))
                    $groups = array_merge($groups,$childs);

                if ($parents = $this->getGroupParents($line))
                    $groups = array_merge($groups,$parents);
            }
    
            $groups = array_unique($groups);

            $str .= '|G'.implode($groups,',|G').',';
        }   
        
        // G-3 - member of a group in an active event
        if ($this->fetchOne("SELECT * FROM events,structure,membership WHERE active='Y' AND structure.event=events.id AND membership.gid=structure.gid AND uid=$uid"))
            $str .= '|G-3,';

        if ($flags = $this->fetchAll("SELECT * FROM membership JOIN structure USING (gid) WHERE name LIKE '-%' AND uid=$uid")) {
            foreach ($flags as $flag)
                $str .= '|F'.$flag['name'].''.$flag['parent'].',';
        }

        return $str;
    }
    

    function getGroupChilds($gid) {
        $groups = array();

        if ($groups = $this->fetchAllOne("SELECT gid FROM structure WHERE parent=$gid AND NOT (LEFT(name,1)='-')"))
            foreach ($groups as $line)
                if ($childs = $this->getGroupChilds($line))
                    $groups = array_merge($groups,$childs);

        return $groups;
    } 

    function getGroupParents($gid) {
        $ret = array();
        $parent = $gid;

        while ($parent > 0) {
            if(!$parent = $this->fetchOne("SELECT parent FROM structure WHERE gid=$parent"))
                break;
            $ret[] = $parent;
        }
        
        return $ret;
    }

    function _signin () {
        global $tpl;

        $v = new validate($_POST);

        if (count($_POST)>1) {
            
            $v->length('signin_username',1,100);
            $v->length('signin_password',2,100);
            

            if(!$v->existErrors()) {
                if ($this->signin($_POST['signin_username'],$_POST['signin_password'])) { 
                    header("location: /");
                    exit;
                } else
                    $v->insErr('signin_password','Felaktiga inlogginingsuppgifter');               
            }
        }

        $tpl->assign('vals',$_POST);
        $tpl->assign('errors',$v->getAll());
        $this->display('signin.tpl');

    } 

    function signin ($user,$pass) {
        $u = $this->escapeStr($user);
        $p = sha1($pass);

        if ($u = $this->fetchSingle("SELECT uid,password FROM users WHERE username='$u' AND (password='$p' OR password=old_password('".$this->escapeStr($pass)."'))")) {
            $_SESSION['id'] = $u['uid']; 
            $_SESSION['access'] = $this->makeAccessStr($u['uid']); 

            if ($u['password']!=$p)
                $this->query("UPDATE users SET password='$p' WHERE uid={$u['uid']}");

            if($this->tableExists('user_profile')) 
                $_SESSION['validprofile'] = $this->fetchOne("SELECT valid FROM user_profile WHERE uid=".$u['uid']);

            if($this->tableExists('user_eventinfo'))
                $_SESSION['valideventinfo'] = $this->fetchOne("SELECT valid FROM user_eventinfo WHERE uid=".$u['uid']);

    	    if (is_file('modules/extendeduser/module.inc')&&$ext = core::load('extendeduser')) {
                $_SESSION['validcontract'] = $this->fetchOne("SELECT validcontract FROM user_profile WHERE uid=".$u['uid']);
            }

            $this->query("UPDATE users SET latestlogin=NOW(),logincount=logincount+1 WHERE uid={$_SESSION['id']} LIMIT 1");
            logg('Signed in');
            return true;
        } else
            logg('FAILED: Signing in');
            return false;
    }

    function _signout() {
        logg('Signed out');
        $_SESSION = array();
        session_destroy();

        header("location: /");
        exit;
    }

    function _showProfile() {
    	global $path,$tpl;
    	
    	if (!isset($path->vars['uid'])||isset($path->vars['uid'])&&!is_numeric($path->vars['uid'])) {
    		echo 'User id input is invalid!';
    		return false;
    	}
    	
    	if (is_file('modules/extendeduser/module.inc')&&$ext = core::load('extendeduser')) {
    		$ext->_showProfile();
    	} else {
    		$tpl->assign('profile',$this->fetchSingle("SELECT * FROM users WHERE uid={$path->vars['uid']}"));
    		$tpl->assign('short',$this->short($path->vars['uid']));
    		if ($p = $this->getPicture($path->vars['uid']))
    			$tpl->assign('picture',$p);
    		$this->display('show.tpl');
    	}
    }

    function _registerNew () {
        global $tpl;

        $v = new validate($_POST);

        if ($_POST->is_set('action')&&$_POST['action']=='register') {
            $v->length('username',2,100,true);
            $v->length('firstname',2,100,true);
            $v->length('lastname',2,100,true);
            $v->swepnr('birthdate');
            $v->length('password',2,100);
            $v->match('password2','password');
            
            $birthdate = str_replace('-','',$_POST->sql('birthdate'));
            if($this->fetchAll("SELECT * FROM users WHERE birthdate='{$birthdate}'"))
                $v->insErr('birthdate','Du har redan ett konto registrerat!');
            
            $username = $_POST->nohtml('username');
            $locked = array('root','mail','deamon','bin','sys','sync','games','man','news','uucp','proxy','postgres','www-data','backup','nobody');
            if(in_array($username,$locked))
                $v->insErr('username','Användarnamnet är låst!');

            if($this->fetchAll("SELECT * FROM users WHERE username='{$username}'"))
                $v->insErr('username','Användarnamnet är upptaget!');
            
            if ($this->age($birthdate)<18)
                $v->insErr('birthdate','Tyvärr är det 18 gräns!');

            if(!$v->existErrors()) {
                $res = $this->insert(array(
                        'username'  => $_POST->nohtml('username'),
                        'password'  => sha1($_POST->nohtml('password')),
                        'firstname' => $_POST->nohtml('firstname'),
                        'lastname'  => $_POST->nohtml('lastname'),
                        'birthdate' => str_replace('-','',$_POST->sql('birthdate'))
                    ),
                    'users'
                );
                logg("Registerd user '".mysql_insert_id()."'");
            }
        }
        if (isset($res)&&$res) {
            $tpl->assign('registrationsuccess',true);
        } else {
            $tpl->assign('errors',$v->getAll());
            $tpl->assign('vals',$v->str);
        }
        $this->display('register.tpl');
    }

    function _editProfile() {
    

    }

    function _editPress() {
        global $tpl,$path;
        if($path->write) {
            $userid = $path->vars['uid'];
        } else{
            $userid = $_SESSION['id'];
        }

        if (isset($_POST['text'])) {
            $tags = array('quote','br','em','strong','h1','h2','h3','h4','h5','h6','b','i','a','ul','li');
            $attr = array('user','post','href','target');
            $filter = new InputFilter($tags,$attr);
            $txt = $this->escapeStr($filter->process($_POST['text']));
            $this->query("UPDATE user_profile SET press='$txt' WHERE uid=".$userid);
            send(E_USER_NOTICE,"Presentation uppdaterad!");
            logg('Updated presentation');
        }

        $v = new validate($_POST);
        //  $v->error['text'] = "sljtalt";
        $d = $this->fetchSingle("SELECT press AS text FROM user_profile WHERE uid=".$userid);
        if(isset($d['press']))
        $tpl->assign('text',$d['press']);
        $tpl->assign('head','Ändra pressentation');
        $tpl->assign('vals',$d);
        $tpl->assign('errors',$v->getAll());
        $this->display('editPress.tpl');
   
    }

    function _showEventLog() {
        global $tpl,$path;
        
        if($path->write) {
            $userid = $path->vars['uid'];
        } else{
            $userid = $_SESSION['id'];
        }
            
        

        $logg = db::fetchAll("SELECT timestamp,text FROM logg WHERE uid=".$userid." ORDER BY id DESC");

        $tpl->assign('logg',$logg);
        $this->display('logg.tpl');
    }

    function getUserData($uid,array $data) {
        
        if(!is_numeric($uid))
            return false;

        $valid = array(
            'username','firstname','lastname','birthdate'
        );
        $fields = array();

        foreach ($data as $line) {
            if (in_array($line,$valid))
                $fields[] = $line;
        }

        if (count($fields)==0)
            return false;

        return $this->fetchSingle("SELECT `".implode($fields,'`,`')."` FROM users WHERE uid=$uid");
    }

    function age($date) {
        $date = substr($date,0,6);

        if (substr($date,0,2)<date('y'))
            $date = '20'.$date;
        else
            $date = '19'.$date;

        $year_diff  = date("Y") - substr($date,0,4);
        $month_diff = date("m") - substr($date,4,2);
        $day_diff   = date("d") - substr($date,6,2);
        
        
        if (($day_diff < 0 && $month_diff <= 0) || $month_diff < 0)
          $year_diff--;
        return $year_diff;

    }

    function short($uid) {
        if (!is_numeric($uid))
            return false;

        $ar = $this->fetchSingle("SELECT username,birthdate FROM users WHERE uid=$uid");
           

        $sex  = substr($ar['birthdate'],8,1);
        
        $ret = "<a href=\"/users/".path::encode($ar['username']).".htm\">{$ar['username']}</a>";

        if (in_array($sex, array(0,2,4,6,8)))
            $ret .= ' F';
        else
            $ret .= ' P';

        $ret .= user::age($ar['birthdate']);
        return $ret;
    }
    
	function getPicture($uid) {
		
	}


    function _editPicture(){
        global $tpl,$path;
        if($path->write) {
        $userid = $path->vars['uid'];
        }
        else{
        $userid = $_SESSION['id'];
        }

    if (isset($_FILES['filen']['name'])) {
        $uploaddir = getcwd().'/images/users/upload/';
        $uploadfilename = basename($_FILES['filen']['name']);
        $uploadfilename = str_replace('å','a',$uploadfilename);
        $uploadfilename = str_replace('ä','a',$uploadfilename);
        $uploadfilename = str_replace('ö','o',$uploadfilename);
        $uploadfile = $uploaddir . $uploadfilename;
    
        $nm = explode('.',basename($_FILES['filen']['name']));
        $type = strtolower($nm[count($nm)-1]);
    
        if ($type == 'jpg') $type = 'jpeg';

        $filename = md5(rand());
        $uploadfilename = $filename.'.'.$type;
        $uploadfile = $uploaddir . $uploadfilename;
    
        while(is_file($uploadfile)) {
            $filename = md5(rand());
            $uploadfilename = $filename.'.'.$type;
            $uploadfile = $uploaddir . $uploadfilename;
        }

        $uploadfilename = $filename.'.'.$type;
        $uploadfile = $uploaddir . $uploadfilename;
        if ($_FILES['filen']['error']==0) {
            if ($type == 'jpeg' || $type == 'png' || $type == 'gif') {
                if (move_uploaded_file($_FILES['filen']['tmp_name'], $uploadfile)) {
                    eval('$img = imagecreatefrom'.$type.'($uploadfile);');
                    if ($img) {
                        $x = imagesx($img);
                        $y = imagesy($img);
                        $xmax = 400;
                        $y = intval($y * ($xmax/$x));
                        $x = $xmax;
               
                        $thumb = imagecreatetruecolor($x,$y);
                        imagecopyresampled($thumb,$img,0,0,0,0,$x,$y,imagesx($img),imagesy($img));
                        imagejpeg($thumb,getcwd().'/images/users/thumbs/'.$filename.'.jpg',80);
                        if (imagesx($img)<$xmax) {
                            imagejpeg($thumb,getcwd().'/images/users/src/'.$filename.'.jpg',80);
                        } else {
                            imagejpeg($img,getcwd().'/images/users/src/'.$filename.'.jpg',80);
                        }
                    
                        $tpl->assign('img','/images/users/thumbs/'.$filename.'.jpg');
                        $tpl->assign('src',$filename);
                        $tpl->assign('target',(isset($_POST['target'])&&$_POST['target']=='picture2') ? 'picture2' : 'picture');
                        $this->display("profile.picture.rezise.tpl.php");
                    } else {
                        $tpl->assign('picture',$this->fetchSingle("SELECT picture,picture2,picture_status,picture2_status FROM users WHERE uid=".$userid));
                        $tpl->assign('fel',2);
                        $this->display("profile.picture.show.tpl.php");
                        }
                } else {
                    $tpl->assign('picture',$this->fetchSingle("SELECT picture,picture2,picture_status,picture2_status FROM users WHERE uid=".$userid));
                    $tpl->assign('fel',1);
                    $tpl->assign('error',$_FILES['filen']['error']);
                    $this->display("profile.picture.show.tpl.php");
                }
            } else {
                $tpl->assign('picture',$this->fetchSingle("SELECT picture,picture2,picture_status,picture2_status FROM users WHERE uid=".$userid));
                $tpl->assign('fel',0);
                $this->display("profile.picture.show.tpl.php");
            }
        } else {
            $tpl->assign('picture',$this->fetchSingle("SELECT picture,picture2,picture_status,picture2_status FROM users WHERE uid=".$userid));
            $tpl->assign('fel',1);
            $tpl->assign('error',$_FILES['filen']['error']);
            $this->display("profile.picture.show.tpl.php");
        }
    } else {

        if (isset($_POST['coordX'])) {
    
            $src = imagecreatefromjpeg(getcwd().'/images/users/src/'.$_POST['src'].'.jpg');
            $img = imagecreatefromjpeg(getcwd().$_POST['img']);
        
            $thumb = imagecreatetruecolor(120,160);
            $small = imagecreatetruecolor(60,80);
            $micro = imagecreatetruecolor(30,40);

            $cX = ($_POST['coordX']>0) ? intval(($_POST['coordX']/imagesx($img))*imagesx($src)) : 0;
            $cY = ($_POST['coordY']>0) ? intval(($_POST['coordY']/imagesy($img))*imagesy($src)) : 0;
            $cW = ($_POST['coordW']>0) ? intval(($_POST['coordW']/imagesx($img))*imagesx($src)) : 0;
            $cH = ($_POST['coordH']>0) ? intval(($_POST['coordH']/imagesy($img))*imagesy($src)) : 0;
        
        
            $new = imagecreatetruecolor($cW,$cH);
        
            imagecopy($new,$src,0,0,$cX,$cY,$cW,$cH);

            imagecopyresampled($thumb, $new,0,0,0,0,120,160,imagesx($new),imagesy($new));
            imagecopyresampled($small, $new,0,0,0,0,60,80,imagesx($new),imagesy($new));
            imagecopyresampled($micro, $new,0,0,0,0,30,40,imagesx($new),imagesy($new));
        
            imagejpeg($thumb,getcwd().'/images/users/thumbs/thumb_'.$_POST['src'].'.jpg',80);
            imagejpeg($small,getcwd().'/images/users/thumbs/small_'.$_POST['src'].'.jpg',80);
            imagejpeg($micro,getcwd().'/images/users/thumbs/micro_'.$_POST['src'].'.jpg',80);
        
            $t = (isset($_POST['target'])&&$_POST['target']=='picture2') ? 'picture2' : 'picture';
        
            $this->query("UPDATE users SET $t='".$_POST->nohtml('src')."',{$t}_status=1 WHERE uid=".$userid);
            logg('Uploaded a new picure to '.$_POST->sql('target'));
        
            $tpl->assign('thumsmade',1);
    
        } elseif (isset($_GET['rm'])) {
            $this->query("UPDATE users SET picture='',picture_status=0 WHERE uid=".$userid);
        } elseif (isset($_GET['rm2'])) {
            $this->query("UPDATE users SET picture2='',picture2_status=0 WHERE uid=".$userid);
        }


        $tpl->assign('picture',$this->fetchSingle("SELECT picture,picture2,picture_status,picture2_status FROM users WHERE uid=".$userid));
        $this->display("profile.picture.show.tpl.php");

    }
    }
    function _showOnline(){
        global $tpl;
        $this->display("online.head.tpl");
        $data = $this->fetchAll("SELECT 
                users.uid AS 'Nick', users.username ,
                CONCAT(users.firstname,' ',users.lastname) AS Namn,latestlogin AS 'Senast inloggad',
                GROUP_CONCAT(DISTINCT(structure.gid) ORDER BY events.start) as 'Team'
			FROM sessions 
            JOIN users 
                USING(uid)
			LEFT JOIN membership 
                USING(uid) 
            LEFT JOIN structure
                ON structure.gid = membership.gid  
            LEFT JOIN events
                ON structure.event = events.id
			GROUP BY sessions.uid");
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
        }
        $table = core::load('tables');
        $table->create($data,array('width'=>'100%'),'on'
		);
        $this->display("online.foot.tpl");
    }
    function _changePasswd() {
        global $tpl,$path;
        if($path->write) {
            $userid = $path->vars['uid'];
        } else{
            $userid = $_SESSION['id'];
        }

        $v = new validate($_POST);

        if (isset($_POST)&&count($_POST)>1) {
            //Check old password
            if ($userid == $_SESSION['id']) {
                if($sql = $this->fetchSingle("SELECT users.password,users.uid as id FROM users WHERE uid= {$userid}")) {
                            
                    $p = sha1($_POST['old']);
                    $p2 = crypt($this->escapeStr($_POST['old']),substr($sql['password'],0,2));
                    
                    if (!($sql['password']==$p||$sql['password']==$p2)) {
                        $v->insErr('old','Det gamla lösenordet är fel');
                    }
                }
            }
            //Check new passwords
            $v->length('new',5,100);
            if ($_POST['new']!=$_POST['new2']) $v->insErr('new2','De nya lösenorden matchar inte!');

            $valid = array('new','new2');
            $vals = array();
            foreach ($valid as $va) {
                $vals[$va] = $_POST[$va];
            }   

            if(!$v->ExistErrors()) {
                $pass = sha1($_POST['new']);
                $this->query("UPDATE users SET password = '$pass' WHERE uid={$userid}");
                logg('Changed password');
            send(E_USER_NOTICE,"Ditt lösenord är nu ändrat!");
            }
        }

        
        $tpl->assign('vals',$_POST);
        $tpl->assign('user',$userid);
        $tpl->assign('errors',$v->getAll());

        $this->display("changepass.tpl");
    }
    function _changeNick(){
        global $tpl,$path;
        if($path->write) {
        $userid = $path->vars['uid'];
        }
        else{
        $userid = $_SESSION['id'];
        }

        $v = new validate($_POST);

        if (isset($_POST)&&count($_POST)>1) {
            //Check new passwords
            $v->length('username',3,20);

            if($this->fetchSingle("SELECT username FROM users WHERE username='".$this->escapeStr($_POST['username'])."'"))
                $v->error['username'] = "Användarnamnet finns redan";

            if(!$v->ExistErrors()) {
                $username = $this->escapeStr($_POST['username']);
                $this->query("UPDATE users SET username = '$username' WHERE uid={$userid}");
                logg('Changed username');
                send(E_USER_NOTICE,"Ditt användarnamn är nu ändrat!");
            };
        }

    
        $tpl->assign('vals',$this->fetchSingle("SELECT username FROM users WHERE uid='".$userid."'"));
        $tpl->assign('errors',$v->getAll());

        $this->display("changenick.tpl");

    }

    function picture($src) {
        if (strlen($src)==32) 
            $file = 'images/users/thumbs/thumb_'.$src.'.jpg';
        else
            $file = 'images/users/old/'.$src;
        
        if (file_exists($file)&&rtrim($file,'/')==$file)
            return '/' . $file;

        return false;
    }
}
?>
