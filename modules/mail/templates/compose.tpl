<style type="text/css">
    #list {
      position:absolute;
      width:250px;
      background-color:white;
      margin:0px;
      padding:0px;
    }
    #list ul {
      list-style-type:none;
      margin:0px;
      padding:0px;
      border-top:1px solid #888;
      border-left: 1px solid #888;
    }
    #list ul li.selected { 
      background-color: #99f;
    }
    #list ul li {
      list-style-type:none;
      display:block;
      margin:0;
      padding:2px;
      cursor:pointer;
      color:#000;
      border:0;
      border-bottom:1px solid #888;
      border-right: 1px solid #888;
    }
    #list ul li span.hit {
        background: #999;
        color:#fff;
    }

</style>
<h1>Skicka mail</h1>
<br>
<form action="" method="post" onSubmit="if(this.subject.value=='') return confirm('Vill du verkligen skicka mailet utan ämne?');">
    <div style="padding:3px;background:#333;margin:0px 10px;">
        <input type="submit" value="Skicka">
    </div>
    <table class="boxcontainer" width="620" id="headers">
        <tr>
            <td style="background:#222;width:100px;"><b>Från</b></td>
            <td style="background:#222;" align="right">
                <select name="from" style="width:520px;">
                    <?php foreach($this->from as $line) { ?>
                    <option value="<?php echo htmlspecialchars($line); ?>"><?php echo htmlspecialchars($line); ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td>
                <span id="indicator" style="display: none;float:right;"><img src="http://www.napyfab.com/ajax-indicators/images/indicator_circle_ball2.gif" alt="Working..." /></span>
                <b>Till</b> 
            </td>
            <td align="right"><input type="text" id="searchto" name="to" value="<?php if(isset($_POST['to'])) echo htmlspecialchars($_POST['to']); ?>" style="width:520px;"></td>
            <div id="list" style="display:none;z-index:10"></div>
            <script language="javascript">
                new Ajax.Autocompleter('searchto','list','/fetch.php?path=<?php echo $this->cleanpath; ?>', {
                paramName: "search",tokens: ',',frequency: 0.2,indicator: 'indicator'})
            </script>
        </tr>
        <tr>
            <td style="background:#222;"><b>Ämne</b></td>
            <td style="background:#222;" align="right"><input type="text" name="subject" value="<?php if(isset($_POST['subject'])) echo htmlspecialchars($_POST['subject']); ?>" style="width:520px;"></td>
        </tr>
    </table>
    
    <textarea name="text" style="margin:0px 10px 10px;width:620px;height:300px;boder:0;"><?php 
	
	$filter = new InputFilter();

	//if(isset($_POST['text'])) echo $filter->process($_POST['text']; 
	if(isset($_POST['text'])) echo $filter->process($_POST['text']); 
	
	
	?></textarea>
    <div style="padding:3px;background:#333;margin:0px 10px;">
        <input type="submit" value="Skicka">
    </div>
</form>
