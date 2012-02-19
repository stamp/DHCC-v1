<?php if (!isset($_SESSION['id'])) { ?>
    <table width="100%" height="100%" cellspacing=0 cellpadding=0 border=0>
        <tr>
            <td style="padding:10px;" valign="top">
                <h3>V�lkommen till Crew Corner</h3>
					Det h�r �r v�rt community f�r all v�r
					crew. H�r finns diskussionsforum och annat sm�tt och gott.
					H�r planeras teamen, arbetsuppgifter, t�vlingar, m.m. Alla i
					crew har en egen personlig sida med g�stbok och internmail.<br><br>
					<h3>Vill du bli crew?</h3>
					Inf�r varje DreamHack beh�vs det nytt folk i crew. Om du �r
					intresserad av att vara med kan du g�ra en ans�kan via
					l�nken i menyn l�ngst upp. Vi tar generellt inte in nya
					personer i crew som inte fyllt 18 innan DreamHack. Alla i crew jobbar id�ellt.
                </div>
            </td>
            <td width="199" style="border-left: 1px solid #FFFFFF;padding:10px;"  valign="top">
                <form method="post" id="loginform" action="/login.htm" enctype="multipart/form-data" >
                    <label style="width:150px;">Anv�ndarnamn
                      <input type="text" name="signin_username" onfocus="this.className='hi'"  onblur="this.className='blur';" value="" />
                    </label>
                    <label style="width:150px;">L�senord
                      <input type="password" onfocus="this.className='hi'"  onblur="this.className='blur';" name="signin_password" />
                    </label>
                      <input type="submit" name="button" class="submit" value="Logga in" />
                </form>
<script language="javascript">document.getElementById('loginform').signin_username.focus()</script>

            </td>
		</tr>
        <tr>
            <td colspan="2" style="border-top: 1px solid #FFFFFF;height:239px;">
                <img border="0" src="/images/crewbild-dhw06.jpg" width="100%">
            </td>
        </tr>
    </table>
<?php } else { ?>

            <div style="padding:10px;padding-bottom:0">                
                <h1>V�lkommen in i v�rmen!</h1>
                <p>
                Du har nu kommit till Crew Corner, ett community f�r all DreamHack Crew! H�r kan du enkelt f� senaste infon, ans�ka till det team du vill jobba i, titta vilka som jobbar i de andra teamen, m.m. Efter hand kommer det att finnas en hel massa saker h�r, s� titta in ofta! Det b�sta av allt �r att Crew Corner kommer att vara tillg�ngligt �ret runt!
                </p>

            </div>
            <?php if ($path->write) {
                if (isset($_POST['head'])&&isset($_POST['text'])&&isset($_POST['access'])) {
                    $news = core::load('news');
                    if ($_POST['access'] == 'G0,')
                        $news->addPost(1,$_POST['head'],nl2br($_POST['text']),$_POST['access']);
                    else
                        $news->addPost(3,$_POST['head'],nl2br($_POST['text']),$_POST['access']);
                }
                ?>
                <div style="padding-left:10px;"><a onClick="new Effect.BlindDown('ny');this.style.display='none'">Skriv en nyhet</a></div>
                <div class="boxcontainer" id="ny" style="display:none;">
                    <form action="" method="post">
                    <div class="boxhead">
                        <span style="float:right;font-size:10px;margin-bottom:5px;">Nytt inl�gg</span>
                        <input style="width:300px" name="head">
                    </div>
                    <div style="padding:5px;">
                        <?php $u = $db->fetchOne("SELECT username FROM users WHERE uid={$_SESSION['id']}"); ?>
                        <i style="font-size:10px;">Skrivet av <b><a href="/users/<?php echo path::encode($u); ?>"><?php echo $u; ?></a></b> till </i>
                        <select name="access" style="display:inline;">
                            <option value="G-3,">Nuvarande crew</option>
                            <option value="G10,">Tidigare crew</option>
                            <option value="G-1,">Alla</option>
                            <option value="G0,">Nyhet p� utsidan (bara rubriken syns!)</option>
                        </select>
                        <div style="margin-top:5px;font-size:11px;">
                            <textarea name="text" style="width:100%;height:200px"></textarea>
                            <input type="submit" style="float:right;">
                        </div>
                    </div>
                    </form>
                </div>
            <?php } ?>
            <?php
                function getPosts() {
                    
                    $posts = db::fetchAll("SELECT *,NOT FIND_IN_SET({$_SESSION['id']},new) AS new,new as readlist,username FROM news,users WHERE access REGEXP '.*({$_SESSION['access']}).*' AND users.uid=writer ORDER BY id DESC LIMIT 5");

                    foreach($posts as $key => $line) 
                        if ($line['new'])
                            db::query("UPDATE news SET new='".trim($line['readlist'].','.$_SESSION['id'],',')."' WHERE id=".$line['id']);
                    
                    return $posts;

                }


                $news = getPosts();           
                if (isset($news)&&is_array($news)) foreach ($news as $line) { ?>

                <div class="boxcontainer<?php if ($line['new']) echo " highlight";?>" >
                    <div class="boxhead">
                        <span style="float:right;font-size:10px;margin-bottom:5px;"><?php echo timestamp($line['timestamp']);  ?></span>
                        <?php echo $line['head'];  ?>
                    </div>
                    <div style="padding:5px;">
                        <i style="font-size:10px;">Skrivet av <b><a href="/users/<?php echo path::encode($line['username']); ?>"><?php echo $line['username'];  ?></a></b></i>
                        <div style="margin-top:5px;font-size:11px;">
                        <?php echo $line['text'];  ?>
                        
                        </div>
                    </div>
                </div>
            <?php } ?>

 
<?php } ?>
