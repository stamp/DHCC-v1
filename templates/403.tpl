<?php
send(E_ERROR,"403: Forbidden");

get();

if (!isset($_SESSION['id'])) { ?>
<div style="padding:10px;">Du �r inte inloggad! Vill du logga in?</div>
    <form method="post" style="width:199px;margin-left:10px;" id="loginform" action="/login.htm" enctype="multipart/form-data" >
        <label style="width:150px;">Anv�ndarnamn
          <input type="text" name="signin_username" onfocus="this.className='hi'"  onblur="this.className='blur';" value="" />
        </label>
        <label style="width:150px;">L�senord
          <input type="password" onfocus="this.className='hi'"  onblur="this.className='blur';" name="signin_password" />
        </label>
          <input type="submit" name="button" class="submit" value="Logga in" />
    </form>
<?php
}

?>
