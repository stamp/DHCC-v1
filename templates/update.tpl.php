<?php

if ($this->gb==1) {
    echo '<a href="/users/'.path::encode($this->user).'/guestbook.htm"><b>1</b> nytt gastboksinlagg</a><br>';
} elseif ($this->gb>1) {
    echo '<a href="/users/'.path::encode($this->user).'/guestbook.htm"><b>'.$this->gb.'</b> nya gastboksinlagg</a><br>';
}


if ($this->mail==1) {
    echo '<a href="/users/'.path::encode($this->user).'/mail.htm"><b>1</b> nytt mail</a>';
} elseif ($this->mail>1) {
    echo '<a href="/users/'.path::encode($this->user).'/mail.htm"><b>'.$this->mail.'</b> nya mail</a>';
}

if ($this->gb==0&&$this->mail==0) 
echo '0';

?>
