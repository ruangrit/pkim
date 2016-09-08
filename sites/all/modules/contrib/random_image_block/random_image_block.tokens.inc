<?php if ( ! defined('DRUPAL_ROOT')) exit('No direct script access allowed');
/**
 *
 * Originally based on Drupal7 Tokens Example
 * Alterations Copyright (C) 2012 PerceptiSys Ltd
 * Licensed for reuse using the Gnu Public License (GPL) version 3.0
 * or later. See: http://www.gnu.org/licenses/gpl.html
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @file
 * Token callbacks for the random_image_block module.
 */

$imageDirectory = variable_get('random_image_block_image_directory', 'sites/all/libraries/randomImages');
$imageListFile = DRUPAL_ROOT.'/'.$imageDirectory.'/'.variable_get('random_image_block_image_list_php', 'imageList.php');
if (file_exists($imageListFile)) {
  // There is no longer any simple way of catching syntax exceptions
  // in a dynamic require!
  // (even the user defined exception handler does not catch syntax errors ;-(
  //   http://php.net/manual/en/function.set-error-handler.php
  //
  // The php_check_syntax page has some (complex) solutions
  //   http://php.net/manual/en/function.php-check-syntax.php
  //
  // WE WILL NOT EVEN ATTEMPT TO CATCH SYNTAX ERRORS AT THIS TIME
  //
  require_once($imageListFile);
}

/**
 * Implements hook_token_info().
 *
 * @ingroup random_image_block
 */
function random_image_block_token_info() {
  $info['types']['rib'] = array(
    'name' => t('Random Image Block'),
    'description' => t('Tokens related to the images displayed by the random image block.'),
  );

  // Tokens for the text format token type.
  $info['tokens']['rib']['credits'] = array(
    'name' => t('Credits for all images'),
    'description' => t("Return an html fragment which credits the owners of all images currently displayed by the random image block."),
  );

  /* get the list of random images */
  if (!function_exists('random_image_block_get_image_list')) {
    drupal_set_message("Could not find the function random_image_block_get_image_list in the Random Image Block tokens_info; check your configuration of the Random Image Block.", "error");
    return array();
  }

  $imageList = random_image_block_get_image_list();
  $randomImageKeys = array_keys($imageList);

  $info['tokens']['rib']['credit'] = array(
    'name' => t('Credit for an image'),
    'description' => t("Return an html fragement which credits the owner of a specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['url'] = array(
    'name' => t('URL'),
    'description' => t("Return the url for the specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['title'] = array(
    'name' => t('Title'),
    'description' => t("Return the title of the specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['shortTitle'] = array(
    'name' => t('Short title'),
    'description' => t("Return the short title of the specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['width'] = array(
    'name' => t('Width'),
    'description' => t("Return the width of the specified image. The possible images are @imageName and current-image.", 
                       array('@imageName' => implode(', ', $randomImageKeys))),
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['height'] = array(
    'name' => t('Height'),
    'description' => t("Return the height of the specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['license'] = array(
    'name' => t('License'),
    'description' => t("Return an html link to the license for the specified image. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['shortCC'] = array(
    'name' => t('Short Creative Commons indicator'),
    'description' => t("Return the Creative Commons indicator for the specified image (by, by-sa, etc). See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['accessedOn'] = array(
    'name' => t('Accessed on'),
    'description' => t("Return the date the last time the image's license was checked. See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['nameHtml'] = array(
    'name' => t('Name html'),
    'description' => t("Return an html link to the image owner's photo stream or website. (Used to credit this owner). See width for the possible list of image names."), 
    'dynamic' => TRUE
  );
  $info['tokens']['rib']['shareHtml'] = array(
    'name' => t('Share html'),
    'description' => t("Return an html fragment which includes a small version of the full image in the image owner's photo stream or website. (Used to credit this owner). See width for the possible list of image names."), 
    'dynamic' => TRUE
  );

  return $info;
}

/**
 * Implements hook_tokens().
 *
 * @ingroup random_image_block
 */
function random_image_block_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // All of the values we use come from the imageList.php file (or its 
  // configured equivilant) and not directly from a user.... 
  // ... SO we do not bother to sanitize ....
  //
  $sanitize = !empty($options['sanitize']);

  if (!function_exists('random_image_block_get_image_list')) {
    drupal_set_message("Could not find the function random_image_block_get_image_list in the Random Image Block tokens; check your configuration of the Random Image Block.", "error");
    return array();
  }

  $imageList = random_image_block_get_image_list();

  // Text format tokens.
  if ($type == 'rib') {
    foreach ($tokens as $name => $original) {
      $names = preg_split('/:/', $name);
      if (isset($names[1]) && (strcmp($names[1],'current-image') == 0)) {
        $names[1] = random_image_block_get_current_image_key();
      }
      switch ($names[0]) {
        case 'credits':
          drupal_add_css(drupal_get_path('module', 'random_image_block') . '/image_credit.css');
          $output = '';
          $seenImageIds = array();
          foreach ($imageList as $anImageKey => $anImage) {
            if (!isset($seenImageIds[$anImage['id']])) {
              $seenImageIds[$anImage['id']] = TRUE;
              $output .= _buildCreditDivFor($anImage);
            }
          }
          $replacements[$original] = $output;
        break;
        case 'credit':
          // need to build 
          drupal_add_css(drupal_get_path('module', 'random_image_block') . '/image_credit.css');
          $replacements[$original] = _buildCreditDivFor($imageList[$names[1]]);
        break;
        case 'url':
        case 'title':
        case 'shortTitle':
        case 'width':
        case 'height':
        case 'license':
        case 'shortCC':
        case 'accessedOn':
        case 'nameHtml':
        case 'shareHtml':
          $rib_data = $imageList[$names[1]][$names[0]];
          if ($names[0] == 'url') {
            $imageDirectory = variable_get('random_image_block_image_directory', 'sites/all/libraries/randomImages');
            $rib_data = DRUPAL_ROOT.'/'.$imageDirectory.'/'.$rib_data;
          }
          $replacements[$original] = $rib_data;
        break;
      }
    }
  }

  return $replacements;
}

function _buildCreditDivFor($anImage) {
  $output  = "<div class=\"rib-image-credit\">";
  $output .= "  ".$anImage['shareHtml'];
  $output .= "  <br/>";
  $output .= "  ".$anImage['shortTitle'];
  $output .= "  <br/>";
  $output .= "  ".$anImage['nameHtml'];
  $output .= "  <br/>";
  $output .= "  (Accessed ".$anImage['accessedOn']." )";
  $output .= "  <br/>";
  $output .= "  <a href=\"".$anImage['license']."\"";
  $output .= "    alt=\"CC ".$anImage['shortCC']."\"";
  $output .= "    title=\"CC ".$anImage['shortCC']."\">";
  $output .= "    <img src=\"http://i.creativecommons.org/l/".$anImage['shortCC']."/3.0/88x31.png\"";
  $output .= "      width=\"88\" height=\"31\" />";
  $output .= "  </a>";
  $output .= "</div>";
  return $output;
}
