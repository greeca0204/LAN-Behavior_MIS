<?php
$op = fopen('../../protocol/protocol.txt', 'a');
fwrite($op, $table . '#' . $action . '#' . $id . "\r\n");
fclose($op);
