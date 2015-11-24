<?php
   $h = $_GET['h'];
   $m = $_GET['m'];
   file_put_contents("alarm.txt", $h.':'.$m);
   echo "OK\r\n";
