<div class="module" id="paging"><?php
    
    $data = array();
    foreach ($this->paging as $line) {
        $qry = $_GET;
        $qry['page'] = $line['page'];
        $qry = http_build_query($qry);

        if ($line['active'])
            $data[] = "<b>{$line['head']}</b>";
        else
            $data[] = "<a href=\"?$qry\">{$line['head']}</a>";
    }

    echo 'Sida: '.implode($data, ' - ');
?></div>
