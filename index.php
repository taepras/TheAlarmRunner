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
      <br>
      <div class="block">
         <p style="vertical-align:middle">
            Current Alarm: <span style="font-size:30pt;"><?php require('alarmtime.php'); ?></span>
         </p>
         <hr>
         <h3>Set New Alarm</h3>
         <form method="get" action="success.php" class="form-inline">
            <input type="number" name="h" class="form-control time-input input-lg" placeholder="hour" size="2" value="<?php echo $alarm_string[0] ?>" required>
            <b>:</b>
            <input type="number" name="m" class="form-control time-input input-lg" placeholder="minute" size="2" value="<?php echo $alarm_string[1] ?>" required>
            <input type="hidden" name="browser" value="true">
            <br><br>
            <button type="submit" class="btn btn-success btn-lg btn-block">Set Alarm</button>
         </form>
      </div>
   </div>
</body>
</html>
