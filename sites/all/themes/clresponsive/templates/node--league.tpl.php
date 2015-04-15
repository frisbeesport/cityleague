<?php

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
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
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

<?php if ($teaser) {  ?>

  <div class="smalltile">
    <div style="overflow:hidden" class="smalltile-image">
      <a href="<?php print $node_url; ?>">
        <?php 
          if (isset($content['field_tile_image'])) {
            $content['field_tile_image'][0]['#item']['attributes']['class'][] = "hover-image";
            print render($content['field_tile_image']);
          }
        ?>
      </a>  
    </div>
    <div class="smalltile-data">
      <a href="<?php print $node_url; ?>"><div class="smalltile-title"><?php print render($content['field_city']); ?></div></a>
      <div><strong><?php print render($content['field_dates']); ?></strong></div>
      <div><?php print render($content['field_pitch']); ?></div>
    </div>
  </div>

<?php } else { ?>

<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <div class="row">
    <?php if (isset($content['field_league_image'])) { ?>

    <div class="col-sm-8 col-sm-push-4">
      <?php print render($title_prefix); ?>
      <?php if (!$page): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

       <?php if (isset($content['field_lead_text'])) { ?>
          <p class="leadtext" > <?php print $content['field_lead_text'][0]['#markup']; ?> <p>
        <?php } ?>

        <?php print render($content['body']); ?>
    </div>
    <div class="col-sm-4 col-sm-pull-8 hidden-xs">
      <?php $content['field_league_image'][0]['#item']['attributes']['class'][] = 'league-image'; print render($content['field_league_image']); ?>
    </div>

    <?php }  else { ?>
      <div class="col-sm-12">
        <?php if (isset($content['field_lead_text'])) { ?>
          <p class="leadtext" > <?php print $content['field_lead_text'][0]['#markup']; ?> <p>
        <?php } ?>

        <?php print render($content['body']); ?>
      </div>
    <?php } ?>
  </div>
  
    <?php 
      $cols = 0;
      if (isset($content['field_about_us'])) $cols++;
      if (isset($content['field_play_on'])) $cols++;
      if (isset($content['field_sponsors'])) $cols++;

      if ($cols != 0) $colwidth = 12 / $cols;
    ?>

  <?php if ($cols != 0) { ?>
  
  <hr>

  <div class="row">
    


    <?php if (isset($content['field_about_us'])) { ?>
      <div class="col-sm-<?php print $colwidth; ?>">
        <h3><?php print t("About us"); ?></h3>
         <?php print render($content['field_about_us']); ?>
      </div>
    <?php } ?>

    <?php if (isset($content['field_play_on'])) { ?>
      <div class="col-sm-<?php print $colwidth; ?>">
        <h3><?php print t("Play on!"); ?></h3>
        <?php print render($content['field_play_on']); ?>
      </div>
    <?php } ?>

    <?php if (isset($content['field_sponsors'])) { ?>
      <div class="col-sm-<?php print $colwidth; ?>">
        <h3><?php print t("Partners"); ?></h3>
        <?php print render($content['field_sponsors']); ?>
      </div>  
    <?php } ?>

  </div>

  <?php } ?>
 
  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?> <!-- moved to outside of the node div -->

</div>

<?php } ?>