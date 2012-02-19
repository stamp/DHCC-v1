<?php

class ssv extends module {
    
    function __construct() {
        $this->setId(get_class($this));
        $this->setName('SSV');
        $this->setDescription('A time keeping system for the ssv team');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setDepends('path safety');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }

    function install() {
        $tables = array (
            'ssv_floor' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'x',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'y',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'type',
                    'Type'    => "enum('floor','wall','bed')",
                    'Default' => 'floor'
                )
            ),
            'ssv_booking' => array(
                array(
                    'Field'   => 'id',
                    'Type'    => 'int(11)',
                    'Key'     => 'PRI',
                    'Extra'   => 'auto_increment'
                ),
                array(
                    'Field'   => 'x',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'y',
                    'Type'    => 'int(11)'
                ),
                array(
                    'Field'   => 'uid',
                    'Type'    => 'int(11)'
                ),
		array(
		    'Field'   => 'note',
		    'Type'    => 'varchar(255)'
		)
            ),
	    'ssv_order' => array(
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
                    'Field'   => 'time',
                    'Type'    => 'datetime'
                ),
                array(
                    'Field'   => 'note',
                    'Type'    => 'varchar(255)'
                ),
		array(
		    'Field'   => 'status',
		    'Type'    => 'int(11)'
		)
            )
        );

        $this->installTables($tables);
        
        return parent::install();
    }

    function getChilds($parent) {
        if($childs = db::fetchAllOne("SELECT gid FROM structure WHERE parent=$parent")) {
            foreach ($childs as $line)
                if ($childs2 = self::getChilds($line))
                    $childs = array_merge($childs,$childs2);

            return $childs;
        } else return false;
    }

    function _run() {
        global $tpl;
        if (isset($_GET['plats'])) {
            list($x,$y) = explode('-',$_GET['plats']);
            $tpl->x = $x;
            $tpl->y = $y;
        }
		if (!isset($_GET['action'])) {

            $members = array();
            $teams = db::fetchAll("SELECT structure.* FROM structure JOIN events ON events.id = structure.event AND events.active='Y' WHERE is_team='Y' ORDER BY structure.name");
            foreach ($teams as $line) {
                $groups = array($line['gid']);
                if ( $childs = self::getChilds($line['gid']) )
                    $groups = array_merge($childs,$groups);

                $members[$line['gid']] = db::fetchAll("SELECT *,CASE ISNULL(ssv_booking.id) WHEN true THEN username ELSE concat('- ',username) END as username FROM membership JOIN users USING (uid) LEFT JOIN ssv_booking USING (uid) WHERE gid IN (".implode($groups,',').") GROUP BY uid ORDER BY username");
            }
            $tpl->assign('teams',$teams);
            $tpl->assign('box',$members);

    		$this->display('main.tpl.php');
		} else 
			switch($_GET['action']) {
            case 'wake':
               if ($_POST['final'] == 'true')
                  db::query('UPDATE ssv_order SET status = concat(\'-\',status) WHERE status>=0 AND id='.$_POST->sql('wake'));
               else
                  db::query('UPDATE ssv_order SET status = status + 1 WHERE id='.$_POST->sql('wake'));
				case 'list':
					global $tpl;
					
					$data = db::fetchAll("
						SELECT 
							x,
							y,
							type,
							ssv_booking.uid as uid,
							status,
                            ssv_booking.uid,
							MIN(time) as time
						FROM 
							ssv_floor 
						LEFT JOIN 
							ssv_booking USING (x,y) 
						LEFT JOIN 
							ssv_order ON ssv_order.uid=ssv_booking.uid AND status>=0 	
						GROUP BY 
							x,y
						ORDER BY time DESC");

               $pre = strtotime(read('ssv_pre_alarm_time','+20 minutes','strtotime'));
					foreach ($data as $key => $line) {
						if ($line['type']=='wall')
							$data[$key]['color'] = 'cwall';
						else {
							if ($line['time']>'') {
								if ($line['time'] < date('Y-m-d H:i:s'))
									$data[$key]['color'] = 'calarm';
								elseif ($line['time'] < date('Y-m-d H:i:s',$pre))
                           $data[$key]['color'] = 'cwarn';
								else
									$data[$key]['color'] = 'csleep';
							} else 
								if ($line['uid'] == '')
									$data[$key]['color'] = 'cfree';
								else
									$data[$key]['color'] = 'cbed';
						}
					}
					$tpl->assign('ssvdata',$data);
					$this->display('javalist.tpl');

               $data = db::fetchAll("
						SELECT 
                     *,ssv_order.id as id,ssv_order.note as note
						FROM 
							ssv_order 
                  JOIN 
                     ssv_booking USING (uid)
                  JOIN
                     users USING (uid)
                  WHERE
                     status >= 0
						ORDER BY time");

               $tpl->ssvdata = $data;
               $this->display('list.tpl');

					break;

				case 'simple':
               list($x,$y) = explode('-',$_GET['plats']);
              
               if ($user = db::fetchSingle("SELECT * FROM ssv_booking JOIN users USING (uid) JOIN user_eventinfo USING (uid) WHERE x={$x} AND y={$y}")) {
              
                  echo '<img src="'.self::picture($user['picture']).'" style="float:left;margin-bottom:10px;">';
                  echo '<div style="float:left; padding-left:20px;">';
                     echo '<span style="font-size:16px;"><b>'.user::short($user['uid']).'</b></span><br>';
                     echo '<b>Var:</b> '.$user['where'].'<br>';
                     echo '<b>På:</b> '.$user['on_what'].'<br>';
                     echo '<b>Med:</b> '.$user['with_who'].'<br>';
                     echo ($user['sleep_hard']?'Sover hårt<br>':'');
                     echo ($user['snore']=='Ja'?'<b>SNARKAR!</b><br>':'');
                  echo '</div>';
               }
					break;
				case 'book':
                    if (db::fetchOne("SELECT * FROM ssv_booking WHERE  x=".$x." AND y=".$y." AND uid=".$_GET['uid'].""))
                        db::query("DELETE FROM ssv_booking WHERE  x=".$x." AND y=".$y." AND uid=".$_GET['uid']."");
                    else
                        db::query("INSERT INTO ssv_booking SET x=".$x.",y=".$y.",uid=".$_GET['uid']."");
			    case 'advanced':
               list($x,$y) = explode('-',$_GET['plats']);
               if ($user = db::fetchSingle("SELECT * FROM ssv_booking JOIN users USING (uid) JOIN user_eventinfo USING (uid) WHERE x={$x} AND y={$y}")) {
                  
                  if (isset($_POST['time']))
                     db::query("INSERT INTO ssv_order SET uid={$user['uid']},time='".$_POST->sql('time')."',note='".$_POST->sql('note')."'");
                  if (isset($_POST['booking']))
                     db::query("UPDATE ssv_order SET status=-10 WHERE id=".$_POST->sql('booking'));

                  echo '<img src="'.self::picture($user['picture']).'" style="float:left;margin-bottom:10px;">';
                  echo '<div style="float:left; padding-left:20px;">';
                     echo '<span style="font-size:16px;"><b>'.user::short($user['uid']).'</b></span><br>';
                     echo '<b>Var:</b> '.$user['where'].'<br>';
                     echo '<b>På:</b> '.$user['on_what'].'<br>';
                     echo '<b>Med:</b> '.$user['with_who'].'<br>';
                     echo ($user['sleep_hard']?'Sover hårt<br>':'');
                     echo ($user['snore']=='Ja'?'<b>SNARKAR!</b><br>':'');
                     echo '<br><a onClick="upd('.$x.','.$y.','.$user['uid'].'); return false;">Avboka plats</a>';
                  echo '</div>';
                    
                  echo '<div style="border-top:1px solid #fff;height:200px;clear:both;overflow:auto;padding-top:5px;">';
                  
                     $order = db::fetchAll("SELECT * FROM ssv_order WHERE uid={$user['uid']}");
                     foreach($order as $line) {
                        echo "<div style=\"clear:both;float:left;width:15px;\"><a onClick=\"obokatid({$line['id']},$x,$y);return false;\">X</a></div>";
                        if ($line['time'] < date('Y-m-d H:i:s') && $line['status'] >= 0)
                           echo '<div style="width:150px;float:left;color:#f00">'.$line['time'].'</div>';
                        elseif ($line['status'] < 0)
                           echo '<div style="width:150px;float:left;color:#666">'.$line['time'].'</div>';
                        else
                           echo '<div style="width:150px;float:left;">'.$line['time'].'</div>';

                        if ($line['status'] < 0)
                           echo '<div style="float:left;width:200px;overflow:hidden;color:#666">'.$line['note'].'</div>';
                        else
                           echo '<div style="float:left;width:200px;overflow:hidden;">'.$line['note'].'</div>';
                     }
                  echo '</div>';
                  
                  echo "<form onSubmit=\"bokatid(this,$x,$y);return false;\" style=\"clear:both;margin-top:5px;padding-top:5px;border-top:1px solid #fff;\">";
                        echo '<input name="time" style="clear:both;width:150px;float:left;" value="'.date('Y-m-d H:i:s',strtotime(read('ssv_def_sleeptime','+8 hours','strtotime'))).'">';
                        echo '<input name="note" style="float:left;width:190px;">';
                        echo '<input type="submit" style="float:left;width:50px;" value="Boka">';
                  echo '</from>';

                  $this->display('profile.tpl');
               } elseif ($user = db::fetchSingle("SELECT * FROM ssv_floor WHERE type='bed' AND x={$x} AND y={$y}")) {
                    $this->display('book.tpl');;
               } else
                    echo 'Låst plats!';
					break;

				case 'standard':
					echo 'standard';
					break;
			}
    }

    function _admin() {
        global $tpl;
        if (isset($_POST['text'])&&$_POST['text']>'') {
            $d = explode("|",$_POST['text']);
            foreach ($d as $k => $l) {
                $d[$k] = explode(';',$l);
                if ($d[$k][0]>'') {
                    if (isset($d[$k][2])&&substr($d[$k][2],0,9)=='undefined') $d[$k][2] = 'floor';
                    if ($d[$k][2] != 'floor') 
                        if (!isset($q)) {
                            $q ="({$d[$k][0]},{$d[$k][1]},'{$d[$k][2]}')";
                        } else {
                            $q .=",({$d[$k][0]},{$d[$k][1]},'{$d[$k][2]}')";
                        }
                }
            }   
            db::query("TRUNCATE TABLE ssv_floor");
            db::query("INSERT INTO ssv_floor (x,y,type) VALUES $q");
        }

           $data = db::fetchAll("
                SELECT
                    x,
                    y,
                    type,
                    type as color,
                    status,
                    ssv_booking.uid,
                    MIN(time) as time
                FROM
                    ssv_floor
                LEFT JOIN
                    ssv_booking USING (x,y)
                LEFT JOIN
                    ssv_order ON ssv_order.uid=ssv_booking.uid AND status>=0
                GROUP BY
                    x,y
                ORDER BY time DESC");

            $tpl->assign('ssvdata',$data);

    	$this->display('admin.tpl.php');
    	$this->display('javalist.tpl');
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
