
jQuery(document).ready(function($){
	var currentdevice = '';
	var currentdevice2 = '';
	var bootstrap_elements = $('#bd [class*="span"]');
	var item_g_elements = $('#bd [class*="sns-item-g"]');
	// Build data
	bootstrap_elements.each ( function(){
		var $this = $(this);
		// With attr data-*
		$this.data();
		// Make the source better view in inspector
		$this.removeAttr ('data-default data-wide data-normal data-tablet data-mobile');
		// For element no attr data-default
		if (!$this.data('default'))
			$this.data('default', $this.attr('class'));
	});

	function updateBootstrapElementClass(newdevice){
  		if (newdevice == currentdevice) return ;
		bootstrap_elements.each(function(){
			var $this = $(this);
			// Default
			if ( !$this.data('default') || (!$this.data(newdevice) && (!currentdevice || !$this.data(currentdevice))) )
				return;
			// Remove current
			if ($this.data(currentdevice)) $this.removeClass($this.data(currentdevice));
			else $this.removeClass ($this.data('default'));
			// Add new
			if ($this.data(newdevice)) $this.addClass ($this.data(newdevice));
			else $this.addClass ($this.data('default'));
		});
    	currentdevice = newdevice;
	};

	function viewport() {
	    var e = window, a = 'inner';
	    if (!('innerWidth' in window )) {
	        a = 'client';
	        e = document.documentElement || document.body;
	    }
	    return { width : e[ a+'Width' ] , height : e[ a+'Height' ] };
	}
	function detectDevice () {
		var width = viewport().width; //console.log(width);
		if( width > 1200 ){
			return 'wide';
		}else if( width >= 980 ){
			return 'normal';
		/*}else if( width >= 640 && width < 980 ){
			return 'stablet';*/
		}else if(  width >= 768 ){
			return 'tablet';
		}else if(  width > 0 ){
			return 'mobile';
		}
		/*
		Mobile portrait (320x480)
		Mobile landscape (480x320)
		Small tablet portrait (600x800)
		Small tablet landscape (800x600)
		Tablet portrait (768x1024
		Tablet landscape (1024x768)
		*/
	}
	anyFix();
	function anyFix(){
		$('.caroufredsel_wrapper').each(function(){
			if( $(this).find('ul.products-grid:first-child').length ){
				$(this).css('height', $(this).find('.item:first-child').outerHeight());
				$(this).find('ul.products-grid:first-child').css('height', $(this).find('.item:first-child').outerHeight());
			}
			/*else if($(this).find('ul:first-child').length){
				$(this).css('height', $(this).find('li:first-child').outerHeight());
				$(this).find('ul.products-grid:first-child').css('height', $(this).find('li:first-child').outerHeight());
			}*/
		});
	}
	function spotlight(el){
		h = 0;
		i = 0;
		$(el).each(function(){ //console.log($(this).height());
			i++;
			if(i==1){
				h = $(this).height();
			}
			if(h < $(this).height()){
				h = $(this).height();
			}
		});
		$(el).each(function(){
			$(this).css('height', h);
		});
	}
	menuOsetTop = 0;



  	updateBootstrapElementClass (detectDevice());
  	// With window resize
  	$(window).resize(function(){
    	if ($.data(window, 'detect-device-time'))
      		clearTimeout($.data(window, 'detect-device-time'));
    	$.data(window, 'detect-device-time',
      		setTimeout(function(){
        		updateBootstrapElementClass (detectDevice());
        		anyFix();
      		}, 200)
    	);
  	});
});





function setHeight(array, status){
	var h=0;
	if(jQuery.isArray(array)){
		for($i=0;$i<array.length; $i++){
			jQuery(array[$i]).css('height', "");
		}
		for($i=0;$i<array.length; $i++){
			if( (h < jQuery(array[$i]).height()) &&  (jQuery(array[$i]).css('display')!="none")){
				h = jQuery(array[$i]).height();
			}
		}
		if(status){
			for($i=0;$i<array.length; $i++){
				jQuery(array[$i]).css('height', h);
			}
		} else {
			for($i=0;$i<array.length; $i++){
				jQuery(array[$i]).css('height', '');
			}
		}
	} else {
		jQuery(array).each(function(){
			if( (h < jQuery(this).height()) &&  (jQuery(this).css('display')!="none")){
				h = jQuery(this).height();
			}
		});
		if(status) jQuery(array).css('height', h);
		else jQuery(array).css('height', '');
	}
}
// fix dropdown bootstrap on magento
(function() {
    var isBootstrapEvent = false;
    if (window.jQuery) {
        var all = jQuery('*');
        jQuery.each(['hide.bs.dropdown', 
            'hide.bs.collapse', 
            'hide.bs.modal', 
            'hide.bs.tooltip',
            'hide.bs.popover'], function(index, eventName) {
            all.on(eventName, function( event ) {
                isBootstrapEvent = true;
            });
        });
    }
    var originalHide = Element.hide;
    Element.addMethods({
        hide: function(element) {
            if(isBootstrapEvent) {
                isBootstrapEvent = false;
                return element;
            }
            return originalHide(element);
        }
    });
})(jQuery);
// end fix dropdown bootstrap on magento
function dump(obj) {
    var out = '';
    for (var i in obj) {
        out += i + ": " + obj[i] + "\n";
    }
    alert(out);
    var pre = document.createElement('pre');
    pre.innerHTML = out;
    document.body.appendChild(pre)
}
(function($){
	$.fn.SnsAccordion = function(options) {
		var $el    = $(this);
		var defaults = {
			active: 'open',
			el_wrap: 'li',
			el_content: 'ul',
			accordion: true,
			expand: true,
			btn_open: '<i class="fa fa-plus-square-o"></i>',
			btn_close: '<i class="fa fa-minus-square-o"></i>'
		};
		var options = $.extend({}, defaults, options);
		
		
		$(document).ready(function() {
			$el.find(options.el_wrap).each(function(){
				$(this).find('> a, > span, > h4').wrap('<div class="accr_header"></div>');
				if(($(this).find(options.el_content)).length){
					$(this).find('> .accr_header').append('<span class="btn_accor">' + options.btn_open + '</span>');
					$(this).find('> '+options.el_content+':not(".accr_header")').wrap('<div class="accr_content"></div>');
				}
			});
			if(options.accordion){
				$('.accr_content').hide();
				$el.find(options.el_wrap).each(function(){
					if($(this).hasClass(options.active)) {
						$(this).find('> .accr_content')
							   .addClass(options.active)
							   .slideDown();
						$(this).find('> .accr_header')
							   .addClass(options.active);
					}
				});
			} else {
				$el.find(options.el_wrap).each(function(){
					if(!options.expand){
						$('.accr_content').hide();
					} else {
						$(this).find('> .accr_content').addClass(options.active);
						$(this).find('> .accr_header').addClass(options.active);
						$(this).find('> .accr_header .btn_accor').html(options.btn_close);
					}
				});
			}
	   });
	    $(window).load(function() {
			$el.find(options.el_wrap).each(function(){
				var $wrap = $(this);
				var $accrhead = $wrap.find('> .accr_header');
				var btn_accor = '.btn_accor';
				
				$accrhead.find(btn_accor).on('click', function(event) {
					event.preventDefault();
					var obj = $(this);
					var slide = true;
					if($accrhead.hasClass(options.active)) {
						slide = false;
					}
					if(options.accordion){
						$wrap.siblings(options.el_wrap).find('> .accr_content').slideUp().removeClass(options.active);
						$wrap.siblings(options.el_wrap).find('> .accr_header').removeClass(options.active);
						$wrap.siblings(options.el_wrap).find('> .accr_header ' + btn_accor).html(options.btn_open);
					}
					if(slide) {
						$accrhead.addClass(options.active);
						obj.html(options.btn_close);
						$accrhead.siblings('.accr_content').addClass(options.active).stop(true, true).slideDown();
					} else {
						$accrhead.removeClass(options.active);
						obj.html(options.btn_open);
						$accrhead.siblings('.accr_content').removeClass(options.active).stop(true, true).slideUp();
					}
					return false;
				});
			});
		});
	};
})(jQuery);