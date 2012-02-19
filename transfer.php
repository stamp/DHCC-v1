<?php

include('config.inc');
 
$temp = db::fetchAll("SELECT DISTINCT(uid),name,size,gsize FROM user_eventinfo LEFT JOIN membership
        USING(uid) LEFT JOIN structure USING(gid) WHERE size NOT LIKE '' and event= 14");

        core::printarr($temp);
/*
$old = $db->fetchAll("SELECT * from cco_old.User_betyg");
foreach ( $old as $key => $line ) {	
	if ( !$event = db::fetchOne("SELECT id FROM events WHERE shortname='".$line['DH']."'") )
		continue;
	if ( !$crew = db::fetchOne("SELECT gid FROM structure WHERE name='".$line['Team']."' AND event='".$event."'") )
		continue;

	$betyg = $line['Betyg'] * 20;

	$data = array(
		'uid' => $line['UserID'],
		'event' => $event,
		'writer' => $line['Av'],
		'pre' => $betyg,
		'during' => $betyg,
		'after' => $betyg,
		'note' => $line['Fritext'],
		'team' => $crew
	);
	db::insert($data,'betyg');
}*/
/*
$old = $db->fetchAll("SELECT * from cco_old.User_data");
foreach ( $old as $key => $line ) {
        $db->query(
            "UPDATE users SET
                birthdate           = '".str_replace('-','',$line['Personnr'])."'
            WHERE uid = {$line['UserID']}"
        );
}
*/
/*
$old = $db->fetchAll("SELECT * from cco_old.User_gb");
$new = array();
foreach ( $old as $key => $line ) {
    $new[$key]['timestamp'] = date('Y-m-d H:i:s',$line['From_time']);
    $new[$key]['gbid'] = $line['To_user'];
    $new[$key]['from'] = $line['From_user'];
    $new[$key]['text'] = $line['Content'];
    $new[$key]['new'] = 'read';
}
$db->installTablesContent(array('guestbook'=>$new));
*/
/*
$old = $db->fetchAll("SELECT * from cco_old.Info");
$new = array();
foreach ( $old as $key => $line ) {
    if (!$u = $db->fetchOne("SELECT uid FROM users WHERE username='{$line['Av']}'")) {
        $db->query("INSERT INTO users SET username='{$line['Av']}'");
        $event = mysql_insert_id();
        echo "Added user {$line['Av']}<br>";
    }
    $types = array('CrewCorner'=>2,'Utsidan'=>1,'Admin'=>3,'Crewinfo'=>4);
    $access = array(1=>'G0,G-1,',2=>'G-1,',3=>'G10,',4=>'G10,');
    $new[$key]['timestamp'] = $line['Datum'].' '.$line['Klockan'];
    $new[$key]['head'] = $line['Rubrik'];
    $new[$key]['text'] = $line['Info'];
    $new[$key]['list'] = $types[$line['Typ']];
    $new[$key]['writer'] = $u;
    $new[$key]['access'] = $access[$line['Till']];
}
$db->installTablesContent(array('news'=>$new));
*/
// {{{ User_Skills
/*
$old = $db->fetchAll("SELECT * from cco_old.User_Skills");
foreach ( $old as $key => $line ) {
    $user = $line['UserID'];

    if (!$event = $db->fetchOne("SELECT id FROM events WHERE shortname='{$line['LAN']}'")) {
        $db->query("INSERT INTO events SET shortname='{$line['LAN']}',name='{$line['LAN']}',active='N'");
        $event = mysql_insert_id();
        echo "Added event {$line['LAN']}<br>";
    }

    if (!$team = $db->fetchOne("SELECT gid FROM structure WHERE name='{$line['Team']}' AND event='$event' AND is_team='Y'")) {
        $db->query("INSERT INTO structure SET name='{$line['Team']}',safename='".path::encode($line['Team'])."',parent='10',is_team='Y',event='$event'");
        $team = mysql_insert_id();
        echo "Added team {$line['Team']}<br>";
    }
    if(!$db->fetchALl("SELECT * FROM membership WHERE gid=$team AND uid=$user")) {
        $db->query("INSERT INTO membership SET gid=$team, uid=$user");
        echo "Added membership  {$line['LAN']} - {$line['Team']} - $user<br>";
    }

}
*/
// }}}
// {{{ User_data extended
/*
$old = $db->fetchAll("SELECT * from cco_old.User_data");
foreach ( $old as $key => $line ) {
    $new = array();
    if ($db->fetchOne("SELECT uid FROM user_profile WHERE uid = {$line['UserID']}"))
        $db->query(
            "UPDATE user_profile SET
                email           = '{$line['Epost']}',
                primaryphone    = '{$line['Mobil']}',
                primaryphontype = 'Mobil',
                secondaryphone    = '{$line['Telefon']}',
                secondaryphonetype = 'Hem',
                street          = '{$line['Adress']}',
                city            = '{$line['Ort']}',
                postcode        = '{$line['Postnr']}',
                medical         = 'Medicin: {$line['Medicin']}, Sjukdom: {$line['Sjukdom']}',
                valid           = 'Y',
                country         = 170
            WHERE uid = {$line['UserID']}"
        );
    else
        $db->query(
            "INSERT INTO user_profile SET
                email           = '{$line['Epost']}',
                primaryphone    = '{$line['Mobil']}',
                primaryphontype = 'Mobil',
                secondaryphone    = '{$line['Telefon']}',
                secondaryphonetype = 'Hem',
                street          = '{$line['Adress']}',
                city            = '{$line['Ort']}',
                postcode        = '{$line['Postnr']}',
                medical         = 'Medicin: {$line['Medicin']}, Sjukdom: {$line['Sjukdom']}',
                valid           = 'Y',
                country         = 170,
                uid = {$line['UserID']}"
        );       

}
*/
//$db->installTablesContent(array('user_profile'=>$new));

// }}}
// {{{ Team_data
/*
$old = $db->fetchAll("SELECT * from cco_old.Team_data WHERE Ok LIKE 'Ok'");
$new = array();

foreach ($old as $line) {
    if(!$id = $db->fetchOne("SELECT gid FROM structure WHERE is_team='Y' AND name LIKE '{$line['Team']}' AND event=11")) {
        //die("SELECT gid FROM structure WHERE is_team='Y' AND name LIKE '{$line['Team']}' AND event=11");
            $db->query("INSERT INTO structure SET is_team='Y', name='{$line['Team']}',event=11,parent=10");
            $id = mysql_insert_id();
    }

    if(!$db->fetchOne("SELECT id FROM membership WHERE uid={$line['UserID']} AND gid=$id")) {
        $db->query("INSERT INTO membership SET uid={$line['UserID']}, gid=$id");
    }
        
}
*/
// }}}
// {{{ Forum postzos
/*
//$old = $db->fetchAll("SELECT * from cco_old.Forum_posts");
$ant = $db->fetchOne("SELECT count(*) from cco_old.Forum_posts");
$query = 'SELECT * from cco_old.Forum_posts' ;
$result = mysql_query($query) or die('Query failed: ' . mysql_error());
$i =0;$j= 0;$prev =0;$prev2=0;$start = time();
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    $i++;$j++;
    $new = array();
    $new['post'] = $line['ID'];
    $new['topic'] = $line['Topic'];
    //$new['text'] = str_replace('<td bgcolor=#FFFFFF style="border: 1 solid #000000">','<td bgcolor="#eeeeee" style="padding:5px;border: 1 solid #000000;color:#000;">',$line['Post']);
    $new['text'] = preg_replace_callback(
        '$\<table(.*)\>\<tr\>\<td\>QUOTE\s\((.+)\s\@\s(.+)\)\</td\>\</tr\>\<tr\>\<td(.*)\>(.+)\</td\>\</tr\>\</table\>$',
        'quote',
        $line['Post']
    );

    $new['timestamp'] = date('Y-m-d H:i:s',$line['Tidpunkt']);
    $new['uid'] = $line['Referens'];
    if(!$db->fetchOne("SELECT id FROM posts WHERE post=".$line['ID'])) {
        $db->insert($new,'posts')+1;
    } else {
        $db->query("UPDATE posts SET text='".$db->escapeStr($new['text'])."' WHERE post=".$line['ID']);
    }

    if ($prev < time()) { 
        $dif = time() - $prev;
        $speed = ($j - $prev2) / $dif;
        echo date('H:i:s - ').$speed.'p/s - '.((($ant-$j)/$speed)).'s kvar - '.(round($j/$ant,5)*100)."%<br><script>window.scrollTo(0,400000);</script>";
        ob_flush();
        flush();
        $i = 0;
        $prev2 = $j;
        $prev = time();
    }
}
       $dif = time() - $prev;
        $speed = ($j - $prev2) / $dif;
        echo date('H:i:s - ').$speed.'p/s - 0s kvar - '.(round($j/$ant,5)*100)."%<br><script>window.scrollTo(0,400000);</script>";
        ob_flush();
        flush();
        $i = 0;
        $prev2 = $j;
        $prev = time();

function quote($data) {
    return "<quote user='{$data[2]}' time='".str_replace('kl ','',$data[3])."'>{$data[5]}</quote>";
}

*/
//$db->installTablesContent(array('posts'=>$new));
// }}}
// {{{ Topics extra
/*
$old = $db->fetchAll("SELECT * from cco_old.Forum_topics_extra");
$new = array();

foreach ( $old as $key => $line ) {
    if(!$forum = $db->fetchOne("SELECT Namn from cco_old.Forum WHERE ID={$line['Forum']}"))  
        $forum = 'Gamla trådar';
    if (!$forum = $db->fetchOne("SELECT id from forums WHERE head='$forum'"))
        $forum = 39;
    $db->query("UPDATE topics SET forum=$forum WHERE id={$line['ID']}");
}
*/
// }}}
/// {{{ Updatera räknare
/*
$forum = $db->fetchAll("SELECT * from topics");

foreach ($forum as $line) {
    if ($a = $db->fetchOne("SELECT count(*) from posts WHERE topic='{$line['id']}'"))
        $db->query("UPDATE topics SET  posts={$a} WHERE id={$line['id']}");
}

$forum = $db->fetchAll("SELECT * from forums");

foreach ($forum as $line) {
    if ($a = $db->fetchSingle("SELECT count(*) as topics,sum(posts) as posts from topics WHERE forum='{$line['id']}'"))
        $db->query("UPDATE forums SET topics={$a['topics']}, posts='{$a['posts']}' WHERE id={$line['id']}");
}
*/
//}}}
// {{{ Topics tabellen
/*
$old = $db->fetchAll("SELECT * from cco_old.Forum_topics");
$new = array();

function r2($d) {
    $d = explode(',',$d);
    $n = array();

    foreach ($d as $l) {
        if (is_numeric($l)) $n[] = $l;
    }
    return implode(array_unique($n),',').',';
}


foreach ( $old as $key => $line ) {
    $new[$key]['id'] = $line['ID'];
    $new[$key]['head'] = $line['Topic'];
    $new[$key]['created'] = date('Y-m-d h:i:s',$line['Start_date']);
    $new[$key]['owner'] = $line['Start_id'];
    $new[$key]['last_poster'] = $line['Last_id'];
    $new[$key]['last_timestamp'] = date('Y-m-d h:i:s',$line['Last_date']);
    $new[$key]['sticky'] = ($line['Klistrad']) ? 'Y' : 'N';
    $new[$key]['teamlock'] = ($line['Teamlock']) ? 'Y' : 'N';
    $new[$key]['lock'] = ($line['Locked']) ? 'Y' : 'N';
    $new[$key]['new'] = r2($line['Read_by']);
}


$db->installTablesContent(array('topics'=>$new));

// }}}
*/
// {{{ User_data
/*
$old = $db->fetchAll("SELECT * from cco_old.User_data");
$new = array();
foreach ( $old as $key => $line ) {
    $new[$key]['uid'] = $line['UserID'];
    $new[$key]['username'] = $line['Nick'];
    $new[$key]['firstname'] = $line['Fnamn'];
    $new[$key]['lastname'] = $line['Enamn'];
    $new[$key]['password'] = $line['Pass2'];
    $new[$key]['birthdate'] = $line['Personnr'];
    $new[$key]['latestlogin'] = $line['Senast'];
    $new[$key]['logincount'] = $line['Antal'];
    $new[$key]['Picture'] = $line['Bild'];
    $new[$key]['Picture2'] = $line['Bild2'];
}

$db->installTablesContent(array('users'=>$new));
*/
// }}}
// {{{ Forum
/*
function access($val) {
    global $db;
    if (trim($val)=='')
        return false;

    if ($val == 'Alla')
        return 'G0';
    
    if (is_string(substr($val,0,1))&&is_numeric(substr($val,1))) {
        switch(substr($val,0,1)) {
            case 'L':
                if ($val=='L2')
                    return 'G10';
                if ($val=='L4')
                    return 'G53';
                die("Okännd $val");
            case 'M':
                return 'U'.substr($val,1);
            case 'U':
                return 'U'.substr($val,1);
            default:
                die("Okänd $val");
        }
    } else {
        if($id = $db->fetchOne("SELECT gid FROM structure WHERE is_team='Y' AND name='$val'"))
            return 'G'.$id;
        
        $db->query("INSERT INTO structure SET is_team='Y', name='$val'");
            return 'G'.mysql_insert_id();
    }
}

function r($da) {
    $read=explode(',',$da);
    $newa = array();
    foreach($read as $line) {
        if ($d = access($line)) $newa[] = $d;
    }

    return implode(array_unique($newa),',').',';
}

$old = $db->fetchAll("SELECT * FROM cco_old.Forum");
$new = array();
foreach ( $old as $key => $line ) {
    $new[$key]['group'] = ($line['ID'] > 0) ? 'Team specifika forum' : (($line['Beskrivning']=='')?'Gamla team forum':'Gemensamma forum');
    $new[$key]['head'] = $line['Namn'];
    $new[$key]['timestamp'] = date('Y-m-d h:i:s',$line['Senast']);
    $new[$key]['desc'] = $line['Beskrivning'];
    
    $new[$key]['read'] = r($line['Lasa']);
    $new[$key]['write'] = r($line['Skriva'].(($new[$key]['group'] == 'Team specifika forum')?','.$line['Namn']:''));
    

}

core::printArr($new);
core::printArr($old);

$db->installTablesContent(array('forums'=>$new));
*/
// }}}

get();
?>
