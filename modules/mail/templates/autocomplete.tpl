<ul>
<?
    if (isset($this->search)&&is_array($this->search))
        foreach ($this->search as $line) {
            echo "<li><b>{$line['username']}</b><span class=\"informal\" style=\"color:#555\"> <i>{$line['name']}</i></span></li>";
        }
?>
</ul>
