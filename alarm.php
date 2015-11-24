<?php
   $alarm_string = explode(':',file_get_contents("alarm.txt"));
   if($alarm_string[0] < 10) $alarm_string[0] = "0".$alarm_string[0];
   if($alarm_string[1] < 10) $alarm_string[1] = "0".$alarm_string[1];
   echo "ALARM\r\n".$alarm_string[0].":".$alarm_string[1];
