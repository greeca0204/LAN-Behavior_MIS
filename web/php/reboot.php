<?php
include('send_msg.php');

$in = "reboot";
send_msg($in);

echo '<script>window.location.href="../index.php"</script>';