$ = jQuery;
$(document).ready(function () {


  $('#block-views-menu-block .item-list').find('ul').find('a').hover(function () {

    $('#nid').html($(this).attr('nid'));
  });


  $('#block-views-menu-block .item-list').find('ul').hide();
  $('#block-views-menu-block h3').click(function () {

    var display = $(this).siblings('ul').css('display');
    $('#block-views-menu-block .item-list').find('ul').hide();
    if (display == 'none') {
      $(this).siblings('ul').show();
    }
  });

  //$('#block-views-menu-block').find('.more-menu').find('ul').hide();
  $('#block-views-menu-block').find('.more-menu').find('.about').click(function () {

    var display = $(this).find('ul').css('display');
    console.log(display);
    $(this).find('ul').hide();
    if (display == 'none') {
      $(this).find('ul').show();
    }
  });

  /*
  $('#block-views-menu-block .item-list').find('ul').find('a').tooltipster({
    arrow: false,
    position: 'bottom',
    offsetY: -10,
    theme: 'tooltipster-shadow',
  });
  */
  //var nid = getParameterByName('nid');
  var nid = $('#nid').html();
  var active_node = $('#block-views-menu-block .item-list').find('ul').find('a.'+nid);
  var parent_node = active_node.parents('.item-list').find('h3')
  parent_node.trigger('click');
  active_node.addClass('active');
  function getParameterByName(name) {
    name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);
    return results === null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
}
});

window.onbeforeunload = function() {
     window.name = "reloader"; 
     console.log('ddddddddd');
        }
