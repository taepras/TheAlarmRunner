<?php
   if(isset($_GET['h']) && isset($_GET['m'])) {
      $h = intval($_GET['h']);
      $m = intval($_GET['m']);
      $valid = false;
      if(0 <= $h && $h < 24){
         if(0 <= $m && $m < 60){
            file_put_contents("alarm.txt", $h.':'.$m);
            $valid = true;
            if (!isset($_GET['browser'])) {
               echo "OK\r\n";
            }
            return true;
         }
      }
      return false;
   }
