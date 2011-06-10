<?php
// $Id: brightcove3.inc,v 1.1.2.5 2009/07/31 19:48:54 aaron Exp $

/**
 * @file
 *  This is an brightcove3 provider include file for Embedded Media Video.
 *  Use this as a base for creating new provider files.
 *
 *  When using this, first make the following global replacements:
 *    * Replace BRIGHTCOVE3 with the name of your provider in all caps.
 *    * Replace brightcove3 with the name of your provider in all lower case.
 *    * Replace Brightcove3 with the name (to be translated) of your provider in
 *        uppercase.
 *
 *  You then need to go through each function and modify according to the
 *  requirements of your provider's API.
 */

/**
 *  This is the main URL for your provider.
 */
define('EMVIDEO_BRIGHTCOVE3_MAIN_URL', 'http://www.brightcove3.com/');

/**
 *  This is the URL to the API of your provider, if this exists.
 */
define('EMVIDEO_BRIGHTCOVE3_API_URL', 'http://www.brightcove3.com/api');

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_brightcove3_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_BRIGHTCOVE3_DATA_VERSION', 1);

/**
 * hook emvideo_PROVIDER_info
 * This returns information relevant to a specific 3rd party video provider.
 *
 * @return
 *   A keyed array of strings requested by various admin and other forms.
 *    'provider' => The machine name of the provider. This must be the same as
 *      the base name of this filename, before the .inc extension.
 *    'name' => The translated name of the provider.
 *    'url' => The url to the main page for the provider.
 *    'settings_description' => A description of the provider that will be
 *      posted in the admin settings form.
 *    'supported_features' => An array of rows describing the state of certain
 *      supported features by the provider. These will be rendered in a table,
 *      with the columns being 'Feature', 'Supported', 'Notes'. In general,
 *      the 'Feature' column will give the name of the feature, 'Supported'
 *      will be Yes or No, and 'Notes' will give an optional description or
 *      caveats to the feature.
 */
function emvideo_brightcove3_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('')),
  );
  return array(
    'provider' => 'brightcove3',
    'name' => t('Brightcove3'),
    'url' => EMVIDEO_BRIGHTCOVE3_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !brightcove3. You can also read more about its !api.', array('!brightcove3' => l(t('Brightcove3.com'), EMVIDEO_BRIGHTCOVE3_MAIN_URL), '!api' => l(t("developer's API"), EMVIDEO_BRIGHTCOVE3_API_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emvideo_PROVIDER_settings
 *  This should return a subform to be added to the emvideo_settings() admin
 *  settings page.
 *
 *  Note that a form field set will already be provided at $form['brightcove3'],
 *  so if you want specific provider settings within that field set, you should
 *  add the elements to that form array element.
 */
function emvideo_brightcove3_settings() {
  $form = array();
  $form['brightcove3']['brightcove3_publisher_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher ID'),
    '#description' => t('Your BrightCove publisher ID. You can find it in your video URLs'),
    '#default_value' => variable_get('brightcove3_publisher_id', ''),
    '#size' => 13,
    '#maxlength' => 13,
  );
  $form['brightcove3']['brightcove3_player_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Player ID'),
    '#description' => t('Your BrightCove player ID. You can find it in your video URLs'),
    '#default_value' => variable_get('brightcove3_player_id', ''),
    '#size' => 13,
    '#maxlength' => 13,
  );
  $form['brightcove3']['emvideo_brightcove3_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#description' => t('Your Read Token, for retrieving thumbnails.'),
    '#default_value' => variable_get('emvideo_brightcove3_token', ''),
  );
  return $form;
}

/**
 *  hook emvideo_PROVIDER_extract
 *
 *  This is called to extract the video code from a pasted URL or embed code.
 *
 *  We'll be passed a URL or the embed code from a video when an editor pastes
 *  that in the field's textfield. We'll need to either pass back an array of
 *  regex expressions to match, or do the matching ourselves and return the
 *  resulting video code.
 *
 *  @param $parse
 *    An optional string with the pasted URL or embed code.
 *  @return
 *    Either an array of regex expressions to be tested, or a string with the
 *    video code to be used. If the hook tests the code itself, it should
 *    return either the string of the video code (if matched), or an empty
 *    array. Otherwise, the calling function will handle testing the embed code
 *    against each regex string in the returned array.
 */
function emvideo_brightcove3_extract($parse = '') {
  // Here we assume that a URL will be passed in the form of
  // http://www.brightcove3.com/video/text-video-title
  // or embed code in the form of <object value="http://www.brightcove3.com/embed...".

  // We'll simply return an array of regular expressions for Embedded Media
  // Field to handle for us.
  return array(
    // In this expression, we're looking first for text matching the expression
    // between the @ signs. The 'i' at the end means we don't care about the
    // case. Thus, if someone enters http://www.Brightcove3.com, it will still
    // match. We escape periods as \., as otherwise they match any character.
    // The text in parentheses () will be returned as the provider video code,
    // if there's a match for the entire expression. In this particular case,
    // ([^?]+) means to match one more more characters (+) that are not a
    // question mark ([^\?]), which would denote a query in the URL.

    // Brightcove v3.3 - URL style.
    '@\.brightcove\.com/services/player/bcpid.*?\?bctid=(\d+)@i',

    // Now we test for embedded video code, which is similar in this case to
    // the above expression, except that we can safely assume we won't have a
    // query in the URL, and that the URL will be surrounded by quotation marks,
    // and have /embed/ rather than /video/ in the URL. Note that regular
    // expressions will be tested for matches in the order provided, so you
    // may need to move this value above the other in some cases. Obviously,
    // in the case of this brightcove3 provider, you could easily improve the
    // regular expression to match against either a URL or embed code with
    // one expression, such as '@brightcove3\.com/[watch|embed]/([^"\?]+)@i'.
    // However, many other providers have more complex requirements, so
    // we split them up for this demonstration.


    // Older blog style.
    '@\.brightcove\.com/services/viewer/.*?videoId=(\d+)@i',
    // Brightcove v3.3 - Blog style.
    '@\.brightcove\.com/services/viewer/.*?videoPlayer=(\d+)@i',
    // Brightcove v3.3 - Website style.
    '@id="myExperience(\d+)" class="BrightcoveExperience"@i',
  );
}

//<object id="flashObj" width="486" height="412" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,47,0"><param name="movie" value="http://c.brightcove.com/services/viewer/federated_f9/31189372001?isVid=1&publisherID=59121" /><param name="bgcolor" value="#FFFFFF" /><param name="flashVars" value="@videoPlayer=13406455&playerID=31189372001&domain=embed&" /><param name="base" value="http://admin.brightcove.com" /><param name="seamlesstabbing" value="false" /><param name="allowFullScreen" value="true" /><param name="swLiveConnect" value="true" /><param name="allowScriptAccess" value="always" /><embed src="http://c.brightcove.com/services/viewer/federated_f9/31189372001?isVid=1&publisherID=59121" bgcolor="#FFFFFF" flashVars="@videoPlayer=13406455&playerID=31189372001&domain=embed&" base="http://admin.brightcove.com" name="flashObj" width="486" height="412" seamlesstabbing="false" type="application/x-shockwave-flash" allowFullScreen="true" swLiveConnect="true" allowScriptAccess="always" pluginspage="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></embed></object>


/**
 *  hook emvideo_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the brightcove3 provider.
 */
function emvideo_brightcove3_data($field, $item) {
  // Initialize the data array.
  $data = array();

  $token = variable_get('emvideo_brightcove3_token', '');

  if (!function_exists('json_decode')) {
    $json_decode_link = l('json_decode', 'http://php.net/json_decode', array('absolute' => TRUE, 'attributes' => array('target' => '_blank')));
    $error_text = t(
      "Brightcove3: Could not retrieve data (including thumbnail) for video @vidId because function !json_decode doesn't exist.",
      array(
        '@vidId' => $item['value'],
        '!json_decode' => $json_decode_link
      )
    );
    emfield_set_error($error_text);
    return array();
  }

  //try to load a cached value
  if ($cache = cache_get('emvideo_brightcove3_data_' . $item['value'], 'cache')) {
    return $cache->data;
  }

  // connect and fetch a value
  $url = 'http://api.brightcove.com/services/library?command=find_video_by_id&video_id='. $item['value'] .'&token='. $token;
  $result = drupal_http_request($url);

  if ($result->code != 200) {
    $error_text = t('Brightcove3: HTTP error @info when fetching video data from URL !url.',
      array('@info' => "$result->code: $result->error", '!url' => $url));
    return array();
  }

  $data = json_decode($result->data, TRUE);
  if (!$data) {
    emfield_set_error(t('Brightcove3: Error parsing JSON @json', array('@json' => $data)));
    return array();
  }

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emvideo_brightcove3_version'] = EMVIDEO_BRIGHTCOVE3_DATA_VERSION;

  cache_set('emvideo_brightcove3_data_' . $item['value'], $data, 'cache', time() + variable_get('emfield_cache_duration', 3600));

  return $data;
}

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emvideo_brightcove3_rss($item, $teaser = NULL) {
//   if ($item['value']) {
// //     $file['thumbnail']['filepath'] = $item['data']['thumbnail'];
//
//     return $file;
//   }
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_brightcove3_embedded_link($video_code) {
}

/**
 * The embedded flash displaying the brightcove3 video.
 */
function theme_emvideo_brightcove3_flash($item, $width, $height, $autoplay) {
  $output = '';
  if ($item['embed']) {
    $publisher_id = variable_get('brightcove3_publisher_id', '');
    $player_id = variable_get('brightcove3_player_id', '');

    if (!empty($publisher_id) && !empty($player_id)) {
      // If SWFObject API is available, use it.
      if (function_exists('theme_swfobject_api')) {
        $url = 'http://c.brightcove.com/services/viewer/federated_f9/' . $player_id . '?isVid=1&publisherID=' . $publisher_id;
        $params = array(
          'base' => 'http://admin.brightcove.com',
          'name' => 'flashObj',
          'width' => $width,
          'height' => $height,
          'seamlesstabbing' => 'false',
          'type' => 'application/x-shockwave-flash',
          'allowFullScreen' => 'true',
          'swLiveConnect' => 'true',
        );
        $vars = array(
          'videoId' => $item['value'],
          'playerId' => $player_id,
        );
        return theme('swfobject_api', $url, $params, $vars);
      }
      $autostart = $autoplay ? 'autoStart=true' : 'autoStart=false';
      return sprintf('<embed src="http://c.brightcove.com/services/viewer/federated_f9/%3$s?isVid=1&publisherID=%2$s" bgcolor="#FFFFFF" flashVars="videoId=%1$s&playerID=%3$s&domain=embed&" base="http://admin.brightcove.com" name="flashObj" width="%4$d" height="%5$d" seamlesstabbing="false" type="application/x-shockwave-flash" allowFullScreen="true" swLiveConnect="true" pluginspage="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"></embed>',
        $item['value'], $publisher_id, $player_id, $width, $height, $autoplay
      );
    }
  }
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_brightcove3_thumbnail($field, $item, $formatter, $node, $width, $height) {
  // In this demonstration, we previously retrieved a thumbnail using oEmbed
  // during the data hook.
  return $item['data']['videoStillURL'] ? $item['data']['videoStillURL'] : $item['data']['thumbnailURL'];
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_brightcove3_video($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_brightcove3_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_brightcove3_preview($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_brightcove3_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  Implementation of hook_emfield_subtheme.
 *  This returns any theme functions defined by this provider.
 */
function emvideo_brightcove3_emfield_subtheme() {
    $themes = array(
        'emvideo_brightcove3_flash'  => array(
            'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/brightcove3.inc',
            // If you don't provide a 'path' value, then it will default to
            // the emvideo.module path. Obviously, replace 'embrightcove3' with
            // the actual name of your custom module.
            'path' => drupal_get_path('module', 'media_brightcove'),
        )
    );
    return $themes;
}
