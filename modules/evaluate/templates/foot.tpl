<input style="margin-top:20px;" type="submit" value="Skicka in utvärdering">

</form>

<script language="javascript">
    function round(x)
    {
      return Math.floor(x+(x < 0 ? 1 : 0))
    }
    
    function col(value) {
        red = round((1-value) * 150);
        green = round(value * 150);
        blue = 20;
        return '#' + red.toColorPart() + green.toColorPart() + blue.toColorPart();;
    }

    function initSliders() {
        
        sliders = document.getElementsByClassName('slider');
        
        sliders.each(function(node){
            new Control.Slider(node.childNodes[0] , node,
                {
                    onChange:function(v,o){ o.track.childNodes[1].value=v;o.track.style.background = col(v); },
                    onSlide: function(v,o){ o.track.style.background = col(v);},
                    sliderValue: node.childNodes[1].value
                });
            node.style.background = col(node.childNodes[1].value);
        });
    }
    
    initSliders();
</script>
