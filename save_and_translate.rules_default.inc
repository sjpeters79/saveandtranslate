<?php
/**
 * @file
 * save_and_translate.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function save_and_translate_default_rules_configuration() {
  $items = array();
  $items['save_and_translate_save_and_translate'] = entity_import('rules_config', '{ "save_and_translate_save_and_translate" : {
      "LABEL" : "Save and Translate",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "save_and_translate", "rules_i18n" ],
      "ON" : { "node_insert--event" : { "bundle" : "event" } },
      "IF" : [
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "2" : "2" } } } },
        { "NOT text_matches" : { "text" : [ "node:title" ], "match" : "Copy of", "operation" : "starts" } },
        { "NOT text_matches" : {
            "text" : [ "node:title" ],
            "match" : "La copie du",
            "operation" : "starts"
          }
        }
      ],
      "DO" : [
        { "save_and_translate_add_translation_url_" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "translation_path" : { "translation_path" : "Translation Add Path" } }
          }
        },
        { "redirect" : { "url" : [ "translation-path" ] } },
        { "drupal_message" : {
            "message" : "You may provide a translation to your content below. If you do not have a translation for your content, you may browse away from this page safely.",
            "repeat" : "0"
          }
        },
        { "drupal_message" : { "message" : "Event submitted and translated. Thanks" } },
      ]
    }
  }');
  $items['save_and_translate_save_and_translate_redirect_preview'] = entity_import('rules_config', '{ "save_and_translate_save_and_translate_redirect_preview" : {
      "LABEL" : "Translate and Redirect to Original language",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "save_and_translate", "rules" ],
      "ON" : { "node_update--event" : { "bundle" : "event" } },
      "IF" : [ { "save_and_translate_is_it_translated" : { "node" : [ "node" ] } } ],
      "DO" : [
        { "save_and_translate_redirect_preview_" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "preview_path" : { "preview_path" : "Preview Path" } }
          }
        },
        { "redirect" : { "url" : "[preview-path:value]" } }
      ]
    }
  }');

  return $items;
}
