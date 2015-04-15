<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar']: Items for the sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>

<div class="body">
  
  <header>

    <div class="container">
      <div class="row">     
        <div class="col-xs-12"> 
      
          <div class="header">
            <?php print render($page['header']); ?>
              
            <h1 class="logo">
               <a href="<?php print $front_page; ?>" title="Home"><img src="<?php print $base_path . path_to_theme(); ?>/images/logo.png" alt="Home"></a>
            </h1>

            <div class="header-top pull-right">
          
               <div class="header-top pull-right">
              <?php if ($page['topbar']) { print render($page['topbar']); } ?>
            </div> <!-- .header-top -->


            </div> <!-- .header-top -->

            <?php if ($main_menu) { ?>
              <nav class="hidden-xs" role="navigation">
                <div class="navbar-inner">
                  <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('class' => array('nav nav-pills')))); ?>
                  
                </div>


              </nav>
            <?php } ?>

          </div> <!-- .header -->
 
        </div> <!-- .column -->
      </div> <!-- .row -->
    </div> <!-- .container -->
  </header>

  

  <div class="banner">
    <img src="/sites/all/themes/clresponsive/images/banner.jpg" width="100%" />
    
    <!-- Signup box -->
    <?php if ($page['actionbox']) { ?>
      <div class="hidden-xs actionbox">
        <?php print render($page['actionbox']); ?>
      </div>
    
    <?php } ?>
	</div>
	
	<div class="container">
    <div class="row">
      <div class="col-xs-12">
        <?php print $messages; ?>
      </div>
    </div>
  </div>


<?php if ($page['actionbox']) { ?>
<div class="container visible-xs">
    <?php print render($page['actionbox']); ?>
  </div>
  <hr class="visible-xs">
    <?php } ?>

    <!--<?php if ($secondary_menu) { ?>
                <div class="navigation">
                  <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('secondary-menu', 'links', 'inline', '')))); ?>
                </div>
              <?php } ?> --> 
              
  <div class="container">

    
  
    <?php if ($page['highlighted']) { ?>
      <div class="highlighted">
        <?php print render($page['highlighted']); ?>
      </div>  
    <?php } ?>    
        
    <!-- <?php print $breadcrumb; ?>-->
        
    <div class="row">

      <div class="col-xs-12">
        
        <?php if (isset($node) && $node->type != "pagina" && $node->type != "article" && $node->type != "team_page") { ?>
    
          <?php print render($title_prefix); ?>
            
          <?php if ($title): ?><h2><?php print $title; ?></h2><?php endif; ?>
          <?php print render($title_suffix); ?>
        <?php } ?>


        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

		<?php if ($action_links): ?> <div class="pull-right buttons"> <?php print render($action_links); ?> </div> <?php endif; ?>
  
      </div>
    </div>
    
    </div>     
 
  <div class="container">
    <?php print render($page['help']); ?>
    
    <?php print render($page['content']); ?>
  </div>

 <?php if (isset($page['secondary'])) { ?>
	  <hr>
	  <div class="container">
		<div class="row">
		  <div class="col-xs-12">
			<?php print render($page['secondary']); ?>
			<?php // print $feed_icons; ?>
		  </div>
		</div>
	  </div>
  <?php } ?>
  
  <br><br>

  <footer>
    <div class="container">
    <div class="row">
          
        <div class="col-sm-6" >
          <h2>Supporters</h2>
          <p>Cityleague wordt ondersteund door de <a href="http://www.frisbeesport.nl">Nederlandse Frisbee Bond</a>.</p>
        </div>

        <div class="col-xs-6 hidden-xs">
          <img class="whitelogo pull-right" src="<?php print $base_path . path_to_theme(); ?>/images/lotto.jpg">
          <a href="http://www.frisbeewinkel.nl" ><img class="whitelogo pull-right" style="margin-right: 15px; margin-left: 15px;" src="<?php print $base_path . path_to_theme(); ?>/images/frisbeewinkel.jpg"></a>
          <div class="clearfix"></div>
        </div>

         <div class="col-sm-6 visible-xs">
          <a href="http://www.frisbeewinkel.nl"><img class="whitelogo pull-left" style="margin-right: 15px;" src="<?php print $base_path . path_to_theme(); ?>/images/frisbeewinkel.jpg"></a>
          <img class="whitelogo pull-left" src="<?php print $base_path . path_to_theme(); ?>/images/lotto.jpg">
          <div class="clearfix"></div>
        </div>   

      </div> <!-- .row -->

      <hr>

      <div class="row">
        <div class="col-xs-12">
        
          <ul class="social-icons pull-right">
            <li class="facebook hastooltip"><a href="https://www.facebook.com/pages/Cityleague/343908799043411" target="_blank" data-placement="bottom" title="Facebook" data-original-title="Facebook">Facebook</a></li>
          </ul>
          Ontwikkeling: Ron van Kesteren.<br>
        </div>
      </div> <!-- .row -->

    </div> <!-- .container -->
  </footer>
        
  <!-- Bootstrap core JavaScript
  ================================================== -->
  <!-- Placed at the end of the document so the pages load faster -->
  <script src="/sites/all/themes/clresponsive/bootstrap/js/bootstrap.min.js"></script>

  <script type="text/javascript">
    $(function () {
        $('.hastooltip').tooltip();
    });

    $(function() {
      // Setup drop down menu
      $('.dropdown-toggle').dropdown();
     
      // Fix input element click problem
      $('#dropdown-login input, #dropdown-login label').click(function(e) {
        e.stopPropagation();
      });
    });
  </script>

</div> <!-- .body -->
