<html>
<head>
   <title> <?php if(isset($this->sitehead)) echo $this->sitehead;?></title>
   <link href="/templates/style/base.css" rel="stylesheet">
   <link href="/templates/style/form.css" rel="stylesheet">
   <link href="/templates/style/error.css" rel="stylesheet">
   <link href="/templates/style/forum.css" rel="stylesheet">
   <link href="/templates/style/modules.css" rel="stylesheet">
   <script type="text/javascript" src="/templates/scripts/update.js"></script>
   <script type="text/javascript" src="/modules/aculo/lib/prototype.js"></script>
   <script type="text/javascript" src="/modules/aculo/src/scriptaculous.js"></script>
   <script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
   </script>
   <script type="text/javascript">
   _uacct = "UA-947636-1";
   urchinTracker();
   </script>
</head>
<body>

<div id="blur" style="position:absolute;z-index:10;background:#333;height:100%;width:100%;opacity: .5;filter: alpha(opacity=50);top:0px;left:0px;display:none;"></div>
<table border="0" cellspacing="0" cellpadding="0" width="100%" height="100%">
<tr><td align="center">
<table id="mainTbl" border="0" cellspacing="0" cellpadding="0">
    <tr>
        <td id="head" colspan="2"><img src="/templates/images/head.png"/></td>
    </tr>
    <tr> 
        <td id="teamslist">
            <div id="popup" style="display:none;border-bottom:1px solid #fff;"><div id="popup2" style="padding:10px;"></div></div>
            <div>
            <?php
                if (isset($this->events) && is_array($this->events)) { 
                    $this->display('../modules/events/templates/getEventSelect.tpl'); 
                } 
                
                if (isset($this->teamlist)) {
                    echo $this->teamlist;
                } elseif (isset($this->news)&&is_array($this->news)) { ?>
            <div style="padding:10px;"><h1>Nyheter</h1>
            <?php    foreach ($this->news as $line) { ?>
                <h3><?php echo $line['head'] ;?></h3><p><?php echo $line['text'] ;?></p>
            <?php   } ?>
            </div>
            <?php
                }

            ?>
            </div>
        </td>
        <td id="main" valign="top">
            <table border="0" cellspacing="0" cellpadding="0">
                <tr>
                    <td id="menu">
                        <ul>
                        <?php
                            if (isset($this->menu[0]) && is_array($this->menu)) {
                                $tmp = split('/',$this->cleanpath);
                                $lp = '/'.$tmp[1];
                                foreach ($this->menu[0] as $line) {
                                    if ($lp == $line['path']) { ?>
                            <li class="selected"><b><a href="<?php echo $line['path']; ?>.htm"><?php echo $line['head']; ?></a></b></li>        
                        <?php       } else { ?>
                            <li><b><a href="<?php echo $line['path']; ?>.htm"><?php echo $line['head']; ?></a></b></li>
                        <?php       }
                                }            
                            }
                        ?>
                        </ul>
                    </td>
                </tr>
                <?php
                    if (isset($this->menu[1]) && is_array($this->menu)) { 
                    $this->menu = array_slice($this->menu,1);
                        foreach ($this->menu as $key => $menu) {
                            if (isset($tmp[($key+2)]))
                                $lp .= '/'.$tmp[($key+2)];
                    ?>
                <tr>
                    <td id="submenu">
                        <ul>
                        <?php
                                foreach ($menu  as $line) {
                                    if ($lp == $line['path']) { ?>
                            <li class="selected"><a href="<?php echo $line['path']; ?>.htm"><?php echo $line['head']; ?></a></li>
                        <?php       } elseif ($line['path']=='') { ?> 
                            <li class="selected"><b><?php echo $line['head']; ?></b></li>
                        <?php       } else { ?>
                            <li><a href="<?php echo $line['path']; ?>.htm"><?php echo $line['head']; ?></a></li>
                        <?php       }
                                }            
                        ?>
                        </ul>
                    </td>
                </tr>
                <?php
                    }
                }
                ?>
                <tr>
                    </td>
                </tr>
            </table>
             
