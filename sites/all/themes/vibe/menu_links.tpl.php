<?php
  if (!count($links)) {
    return '';
  }
  $level_tmp = explode('-', key($links));
  $level = $level_tmp[0];

  $output = "<ul class=\"links-$level\">\n";

  foreach ($links as $index => $link) {
    $css_id = str_replace(' ', '_', strip_tags($link));

    $output .= '<li';

    if (stristr($index, 'active')) {
      $output .= ' class="active"';
    }

    $output .= ' id="' . $css_id . '">'. $link ."</li>\n";
  }

  $output .= '</ul>';

  print $output;

