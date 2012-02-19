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
        '�' => chr(140),
        '�' => chr(138),
        '�' => chr(154),
        '�' => chr(129),
        '�' => chr(128),
        '�' => chr(133),
        '�' => chr(142),
        '�' => chr(233),
        '�' => chr(143),
        '�' => chr(135),
        '�' => chr(231),
        '�' => chr(136),
        '�' => chr(203),
        '�' => chr(159),
        '�' => chr(134),
        '�' => chr(216),
        '�' => chr(191),
        '�' => chr(175)
    );
    $txt = str_replace(array_flip($chars),$chars,$txt);
    return $txt;
}

$tpl = new template('templates-new');

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
            $pdf->SetFont('microgammabold','',25);
        $pdf->SetFillColor(0);
        $pdf->SetTextColor(255,255,255);
            $pdf->Rect(0,0,5.39,8.57,'F');

        $pdf->Image('modules/pdf/dhloggovit.jpg',0.1,0.5,5.1,0.7);

        $pdf->Cell(0,8,microgammatext(strtoupper($data['team'])),0,0,'C');
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
    
    $users = array(
    	array(
		'team' => 'ELMIA',
		'username' => '',
		'firstname' => 'Joakim',
		'lastname' =>'Gustafsson'
	),
    	array(
		'team' => 'ELMIA',
		'username' => '',
		'firstname' => 'Mattias',
		'lastname' =>'Englund'
	),
    	array(
		'team' => 'ELMIA',
		'username' => '',
		'firstname' => 'Robin',
		'lastname' =>'Davidsson'
	),
    	array(
		'team' => 'ELMIA',
		'username' => '',
		'firstname' => 'Daniel',
		'lastname' =>'Gustafsson'
	),
	array(
                'team' => 'ELMIA',
                'username' => '',
                'firstname' => '',
                'lastname' =>''
        )
    );
    get();

    foreach ($users as $line) {
        page($line);
    }

    Header('Content-Type: application/pdf');
    $pdf->Output();


?>
