<?php 
    if (is_array($this->pages)) {
    $bar = '<center><ul style="padding:10px;"><li style="padding:2px 4px;display:inline;border: 1px solid #555;background:#222;color:#999;font-size:10px;">Sida <b>'.$this->pages['page'].'</b> av <b>'.$this->pages['pages'].'</b></li>';
        foreach ($this->pages['list'] as $line) {
            if (isset($line['url'])) {
                $bar .= '<li style="padding:2px 4px;display:inline;border: 1px solid #555;background:#222;border-left:0;font-size:10px;"><a  href="'.$line['url'].'.htm">'.$line['head'].'</a></li>';
            } else {
                $bar .= '<li style="padding:2px 4px;display:inline;border: 1px solid #555;background:#222;border-left:0;color:#999;font-size:10px;">'.$line['head'].'</li>';
            }
        }
    }
    $bar .= '<li style="display:inline;padding-left:70px;">&nbsp;</li></ul></center>';
    echo $bar;
?>
