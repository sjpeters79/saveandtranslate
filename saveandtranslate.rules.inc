<?php
/**
 * @file
 * Code for the Save and Translate Rules
 */

/*
 * Implementation of hook_condition_info()
 */
function saveandtranslate_rules_condition_info() {
  return array(
    'saveandtranslate_is_it_translated' => array(
      'label' => t('Is the node translated?'),
      'parameter' => array(
        'node' => array(
          'label' => t('Node that was updated'),
          'type' => 'node',
        ),
      ),
      'group' => t('Coldfront Labs Inc'),
    ),
  );
}


function saveandtranslate_is_it_translated($node) {
  //check to make sure it's translated and was just created ie does not have a changed date set
  $translations = $node->translations->data;
  unset($translations[$node->translations->original]);
  if (sizeof($translations) > 0) {
    $translated_language = array_keys($translations);
    if (!isset($node->translations->data[$translated_language[0]]['changed'])) {
      return TRUE;
    }
  }
  else {
    return FALSE;
  }
  return FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function saveandtranslate_rules_action_info() {
  return array(
    'saveandtranslate_add_translation_url_' => array(
      'label' => t('Get the path to add a translation'),
      'group' => t('Colfront Labs Inc'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node to be translated'),
          'description' => t('The node to be translated'),
        ),
      ),
      'provides' => array(
        'translation_path' => array(
          'type' => 'uri',
          'label' => t('Translation Add Path'),
        ),
      ),
    ),
    'saveandtranslate_redirect_preview_' => array(
      'label' => t('Get the path to add to preview the translated node'),
      'group' => t('Colfront Labs Inc'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node to be previewed'),
          'description' => t('The node to be previewed'),
        ),
      ),
      'provides' => array(
        'preview_path' => array(
          'type' => 'uri',
          'label' => t('Preview Path'),
        ),
      ),
    ),
    'saveandtranslate_set_as_translated_' => array(
      'label' => t('Set the translated item as translated'),
      'group' => t('Colfront Labs Inc'),
      'parameter' => array(
        'node' => array(
          'type' => 'text',
          'label' => t('Item Id'),
          'description' => t('Item Id of the node that was translated'),
        ),
      ),
      'provides' => array(
        'translation_title' => array(
          'type' => 'text',
          'label' => t('Translated item title'),
        ),
      ),
    ),
    'saveandtranslate_update_translation_status_' => array(
      'label' => t('Set the translation status to be the same as the event status'),
      'group' => t('Colfront Labs Inc'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Item Id'),
          'description' => t('Item Id of the node to update'),
        ),
      ),
      'provides' => array(
        'updated_event_title' => array(
          'type' => 'text',
          'label' => t('Updated Event Title'),
        ),
      ),
    ),
  );
}

/**
 * Custom rule to set a translated item as translated
 * @param $itemId
 */
function saveandtranslate_set_as_translated_($itemId) {

  //load the job
  if ($job = tmgmt_job_item_load($itemId)) {
    if (isset($job->item_id)) {
      $node = node_load($job->item_id);
      $wrapper = entity_metadata_wrapper('node', $node);
      $wrapper->save();
    }
  }
  return $itemId;
}

/**
 * Custom rule action callback to get the add translation path
 */
function saveandtranslate_add_translation_url_($node) {
  if (isset($node->translations->original)) {
    $http = isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' ? 'https' : 'http';
    $server_name = $_SERVER['SERVER_NAME'];
    if ('en' == $node->translations->original) {
      return array('translation_path' => url($http . '://' . $server_name . ':8080/fr/node/' . $node->nid . '/edit/add/en/fr'));
    }
    else {
      return array('translation_path' => url($http . '://' . $server_name . ':8080/en/node/' . $node->nid . '/edit/add/fr/en'));
    }
  }
}

/**
 * Custom rule action callback to get the add translation path
 */
function saveandtranslate_redirect_preview_($node) {
  if (isset($node->translations->original)) {
    $http = isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' ? 'https' : 'http';
    $server_name = $_SERVER['SERVER_NAME'];
    $lang = $node->translations->original;
    return array('preview_path' => url($http . '://' . $server_name . ':8080/' . $lang . '/node/' . $node->nid));
  }
}

function saveandtranslate_update_translation_status_($node) {
  db_update('entity_translation')
    ->fields(array('status' => $node->status))
    ->condition('entity_id', $node->nid)
    ->condition('entity_type', 'node')
    ->execute();
  db_update('entity_translation_revision')
    ->fields(array('status' => $node->status))
    ->condition('entity_id', $node->nid)
    ->condition('entity_type', 'node')
    ->execute();
  return array('updated_event_title' => $node->title);
}
