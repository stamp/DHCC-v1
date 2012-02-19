<script language="javascript" type="text/javascript" src="/modules/tinymce/jscripts/tiny_mce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
	tinyMCE.init({
        mode: "textareas",
        editor_selector : "simple",
		theme : "simple",
        plugins : "advlink",
        extended_valid_elements : "div[class|nick|post|time]",
		content_css : "/modules/tinymce/templates/word.css",
		external_link_list_url : "/modules/tinymce/lists.php?list=links",
		external_image_list_url : "/modules/tinymce/lists.php?list=images",
		paste_auto_cleanup_on_paste : true,
		paste_convert_headers_to_strong : false,
		paste_strip_class_attributes : "all",
		paste_remove_spans : false,
		paste_remove_styles : false,
        apply_source_formatting : true,
        setupcontent_callback : 'fixQuotes',
        save_callback : 'saveQuote',
        force_br_newlines : true
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
