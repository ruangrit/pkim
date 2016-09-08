<?php
foreach ($items as $delta => $item) {
	//print render($item);
  print '<b>Facebook:</b> '.l($item['#markup'], $item['#markup'], array('external' => TRUE, 'attributes' => array('target'=>'_blank'))); 
} 
?>