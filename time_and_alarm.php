<?php
   $alarm_string = explode(':',file_get_contents("alarm.txt"));
   if($alarm_string[0] < 10) $alarm_string[0] = "0".$alarm_string[0];
   if($alarm_string[1] < 10) $alarm_string[1] = "0".$alarm_string[1];
   echo $alarm_string[0].":".$alarm_string[1];
   echo "\r\n";
   date_default_timezone_set("Asia/Bangkok");
   echo date("H:i\r\n");
