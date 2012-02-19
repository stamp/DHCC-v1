<div style="padding:10px;">
<h1>
    <a href="/forum.htm">Forum</a>
<?php

if(isset($this->forum)) echo ' » <a href="/forum/'.$this->forum['url'].'.htm">'.$this->forum['head'].'</a>';
if(isset($this->topic)) echo ' » <a href="/forum/'.$this->forum['url'].'/'.$this->topic['url'].'.htm">'.$this->topic['head'].'</a>';
if(isset($this->action) && $this->action=='reply') echo ' » Nytt svar';

?>
</h1>
</div>
