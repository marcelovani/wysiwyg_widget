<?php

/**
 * @file
 * A Ctools Export UI plugin for Wysiwyg Widget.
 */
$plugin = array(
  'schema' => 'wysiwyg_widget',
  // As defined in hook_schema().
  'access' => 'administer site configuration',
  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu item' => 'wysiwyg_widget',
    'menu title' => 'Wysiwyg Widget',
    'menu description' => 'Manage Wysiwyg Widget Embeds.',
  ),
  // Define user interface texts.
  'title singular' => t('placeholder'),
  'title plural' => t('placeholders'),
  'title singular proper' => t('Wysiwyg Widget placeholder'),
  'title plural proper' => t('Wysiwyg Widget placeholders'),
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
  $form['pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern'),
    '#description' => 'The pattern to match this widget embed',
    '#rows' => 15,
    '#default_value' => $form_state['item']->pattern,
  );
  $form['flags'] = array(
    '#type' => 'textfield',
    '#title' => t('Flags'),
    '#size' => 5,
    '#description' => 'Regex flags. i.e. gi',
    '#rows' => 15,
    '#default_value' => $form_state['item']->flags,
  );
}
