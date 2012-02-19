<h1>Path admin</h1>
<a href="?add=1<? if(isset($_GET['page'])) echo '&page='.$_GET['page']; ?>">Add a new path</a><br>
<br>
<?php

$modules = core::getModules();
foreach ($modules as $key => $line) {
    $modules[$key] = array();
    $modules[$key]['id'] = $line;
    $modules[$key]['methods'] = core::getMethods($line);
}

function printRow($level,$data,$modules) {
    echo "\t<tr class=\"{$data['type']}\" id=\"row{$data['tid']}\">\n";
    echo "\t\t<td class=\"first\"><img src=\"".getImg($data['status'])."\"></td>";
    echo "\t\t<td nowrap>";
        for($i=0;$i<$level;$i++) 
            echo "<div style=\"width:20px;float:left;\">&nbsp;</div>";
    echo "<a href=\"?edit={$data['tid']}";
        if(isset($_GET['page'])) echo '&page='.$_GET['page'];
    echo "\">{$data['path']}</a></td>\n";
    echo "\t\t<td class=noh><a href=\"?up={$data['tid']}\"><img border=0 src=\"/modules/pathadmin/templates/images/up.gif\"></a></td>\n";
    echo "\t\t<td class=noh><a href=\"?down={$data['tid']}\"><img border=0 src=\"/modules/pathadmin/templates/images/down.gif\"></a></td>\n";
    if ($data['type']=='normal') { 
        echo "\t\t<td class=noh><input type=\"text\" name=\"head-{$data['tid']}\" value=\"{$data['head']}\" onChange=\"changed({$data['tid']})\"></td>\n";
        echo "\t\t<td class=noh>".selectModule($data['tid'],$data['module'],$modules)."</td>\n";
        echo "\t\t<td class=noh>".selectMethod($data['tid'],$data['module'],$data['vars'],$modules)."</td>\n";
        echo "\t\t<td class=noh>".selectStatus($data['tid'],$data['status'])."</td>\n";
    } elseif ($data['type']=='redir') {
        echo "\t\t<td colspan=2>!!redir!! -> points to </td>\n";
        echo "\t\t<td class=noh><input type=\"text\" name=\"head-{$data['tid']}\" value=\"{$data['redir']}\" onChange=\"changed({$data['tid']})\"></td>\n";
        echo "\t\t<td class=noh>".selectStatus($data['tid'],$data['status'])."</td>\n";
    } elseif ($data['type']=='var') {
        echo "\t\t<td colspan=2>!!variable!! -> points to </td>\n";
        echo "\t\t<td class=noh><input type=\"text\" name=\"head-{$data['tid']}\" value=\"{$data['vars']}\" onChange=\"changed({$data['tid']})\"></td>\n";
        echo "\t\t<td class=noh>".selectStatus($data['tid'],$data['status'])."</td>\n";
    } elseif ($data['type']=='extend') {
        echo "\t\t<td >!!EXTENDED PATH!!</td>\n";
        echo "\t\t<td class=noh>".selectModule($data['tid'],$data['module'],$modules)."</td>\n";
        echo "\t\t<td class=noh>".selectMethod($data['tid'],$data['module'],$data['vars'],$modules)."</td>\n";
        echo "\t\t<td class=noh>".selectStatus($data['tid'],$data['status'])."</td>\n";
    } elseif ($data['type']=='include') {
        echo "\t\t<td colspan=2>!!include!! -> points to </td>\n";
        echo "\t\t<td class=noh><input type=\"text\" name=\"module-{$data['tid']}\" value=\"{$data['module']}\" onChange=\"changed({$data['tid']})\"></td>\n";
        echo "\t\t<td class=noh>".selectStatus($data['tid'],$data['status'])."</td>\n";
    }
    echo "\t\t<td class=noh>";
        echo pathadmin::helper('access-'.$data['tid'],'',$data['access']);
    echo "</td>\n\t</tr>\n";
}

function selectModule($id,$def,$modules) {
    $ret = '<select name="module-'.$id.'" id="module'.$id.'" onChange="changeModule(\''.$id.'\',this.options[this.selectedIndex].value)">';
        $ret .= '<option value="" style="background:#ff9">none</option>';
    foreach ($modules as $line) {
        $ret .= '<option';
            if ($line['id'] == $def) $ret .= ' selected';
            if (count($line['methods'])==0) $ret .= ' style="background:#fcc"';
        $ret .= '>'.$line['id'].'</option>';
    }
    $ret .= '</select>';
    return $ret;
} 

function selectMethod($id,$mod,$def,$modules) {
    foreach ($modules as $line) {
        if ($line['id']==$mod) {
            foreach ($line['methods'] as $line2) {

                if (!isset($ret))
                    $ret =  '<select name="method-'.$id.'" id="method'.$id.'" onChange="changed('.$id.')"><option value="" style="background:#ff9">none</option>';

                $ret .= '<option';
                    if ($line2 == $def) $ret .= ' selected';
                $ret .= '>'.$line2.'</option>';               
            }
            break;    
        }
    }
    if (!isset($ret))
        $ret =  '<select name="method-'.$id.'" id="method'.$id.'" disabled>';
    $ret .= '</select>';
    return $ret;
}

function selectStatus($id,$def) {
    $opt = array('active','disabled','develop','hidden');
    $ret =  '<select name="status-'.$id.'" id="status'.$id.'" onChange="changed('.$id.')">';
    foreach ($opt as $line) {
        $ret .= '<option style="background: url('.getImg($line).') 2px 5px no-repeat;padding-left:12px;"';
            if ($line == $def || ($def == '' && $line == 'disabled')) $ret .= ' selected';
        $ret .= '>'.$line.'</option>';
    }
        
    $ret .= '</select>';
    return $ret;
}

function printTree($tree,$modules,$level = 0) {
    
    if (isset($tree)&&is_array($tree))
        foreach ($tree as $line) {
            printRow($level,$line,$modules);
            if (isset($line['childs'])&&is_array($line['childs'])) printTree($line['childs'],$modules,($level+1));
        }
    else
        printRow($level,$line,$modules);

}
    // getImg {{{
    function getImg ($val) {
        $status = array (
            0 => '&nbsp;',
            'active' => 'http://www.stamp.se/dh/images/GreenSquare.png',
            'disabled' => 'http://www.stamp.se/dh/images/GreySquare.png',
            'develop' => 'http://www.stamp.se/dh/images/YellowSquare.png',
            'hidden' => 'http://www.stamp.se/dh/images/RedSquare.png'
        );

        if ( !isset($status[$val]) || !$val ) {
            return $status['disabled'];
        }

        return $status[$val];
    }
    // }}}

?>
<script language="javascript">
function changeModule(obj,value) {
    changed(obj);
    obj = 'method'+obj;
    if (value == '') {
        document.getElementById(obj).options.length = 0;
        document.getElementById(obj).disabled = 'disabled';
    } else {
        data = eval(value+'Methods');
        
        document.getElementById(obj).options.length = 0;
        
        if (data.length == 0 ) {
            document.getElementById(obj).disabled = 'disabled';
        } else {
            document.getElementById(obj).disabled = '';
            document.getElementById(obj).options[0] = new Option('none','');    

            for(i = 0; i < data.length; i++) {
             document.getElementById(obj).options[i + 1] = new Option(data[i],data[i]);
            }
        }
    }
}

function changed(obj) {
    document.getElementById('row'+obj).className = 'edited';
}

<?php
    foreach ($modules as $key => $line)
        if (count($line['methods'])>0)
            echo "{$line['id']}Methods = new Array('".implode($line['methods'],"','")."');\n";
        else
            echo "{$line['id']}Methods = new Array();\n";
?></script>
<style>
#tbl  {
    border-left: 1px solid #999;
}
#tbl .head {
    background: #ddd;
    font-family : Verdana;
    font-size : 10px;
    border-right: 1px solid #aaa;
    border-top: 1px solid #aaa;
    border-bottom: 1px solid #aaa;  
    padding-left: 3px;
    padding-right:  3px;
    font-weight: 900;
}

#tbl td {
    font-family : Verdana;
    font-size : 10px;
    border-right: 1px solid #aaa;
    border-bottom: 1px solid #aaa;  
    padding: 1px 3px;
}

#tbl .var {
    background:#ddf;
}

#tbl .redir {
    background:#ddd;
}
#tbl .noh {
    padding:0;
}

#tbl .edited {
    background:#f99;
}

#tbl .tbl div {
    color:#ccF;
    cursor:pointer;
}

#tbl select {
    width:100px;
    border:0;
}

#tbl input {
    border:0;
}

</style>
<form action="" method="post">
<table id="tbl" cellpadding=0 cellspacing=0 >
    <tr>
        <td class="head">&nbsp;</td>
        <td class="head" colspan=3 width=200>Path</td>
        <td class="head">Head</td>
        <td class="head">Module</td>
        <td class="head">Method</td>
        <td class="head">Status</td>
        <td class="head">Access</td>
    </tr>
<?php printTree($this->tree,$modules); ?>

</table>

<input type="submit" value="Save">

</form>
