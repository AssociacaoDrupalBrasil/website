(function($) {
Drupal.behaviors.adminimal_module_filter_box = {
  attach: function (context, settings) {
    //code starts
	  $('input#edit-module-filter-name').each(function(){

	    var defaultVal = Drupal.t('Search');
	    
	    $(this).focus(function(){
	      if ($(this).val() == defaultVal){
	        $(this).val('');
	      }
	      $(this).removeClass('empty');
	    })

	    .blur(function(){
	      if ($(this).val() == ''){
	        $(this).addClass('empty').val(defaultVal);
	      }
	    })

	    .blur().addClass('empty');

	  });
    //code ends
  }
};
})(jQuery);

(function($) {
Drupal.behaviors.infiniteScrollAddClass = {
  attach: function (context, settings) {
    $('#console .messages.status').has("div.krumo-root").removeClass().addClass( "krumo-wrapper" );
  }
};
})(jQuery);
