<?php

include('config.inc');

if (class_exists('user')&&$user = core::load('user')) {
    if (isset($_SERVER['PHP_AUTH_USER'])&&!isset($_SESSION['root'])) {
        $user->signin($_SERVER['PHP_AUTH_USER'],$_SERVER['PHP_AUTH_PW']);
    
        if (isset($_SESSION['access']) && in_array( 'G73',explode(',',str_replace('|','',$_SESSION['access']) ) ) ) {
            $_SESSION['root'] = true;
        } elseif(isset($_SESSION['access'])) {
            $_SESSION = array();
        }
    }
       
    if (!isset($_SESSION['root'])) {
       header('WWW-Authenticate: Basic realm="The controlpanel"');
       header('HTTP/1.0 401 Unauthorized');
       echo 'HTTP/1.0 401 Unauthorized';
       exit;
    }
}

    $tpl = new template();
?> 
<style>
body {
    margin:0;
    padding:0;
}

h1,h2,h3 {
    font-family: verdana;
}

h1 {
    font-size: 24px;
}

h2 {
    font-size: 20px;
    font-style: italic
}

#head {
    height:100px;
    background:#333;
}

#head img {
    padding:10px 20px;
    float:left;
}

#head div#main {
    position:relative;
    top:10px;
    font-size:26px;
    font-family:verdana;
    color:#fff;
}

#head div#minihead {
    position:relative;
    top:20px;
    left: 20px;
    color:#ccc;
    font-family:verdana;
    font-size:15px;
}

#menu {
    height:25px;
    margin-top:-5px;
    background: #333;
}

#menu a {
    float:left;
    height:22px;
    padding-left: 10px;
    padding-right: 10px;
    padding-top: 3px;
    color: #fff;

    font-family: verdana;
    font-size:14px;
    font-weight: 900;
    text-decoration:none;
    margin-left:2px;
}

#menu a:hover {
    background: url(/images/tab.gif) repeat-x #fff;
    color: #37A829;
}

#menu a.selected {
    color: #37A829;
    background: url(/images/tab.gif) repeat-x #fff;
}

#tbl .first {
    border-right: 1px solid #999;
    border-bottom: 0px solid #999;
    width: 1px;
}
#tbl .head {
    background: #333;
    color:#fff;
    font-family : Verdana;
    font-size : 10px;
    border-right: 1px solid #555;
    border-top: 1px solid #555;
    border-bottom: 1px solid #555;
    padding-left: 3px;
    padding-right:  3px;
}

#tbl .tbl {
    font-family : Verdana;
    font-size : 10px;
    border: 1px solid #999;
    border-left: 0;  
    border-top: 0;  
    padding: 1px 3px;
}

pre {
	padding:10px;
	border: 1px solid #ccc;
}

#result {
    position:absolute;
    top:150px;
    margin:20px;
    background:#fff;
    border:1px solid #999;
    padding:10px;

}

#result a {
    color: #c00;
    font-weight: 900;
}

</style>


<?php

$manifest = new manifest();


if (isset($_GET['page']) && $_GET['page']=='module' && isset($_GET['action'])&&isset($_GET['module'])) {
	    echo '<div id="result"><a onClick="document.getElementById(\'result\').style.display=\'none\';">Close</a>';
		switch ($_GET['action']) {
			case 'install':
				$manifest->install($_GET['module']);
				break;
			case 'activate':
				$manifest->activate($_GET['module']);
				break;
			case 'disable':
				$manifest->disable($_GET['module']);
				break;
			case 'send':
				$manifest->sendModule($_GET['module']);
				break;
			case 'get':
				$manifest->getModule($_GET['module']);
                if ($_GET['module']!='core')
    				$manifest->install($_GET['module']);
				break;
		}
        echo '</div>';
}


$manifestdata = $manifest->getData();

get();
$folders = scandir('modules/');
$modules = array();

foreach ($folders as $key => $line) {
    if (filetype('modules/'.$line)=='dir'&&$line!='.'&&$line!='..') {
        if ($mod = core::load($line,true)) {
            $modules[$line] = array();
            $modules[$line]['moduleName'] = $line;
            $modules[$line]['id']            = $mod->_id;
            $modules[$line]['name']          = $mod->_name;
            $modules[$line]['description']   = $mod->_desc;
            $modules[$line]['version']       = $mod->_version;
            $modules[$line]['methods']       = core::getMethods($mod);
            $modules[$line]['status']        = core::status($mod,$mod->_version);
            $modules[$line]['changed']       = $manifest->getChanged($mod->_id);
            $modules[$line]['siteversion']   = (isset($manifestdata[$line]['Version'])) ? $manifestdata[$line]['Version'] : '<font color="#999999">not found</font>';
            $modules[$line]['sitechanged']   = (isset($manifestdata[$line]['Date'])) ? $manifestdata[$line]['Date'] : '0';
            unset($mod);
        }
    }
}

if (!isset($_GET['page'])||(isset($_GET['page']) && $_GET['page']=='module')) {
    // getImg {{{
    function getImg ($val) {
        $status = array (
            0 => '&nbsp;',
            'active' => '<div style="background:#0f0;height:8px;width:8px;float:left;margin-top:3px;"></div>',
            'disabled' => '<div style="background:#ccc;height:8px;width:8px;float:left;margin-top:3px;"></div>',
            'installed' => '<div style="background:#cc0;height:8px;width:8px;float:left;margin-top:3px;"></div>">',
            'not installed' => '<div style="background:#f00;height:8px;width:8px;float:left;margin-top:3px;"></div>'
        );

        if ( !isset($status[$val]) || !$val ) {
            return $status['not installed'];
        }

        return $status[$val];
    }
    // }}}
    // getAction {{{
    function getAction ($val,$module) {
        $status = array (
            0 => '&nbsp;',
            'active' => '<a href="?page=module&action=disable&module='.$module.'">Disable</a>',
            'disabled' => '<a href="?page=module&action=activate&module='.$module.'">Activate</a>',
            'installed' => '<a href="?page=module&action=activate&module='.$module.'">Activate</a>',
            'not installed' => '<a href="?page=module&action=install&module='.$module.'">Install</a>'
        );

        if ( !isset($status[$val]) || !$val ) {
            return $status['not installed'];
        }

        return $status[$val];
    }
    // }}}
}


$data = $modules;

function version($req,$av) {
    if(core::isCompatibleWithApi(explode('.',$req),explode('.',$av))) 
        return  "<font color=\"00CC00\">$req</font>";
    return "<font color=\"CC0000\">$req</font>";
}

?>
<div id="head">
    <img src="/lib/images/butterfly.gif">
    <div id="minihead">
        stamps
    </div>
    <div id="main">
        Superadmin control panel <span style="font-size:14px;color:#777;">v1.0</span>
    </div>
</div>
<div id="menu">
    <a href="?page=module"<?php if (!isset($_GET['page'])||(isset($_GET['page']) && $_GET['page']=='module')) echo 'class="selected"'; ?>>Modules</a>
    <?php
        foreach ($modules as $line)
            if($line['status'] == 'active' && in_array('_admin',$line['methods'])) {
                echo "    <a href=\"?page={$line['id']}\"";
                if (isset($_GET['page']) && $_GET['page']==$line['id']) echo ' class="selected"';
                echo ">{$line['name']}</a>";
            }
    ?>
</div>

   <div style="padding:10px;">
<?php
if (!isset($_GET['page'])||(isset($_GET['page']) && $_GET['page']=='module')) {
?>

    <h1>Modules</h1>

    <h2>Core</h2>
        <table border=0 cellpadding=0 cellspacing=0 id="tbl" >
            <tr>
                <td colspan="3" class=first>&nbsp;</td>
                <td colspan="3" class=head style="border-bottom:0;" align="center">Api versions</tr>
            </tr>
            <tr>
                <td >&nbsp;</td>
                <td class=first style="border-bottom: 1px solid #999">&nbsp;</td>
                <td class=head>Libs</td>
                <td class=head>Core</td>
                <td class=head>Module</td>
                <td class=head>DB</td>
                <td class=head>Action</td>
            </tr>
            <tr>
                <td class=first>&nbsp;</td>
                <td class=tbl><b>Installed</b></td>
                <td class=tbl><?php echo date('Y-m-d H:i',$manifest->getChanged('core')); ?></td>
                <td class=tbl><?php echo core::version; ?></td>
                <td class=tbl><?php echo module::version; ?></td>
                <td class=tbl><?php echo db::version; ?></td>
                <td class=tbl><?php if (isset($manifestdata['core']) && ($manifestdata['core']['Date']<$manifest->getChanged('core')) ) echo '<a href="?page=module&action=send&module=core">Upload</a>'; ?>&nbsp;</td>
            </tr>
            <?php if (isset($manifestdata['core'])) { ?>
            <tr>
                <td class=first>&nbsp;</td>
                <td class=tbl><b>Public</b></td>
                <td class=tbl><?php echo date('Y-m-d H:i',$manifestdata['core']['Date']); ?></td>
                <td class=tbl><?php echo $manifestdata['core']['Core'] ?></td>
                <td class=tbl><?php echo $manifestdata['core']['Module'] ?></td>
                <td class=tbl><?php echo $manifestdata['core']['Db'] ?></td>
                <td class=tbl><?php if ($manifestdata['core']['Date']>$manifest->getChanged('core') ) echo '<a href="?page=module&action=get&module=core">Update</a>'; ?>&nbsp;</td>
            </tr>
            <?php } ?>

        </table>

    <h2>Modules on site</h2>
    <?php if (isset($modules)&&is_array($modules)) { ?>
        <table width="100%" border=0 cellpadding=0 cellspacing=0 id="tbl" width="650">
            <tr>
                <td class=first>&nbsp;</td>
                <td class=head>Module</td>
                <td class=head>ModuleVersion</td>
                <td class=head>PublicVersion</td>
                <td class=head>Description</td>
                <td class=head>Status</td>
                <td class=head>Methods</td>
                <td class=head>Status</td>
                <td class=head>Action</td>
            </tr>
        <?php foreach ($modules as $key => $line) { ?>
            <tr>
                <td class=first>&nbsp;</td>
                <td class=tbl valign="top" nowrap><b><?php echo $line['id']; ?></b><br><?php echo $line['name']; ?></td>
                <td class=tbl valign="top" nowrap <?php if ($line['changed']<$line['sitechanged']) echo 'style="color:#c00;"'?>><b><?php echo $line['version']; ?></b><?php if ($line['changed']<$line['sitechanged']) echo '- <b>!OLD!</b>'?><br><?php echo date('Y-m-d H:i',$line['changed']); ?></td>
                <td class=tbl valign="top" nowrap <?php if ($line['changed']>$line['sitechanged']) echo 'style="color:#c00;"'?>><b><?php echo $line['siteversion']; ?></b><?php if ($line['sitechanged']>0&&$line['changed']>$line['sitechanged']) echo '- <b>!OLD!</b>'?><br><?php if($line['sitechanged']>0) echo date('Y-m-d H:i',$line['sitechanged']); ?></td>
                <td class=tbl valign="top"><?php echo $line['description']; ?></td>
                <td class=tbl valign="top"><?php echo getImg($line['status']); ?><?php echo $line['status']; ?></td>
                <td class=tbl valign="top"><?php echo implode($line['methods'],'<br>'); ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo getAction($line['status'],$line['id']); ?></td>
                <td class=tbl valign="top">
                    <?php if ($line['changed']>$line['sitechanged'] || $line['changed']=='') { ?>
                    <a href="?page=module&action=send&module=<?php echo $line['id']; ?>">Upload</a>
                    <?php } elseif ($line['changed']<$line['sitechanged']) { ?>
                    <a href="?page=module&action=get&module=<?php echo $line['id']; ?>">Update</a>
                    <?php } ?>&nbsp;
                </td>
            </tr>
        <?php } ?>
        </table>
    <?php } ?>

    <h2>Updates</h2>

    <?php if (isset($manifestdata)&&is_array($manifestdata)) { ?>
        <table width="100%" border=0 cellpadding=0 cellspacing=0 id="tbl" width="650">
            <tr>
                <td colspan="7" class=first>&nbsp;</td>
                <td colspan="3" class=head style="border-bottom:0;" align="center">Requires Api</tr>
            </tr>
            <tr>
                <td class=first>&nbsp;</td>
                <td class=head>&nbsp;</td>
                <td class=head>Module</td>
                <td class=head>Version</td>
                <td class=head>Date</td>
                <td class=head>Description</td>
                <td class=head>Maintainer</td>
                <td class=head>Core</td>
                <td class=head>Module</td>
                <td class=head>DB</td>
                <td class=head>Depends</td>
                <td class=head>Action</td>
            </tr>
        <?php foreach ($manifestdata as $key => $line) {
            if ( (substr($key,0,1)!='_' && (!isset($modules[ $line['Source'] ]) || $modules[ $line['Source'] ]['changed']<$line['Date']))&&$line['Source']!='core') { 
            ?>
            <tr>
                <td class=first>&nbsp;</td>
                <td class=tbl width=1 valign="top" style="padding-top:3px;">&nbsp;</td>
                <td class=tbl valign="top" nowrap><?php echo $line['Name']; ?> (<?php echo $line['Source']; ?>)</td>
                <td class=tbl valign="top"><?php echo $line['Version']; ?></td>
                <td class=tbl valign="top"><?php echo date('Y-m-d H:i',$line['Date']); ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo $line['Description']; ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo isset($line['Maintainer'])?htmlspecialchars($line['Maintainer']):''; ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo version(isset($line['CoreApi'])?$line['CoreApi']:'',core::version); ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo version(isset($line['ModuleApi'])?$line['ModuleApi']:'',module::version); ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo version(isset($line['DbApi'])?$line['DbApi']:'',db::version); ?>&nbsp;</td>
                <td class=tbl valign="top"><?php echo isset($line['Build-Depends'])?$line['Build-Depends']:''; ?>&nbsp;</td>
                <td class=tbl valign="top">
                <?php if(!isset($modules[ $line['Source'] ])) { ?>
                    <a href="?page=module&action=get&module=<?php echo $line['Source']; ?>">Install</a>
                <?php } else { ?>
                    <a href="?page=module&action=get&module=<?php echo $line['Source']; ?>">Update</a>
                <?php } ?>
                </td>
            </tr>
        <?php }} ?>
        </table>
    <?php } ?>
<?php } else {
    core::run($_GET['page'],'_admin');
} 
    get();
?>
</div>
