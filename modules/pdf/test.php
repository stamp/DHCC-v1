<?php
chdir('../../');
require('config.inc');

function bild($picture) {
        if (strlen($picture)==32) {
            $file = 'images/users/thumbs/thumb_'.$picture.'.jpg';
        } else {
            $file = 'images/users/old/'.$picture;
        }

        if (file_exists($file)&&filetype($file)=='file')
            return $file;
        else return false;
}

function microgammatext($txt) {
    
    $chars = array(
        'å' => chr(140),
        'ä' => chr(138),
        'ö' => chr(154),
        'Å' => chr(129),
        'Ä' => chr(128),
        'Ö' => chr(133),
        'é' => chr(142),
        'È' => chr(233),
        'è' => chr(143),
        'á' => chr(135),
        'Á' => chr(231),
        'à' => chr(136),
        'À' => chr(203),
        'ü' => chr(159),
        'Ü' => chr(134),
        'ÿ' => chr(216),
        'ø' => chr(191),
        'Ø' => chr(175)
    );
    $txt = str_replace(array_flip($chars),$chars,$txt);
    return $txt;
}

$tpl = new template('templates-new');

if ($path = new path('path')) {
    
    if (!isset($_GET['path'])) die();

    if($cleanpath = $path->process($_GET['path'])) {
        $tpl->assign('sitehead',$path->head);
    }

    $tpl->assign('cleanpath',$path->clean);

    define('FPDF_FONTPATH','modules/pdf/font/');
    require('fpdf.php');
    
    function page($data) {
        global $pdf;

        $pdf->AddPage();

        //$pdf->SetFont('Arial','B',16);

        $pdf->AddFont('microgamma');
        $pdf->AddFont('microgammabold');
        if (strlen($data['team']) > 9)
            $pdf->SetFont('microgammabold','',10);
        else
            $pdf->SetFont('microgammabold','',16);
        $pdf->SetFillColor(0);
        $pdf->SetTextColor(255,255,255);
            $pdf->Rect(0,0,5.39,8.57,'F');

        $pdf->Image('modules/pdf/dhloggovit.jpg',0.1,0.5,5.1,0.7);

        $pdf->Cell(0,3.5,microgammatext(strtoupper($data['team'])),0,0,'C');
        $pdf->Ln(0);

        if ($picture = bild($data['picture2'])) 
            $pdf->Image($picture,1.5,3,2.3,3.07);
        elseif ($picture = bild($data['picture']))
            $pdf->Image($picture,1.5,3,2.3,3.07);

        $pdf->SetTextColor(255,159,0);
        $pdf->SetFont('microgammabold','',16);
        $pdf->Cell(0,5.2,microgammatext($data['username']),0,0,'C');
        $pdf->Ln(0);

        $pdf->Cell(0,12.9,microgammatext($data['firstname']),0,0,'C');
        $pdf->SetFontSize(13);
        $pdf->Ln(0);

        $pdf->SetFont('microgammabold','',12);
        $pdf->Cell(0,14.1,microgammatext($data['lastname']),0,0,'C');
        $pdf->SetFontSize(12);
        $pdf->Ln(0);
        
        if(isset($data['TA'])) {
        $pdf->SetTextColor(255,255,255);
            if ($data['uid'] == 8)
                $pdf->Cell(0,16,'EL-NISSE',0,0,'C');
            elseif ($data['TA'] == '-TA')
                $pdf->Cell(0,16,'TEAMANSVARIG',0,0,'C');
            elseif ($data['TA'] == '-GA')
                $pdf->Cell(0,16,'GRUPPANSVARIG',0,0,'C');
        }
    }


    $pdf=new FPDF('P','cm',array(5.39,8.57));
    $pdf->setMargins(0,0);
    $pdf->setAutoPageBreak(false);
    
    $users = array(0);

    $in = explode(',',$_GET['users']);
    foreach ($in as $line) 
        if (is_numeric($line))
            $users[] = $line;

    $users = db::fetchAll("
        SELECT *,structure.name as team 
        FROM users 
        JOIN membership 
            USING (uid) 
        JOIN structure 
            ON structure.gid=membership.gid AND NOT structure.name LIKE '-%'
        JOIN events 
            ON events.id=structure.event AND events.active = 'Y' 
        LEFT JOIN user_profile 
            USING (uid) 
        WHERE uid IN (".implode($users,',').")
        GROUP BY uid");
    get();

    foreach ($users as $line) {

        $team = array(
            'parent' => $line['gid'],
            'name' => $line['team'],
            'gid' => $line['gid'],
            'is_team' => $line['is_team']
        );

        while ($team['parent']>0 && $team['is_team'] != 'Y') {
            $team = db::fetchSingle("SELECT parent,gid,name,is_team FROM structure WHERE gid={$team['parent']}");
        }

        $line['team'] = $team['name'];

        $line['TA'] = db::fetchOne("SELECT name FROM structure JOIN membership ON membership.gid=structure.gid AND membership.uid={$line['uid']} WHERE parent={$team['gid']} AND name LIKE '-%' LIMIT 1");

        page($line);
    }
    Header('Content-Type: application/pdf');
    $pdf->Output();


}
?>
