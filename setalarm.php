<?php
   if(isset($_GET['h']) && isset($_GET['m'])) {
      $h = $_GET['h'];
      $m = $_GET['m'];
      file_put_contents("alarm.txt", $h.':'.$m);
      if (!isset($_GET['browser'])) {
         echo "OK\r\n";
      }
      return true;
   }
