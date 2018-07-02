<?php

/**
 * @file
 * A Ctools Export UI plugin for Wysiwyg Widget.
 */
$plugin = array(
  'schema' => 'wysiwyg_widget_sets',
  // As defined in hook_schema().
  'access' => 'administer site configuration',
  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu item' => 'wysiwyg_widget',
    'menu title' => 'Wysiwyg Widget',
    'menu description' => 'Administer Wysiwyg Widget sets.',
  ),
  // Define user interface texts.
  'title singular' => t('set'),
  'title plural' => t('sets'),
  'title singular proper' => t('Wysiwyg Widget set'),
  'title plural proper' => t('Wysiwyg Widget sets'),
  // Define the class to use as a handler for DFP ad tags.
  'handler' => array(
    'class' => 'wysiwyg_widget_set_ui',
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
  $item = $form_state['item'];

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
      '#description' => t('Only use letters, numbers and underscores.'),
      '#machine_name' => array(
        'exists' => 'wysiwyg_widget_set_name_exists',
        'source' => array('general', 'name'),
      ),
      '#weight' => -1,
    ) + $form['info']['machinename'];
  unset($form['info']);

  $form['embed_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Embed code'),
    '#description' => '',
    '#rows' => 15,
    '#default_value' => $item->embed_code,
  );
}

// Theme function for wysiwyg_widget_manage_services().
function theme_wysiwyg_widget_manage_services($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form) as $service_identifier) {
    $form[$service_identifier]['weight']['#attributes']['class'] = array('services-order-weight');
    $rows[] = array(
      'data' => array(
        array('class' => array('service-cross')),
        drupal_render($form[$service_identifier]['service']),
        drupal_render($form[$service_identifier]['enabled']),
        drupal_render($form[$service_identifier]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array('', t('Service'), t('Enabled'), t('Weight'));
  $output = drupal_render($form['note']);
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'service-order')
  ));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('service-order', 'order', 'sibling', 'services-order-weight');

  return $output;
}

/**
 * Check if the given machinename is unique in the dfp_tags table.
 */
function wysiwyg_widget_set_name_exists($machinename) {
  $select = db_select('wysiwyg_widget_sets', 's');
  $select->addExpression('COUNT(*)');
  $select->condition('s.machinename', $machinename);

  return $select->execute()->fetchField();
}
