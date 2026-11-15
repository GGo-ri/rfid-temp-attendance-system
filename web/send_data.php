<?php
// Arduino(ESP8266)가 보내는 GET 요청을 받아 enter_list 테이블에 저장합니다.
//   예) /send_data.php?room=10613&rfid=1234567&temp=36
require __DIR__ . '/config.php';

$room = $_GET['room'] ?? '';
$rfid = $_GET['rfid'] ?? '';
$temp = $_GET['temp'] ?? ($_GET['temperature'] ?? '');

if ($room === '' || $rfid === '' || $temp === '') {
    http_response_code(400);
    exit('missing parameter');
}

$db = mysqli_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
if (!$db) {
    http_response_code(500);
    exit('DB connection failed');
}

$stmt = mysqli_prepare($db, 'INSERT INTO enter_list (room, RF_id, temperature) VALUES (?, ?, ?)');
mysqli_stmt_bind_param($stmt, 'sss', $room, $rfid, $temp);
mysqli_stmt_execute($stmt);

echo 'OK';
mysqli_close($db);
