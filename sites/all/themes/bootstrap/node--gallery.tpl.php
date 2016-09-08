<?php
// $Id: node.tpl.php,v 1.8 2010/12/18 12:23:54 jmburnz Exp $

/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: whether submission information should be displayed.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
?>
<?php
  //drupal_add_library('jquery_plugin', 'cycle');
  drupal_add_js(drupal_get_path('module', 'druprong').'/jquery.cycle2.js');

  drupal_session_start();
  $_SESSION['nid'] = $node->nid;
 
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div class="node-inner clearfix">

    <?php print render($title_prefix); ?>
    <?php if ($teaser): ?>
      <h2 class="node-title"<?php print $title_attributes; ?>>
        <a href="<?php print $node_url; ?>" rel="bookmark"><?php print $title; ?></a>
      </h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php print $user_picture; ?>

    <?php if ($display_submitted): ?>
      <div class="node-submitted">
        <?php print $submitted; ?>
      </div>
    <?php endif; ?>

    <div class="node-content"<?php print $content_attributes; ?>>
     <table style="margin: auto; text-align: center;"><tr><td style="padding-top: 80px;">
     <span id="loading" style="display:block">Loading...</span>
     <div class="gallery-wrapper">
       <div style="position: relative; max-width:800px;overflow: hidden; display:none" class="slideshow"
        data-cycle-fx=fade
        data-cycle-timeout=0
        data-cycle-prev="#prev"
        data-cycle-next="#next"
        data-cycle-speed=10
        data-cycle-caption-template="{{slideNum2}} | {{slideCount2}}"
        data-cycle-caption=".caption2">
	      <?php
		foreach($content['field_gallery']['#items'] as $image) {
		  $path = file_create_url($image['uri']);
		?>
			  <img class="large-image" title="<?php print $node->title;?>" src="<?php print $path ?>" style="width:800px; cursor: pointer">
		<?php
		}
		?>
        </div>
	       <?php
		// Hide comments and links and render them later.
		hide($content['comments']);
		hide($content['links']);
		//print render($content);
	      ?>
      <!-- prev/next links -->
        <div class="control-gellary" style="padding-top: 8px; font-size: .8em;display: none;">
            <span id="prev" style="visibility: hidden">prev </span>
            <span class="center caption2" style="padding: 0px 10px"></span>
            <span id="next"> next</span>
            <div style="margin-top: 5px" class="home"><?php print l('back', '<front>'); ?></div>
        </div>

    </div>
    </td></tr></table>
    <?php if ($content['links']): ?>
      <div class="node-links">
        <?php print render($content['links']); ?>
      </div>
    <?php endif; ?>

    <?php print render($content['comments']); ?>

  </div>
</div>

<script>
     $ = jQuery;
     $(document).ready(function () {
        $( '.slideshow' ).on( 'cycle-after', function(event, optionHash, outgoingSlideEl, incomingSlideEl, forwardFlag) {
    		  $('#img_run_number').html(parseInt(optionHash.currSlide)+parseInt(1)+'|'+optionHash.slideCount);
    		  if ((optionHash.slideCount) == optionHash.slideNum) {
    		    $('#next').css({'visibility': 'hidden'});
    		  }
    		  else {
    		    $('#next').css({'visibility': ''});
    		  }
    		  
    		  if (optionHash.slideNum == 1) {
    		    $('#prev').css({'visibility': 'hidden'});
    		  }
    		  else {
    		    $('#prev').css({'visibility': ''});
    		  }
              // argument opts is the slideshow's option hash
        });

        $('.slideshow').cycle({
          fx: 'fade'
        });

        $('.large-image').click(function () {

          $('#next').trigger('click');
        });

        $(window).load(function() {
          $('#loading').hide();
          $('.slideshow').show();
          $('.control-gellary').show();
        });

        $(document).keydown(function(e) {
          switch(e.which) {
          case 37: // left
            $('#prev').trigger('click');
          break;

          case 38: // up
            $('.home').find('a').trigger('click'); 
          break;

          case 39: // right
            $('#next').trigger('click');
          break;

          case 40: // down
          break;

          default: return; // exit this handler for other keys
          }
        e.preventDefault(); // prevent the default action (scroll / move caret)
    });

     });
</script>
