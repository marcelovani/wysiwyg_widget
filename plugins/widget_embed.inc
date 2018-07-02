<?php
/**
 * @file
 * Provides a wysiwyg plugin.
 */

/**
 * Specially named Implements hook_wysiwyg_plugin().
 *
 * Should be named {$module}_{$plugin}_plugin().
 */
function wysiwyg_widget_widget_embed_plugin() {
  $plugins['widget_embed'] = array(
    'title' => t('Widget embed'),
    'icon file' => 'widget_embed.png',
    'icon title' => t('Embed a widget'),
    // CSS doesn't load /grrrrrr/.
    'css path' => drupal_get_path('module', 'wysiwyg_widget'),
    'css file' => '/plugins/widget_embed/widget_embed.css',

    'settings'   => array(
      'form_markup' => '<div class="widget-embed-popup" style="background-color: #ffffff; z-index: 200; border: 2px solid #999999; padding: 1em;">
<form id="widget-embed-form">
<div class="form-item form-type-textarea">
    <label for="edit-widget-embed-body">Embed Code</label>
    <div class="form-textarea-wrapper resizable">
        <textarea id="edit-widget-embed-body" class="form-textarea" rows="15" cols="60" name="widget_embed-body"></textarea>
    </div>
</div>
<input id="edit-widget-embed-insert" class="form-submit" type="submit" value="Insert" name="op" onclick="return (false);"></input>
<input id="edit-widget-embed-cancel" class="button-no form-submit" type="submit" value="Cancel" name="op" onclick="return (false);"></input>
</form>
</div>',
    'icon_markup' => '',
    'placeholders' => array(),
    // By default let ANY code be put into a widget.
    // To restrict to predifined widgets, set this to true
    // and create widget placeholders for allowed widgets in hook_wysiwyg_widget_embed_alter().
    'placeholders_only' => variable_get('wysiwyg_widget_widget_embed_placeholders_only', FALSE),
  ),
  );

  $placeholders = &$plugins['widget_embed']['settings']['placeholders'];
  foreach (wysiwyg_widget_get_all_placeholders() as $name => $item) {
    $machine_name = _wysiwyg_widget_machine_name($name);

    $placeholder = array();

    $placeholder['regex'] = array(
      'pattern' => $item->pattern,
      'flags' => $item->flags,
    );

    $placeholder['icon_markup'] = sprintf('<img class="%s" title="%s" src="%s" />',
      'wysiwyg-widget-embed-img',
      $name,
      file_create_url(_wysiwyg_widget_generate_image($name))
    );

    $placeholders[$machine_name] = $placeholder;
  }

  // Allow module to set placeholders for widgets.
  drupal_alter('wysiwyg_widget_embed', $placeholders);

  return $plugins;
}
