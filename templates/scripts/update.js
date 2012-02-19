
function setText(text) {
    obj = 'popup';
    obj2 = 'popup2';

    if (text>'') {
        $(obj2).innerHTML = text;
        
        if ($(obj).style.display=='none') {
            Effect.BlindDown(obj);
        } 
            
        Effect.Pulsate(obj2);
        new Effect.Highlight('teamslist',{startcolor:'#999999',endcolor:'#000000'});
        
    } else {
        Effect.BlindUp(obj);
    }
}

function updateUser()
{
    
    var opt = {
        method: 'get',
        onSuccess: function(t) {
            if (!($('popup2').innerHTML == t.responseText)) {
                if (t.responseText=='0') {
                    setText('');
                } else if (t.responseText=='1') {
                    setText('');
                } else {
                    setText(t.responseText);
                }
            }
        },
        on404: function(t) {
            txt = 'Error 404: URLn "' + t.statusText + '" hittades inte.';
             if ($('popup2').innerHTML != txt) {
                setText(txt);
            }           
        },
        onFailure: function(t) {
            txt = 'Error ' + t.status + ' -- ' + t.statusText;
            if ($('popup2').innerHTML != txt) {
                setText(txt);
            }
        }
    };

    new Ajax.Request('/update.php', opt);

    setTimeout("updateUser()",10000);
}


setTimeout("updateUser()",1000);


