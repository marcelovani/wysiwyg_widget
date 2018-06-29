<?php

/**
 * @file
 * A Ctools Export UI plugin for Wysiwyg Widget.
 */
$plugin = array(
  'schema' => 'wysiwyg_widget', // As defined in hook_schema().
  'access' => 'administer site configuration', // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'wysiwyg_widget',
    'menu title' => 'Wysiwyg Widget',
    'menu description' => 'Administer Wysiwyg Widgets.',
  ),
  // Define user interface texts.
  'title singular' => t('set'),
  'title plural' => t('sets'),
  'title singular proper' => t('Wysiwyg Widget'),
  'title plural proper' => t('Wysiwyg Widgets'),
  // Define the class to use as a handler for DFP ad tags.
  'handler' => array(
     'class' => 'wysiwyg_widget_ui',
     'parent' => 'ctools_export_ui',
  ),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'wysiwyg_widget_ctools_export_ui_form',
  ),
);

/**
 * Define the preset add/edit form.
 */
function wysiwyg_widget_ctools_export_ui_form(&$form, &$form_state) {
  $item = ($form_state['op'] == 'add' ? $form_state['item'] : $form_state['item']->raw);

  // Generate the list of available widgets.
  module_load_include('inc', 'wysiwyg_widget', 'plugins/widget_embed');
  $widgets = wysiwyg_widget_widget_embed_plugin();

  // Use our dragable table theme function.
  $form['available_widgets']['widgets'] = array(
    '#theme' => 'wysiwyg_widget_manage_widgets',
    '#tree' => TRUE,
  );

  foreach ($widgets as $widget_name => $value) {

    $form['available_widgets']['widgets'][$widget_name]['name'] = array(
      '#markup' => $widget_name,
    );

    // Store the buttons markup along other data.
    $form['available_widgets']['widgets'][$widget_name]['markup'] = array(
      '#type' => 'textbox',
      '#value' => $value,
    );
  }

  // Sort the widgets.
  uasort($form['available_widgets']['widgets'], 'drupal_sort_weight');

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#default_value' => $item->name,
    '#weight' => -2,
  );
  $form['machinename'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#maxlength' => 64,
    '#default_value' => $item->machinename,
    '#description' => t('Only use letters, numbers and underscores. Example: article_top'),
    '#machine_name' => array(
      'exists' => 'wysiwyg_widget_name_exists',
      'source' => array('general', 'name'),
    ),
    '#weight' => -1,
  ) + $form['info']['machinename'];
  unset($form['info']);

  // Custom widgets.
  foreach ($widgets as $widget_name => $value) {
    $var_name = 'wysiwyg_widget_custom_' . $widget_name;

    $form[$var_name] = array(
      '#type' => 'textarea',
      '#title' => t('Custom code for %name', array('%name' => $widget_name)),
      '#description' => '',
      '#default_value' => $value,
    );
  }
}

function wysiwyg_widget_ctools_export_ui_form_submit(&$form, &$form_state) {
  $widgets_values = &$form_state['values'];
  uasort($widgets_values, 'drupal_sort_weight');

  foreach ($widgets_values as $key => &$value) {
    $custom_key = 'wysiwyg_widget_custom_' . $key;

    if (empty($form_state['values'][$custom_key])) {
      continue;
    }

    $value['markup'] = $form_state['values'][$custom_key];
  }
}

//@todo rename or remove this
// Theme function for wysiwyg_widget_manage_widgets().
function theme_wysiwyg_widget_manage_widgets($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form) as $value) {
    $form[$value]['weight']['#attributes']['class'] = array('widgets-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form[$value]['service']),
        drupal_render($form[$value]['enabled']),
        drupal_render($form[$value]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array('', t('Name'), t('Enabled'), t('Weight'));
  $output = drupal_render($form['note']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'service-order')));
  $output .= drupal_render_children($form);

  return $output;
}

//@todo rename this
/**
 * Check if the given machinename is unique in the dfp_tags table.
 */
function wysiwyg_widget_name_exists($machinename) {
  $select = db_select('wysiwyg_widget', 's');
  $select->addExpression('COUNT(*)');
  $select->condition('s.machinename', $machinename);

  return $select->execute()->fetchField();
}
