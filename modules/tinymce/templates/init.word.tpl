<script language="javascript" type="text/javascript" src="/modules/tinymce/jscripts/tiny_mce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
	tinyMCE.init({
        mode: "textareas",
        editor_selector : "editor",
		theme : "advanced",
        extended_valid_elements : "div[class|nick|post|time]",
		plugins : "table,save,advhr,advimage,advlink,emotions,insertdatetime,preview,zoom,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1_add_before : "save,newdocument,separator",
		theme_advanced_buttons1_add : "fontselect,fontsizeselect",
		theme_advanced_buttons2_add : "separator,preview,zoom,separator,forecolor,backcolor",
		theme_advanced_buttons2_add_before: "cut,copy,paste,pastetext,pasteword,separator",
		theme_advanced_buttons3_add_before : "tablecontrols,separator",
		theme_advanced_buttons3_add : "emotions,separator,print,separator,fullscreen",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "bottom",
		content_css : "/modules/tinymce/templates/word.css",
		external_link_list_url : "/modules/tinymce/lists.php?list=links",
		external_image_list_url : "/modules/tinymce/lists.php?list=images",
		paste_use_dialog : false,
		theme_advanced_resizing : true,
		theme_advanced_resize_horizontal : false,
		paste_auto_cleanup_on_paste : true,
		paste_convert_headers_to_strong : false,
		paste_strip_class_attributes : "all",
		paste_remove_spans : false,
		paste_remove_styles : false,
        apply_source_formatting : true,
        setupcontent_callback : 'fixQuotes',
        save_callback : 'saveQuote'
	});

    function fixQuotes(editor_id, body, doc) {
        html = body.innerHTML.replace(/<quote/gi,'<div class="quote" ');
        html = html.replace(/<\/quote>/gi,'</div>');

        body.innerHTML = html+'<br />';
    }

    function saveQuote(element_id, html, body) {
        html = html.replace(/<div class=\"quote\"/gi,'<quote ');
        html = html.replace(/<\/div>/gi,'</quote>');
        html = html.replace(/\n/g,"");
        html = html.replace(/<br \/>/gi,"\n");

        return html;
    }

</script>
