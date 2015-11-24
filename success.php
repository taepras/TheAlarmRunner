<!doctype html>
<html>
<head>
   <title>The Alarm Runner</title>
   <meta charset="utf-8">
   <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
   <link rel="stylesheet" href="main.css">
</head>
<body>
   <div class="container text-center">
      <h1>The Alarm Runner</h1>
      <div class="block">
         <?php
            $result = require('setalarm.php');
            if($result == "OK"):
         ?>
            <h3>Success!</h3>
            <p style="vertical-align:middle">
               Alarm has been set to <b><?php echo $_GET['h'] ?>:<?php echo $_GET['m'] ?></b>.
            </p>
         <?php else: ?>
            <h3>Uh oh!</h3>
            <p style="vertical-align:middle">
               An error occured, please try again.
            </p>
         <?php endif; ?>
         <br>
         <a id="back" href="index.php" class="btn btn-success btn-lg btn-block">&laquo; Back</a>
      </div>
   </div>
</body>
<script>
   document.getElementById("back").focus();
</script>
</html>
