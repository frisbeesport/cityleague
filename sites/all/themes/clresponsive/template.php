<?php

/**
 * @file
 * Contains theme override functions and preprocess functions
 */

include_once dirname(__FILE__) . '/theme/system/button.func.php';
include_once dirname(__FILE__) . '/theme/system/button.vars.php';
include_once dirname(__FILE__) . '/theme/system/status-messages.func.php';

include_once dirname(__FILE__) . '/theme/system/form-element.func.php';
include_once dirname(__FILE__) . '/theme/system/form-element-label.func.php';
include_once dirname(__FILE__) . '/theme/system/date.func.php';

function clresponsive_preprocess_image(&$variables) {
  $attributes = &$variables['attributes'];

  foreach (array('width', 'height') as $key) {
    unset($attributes[$key]);
    unset($variables[$key]);
  }
}

function clresponsive_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<ul class="nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

function clresponsive_menu_local_action($variables) {
  $link = $variables['element']['#link'];
  return '<a href="' . url($link['href']) . '" class="btn btn-default">' . $link['title'] . '</a>';  
}


function clresponsive_preprocess_node(&$variables) {
  global $language;

  $node = $variables['elements']['#node'];

  if ($node->type == 'article')
  {
    $variables['date'] = format_date($node->created, 'custom', 'l, F j, Y');

    $author = '';
    $values = field_get_items('node', $node, 'field_authors');

    if ($values != FALSE) {
      for ($i = 0; $i < count($values); $i++)
      {
        if ($i > 0 && $i < count($values) - 1)
        {
          $author = $author . ', ';     
        } 
        else if ($i > 0 && $i == count($values) - 1)
        {
          if ($language->language == 'nl')
          {
            $author = $author . ' en ';     
          } 
          else
          {
            $author = $author . ', and ';     
          }
        }
        $author = $author . $values[$i]['entity']->title; 
      }  
    }

    $variables['author'] = $author;
  }
}  
  

/**
 * Implements hook_preprocess_html().
 */
function clresponsive_preprocess_html(&$variables) {

  // Add conditional stylesheet for IE
  drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => ' IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));

  $options = array(
    'type' => 'file',
    'group' => CSS_THEME,
    'weight' => 10,
  );

  // Add optional stylesheets
  $options = array(
    'type' => 'inline',
    'group' => CSS_THEME,
    'weight' => 10,
  );
}