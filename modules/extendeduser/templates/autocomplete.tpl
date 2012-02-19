<?php
echo '<ul class="contacts">';
            foreach($this->data as $line) { 
                echo '<li class="contact">';
                echo '<div class="name" style="display:none;">'.$line['username'].'</div>';
                echo '<div class="informal">';
                echo $line['uid'];
                echo '</div>';
                echo '<span class="informal">';
                echo preg_replace("@".$this->string."@i",'<span style="background:#000;color:#fff;">'.$this->string.'</span>',$line['firstname']);
                echo ' ';
                echo preg_replace("@".$this->string."@i",'<span style="background:#000;color:#fff;">'.$this->string.'</span>',$line['lastname']);
                echo '</span>';
                echo '<div class="informal">';
                echo preg_replace("@".$this->string."@i",'<span style="background:#000;color:#fff;">'.$this->string.'</span>',$line['city']);
                echo '</div>';
                echo '<span class="informal">';
                echo '</li>';
            }
echo '</ul>';
?>
