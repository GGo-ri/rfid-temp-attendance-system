<?php
// enter_list 테이블을 읽어 출입 기록을 표로 보여주는 페이지입니다.
require __DIR__ . '/config.php';

$db = mysqli_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
if (!$db) {
    http_response_code(500);
    exit('DB connection failed');
}
mysqli_set_charset($db, 'utf8mb4');

$result = mysqli_query($db, 'SELECT room, RF_id, temperature, date FROM enter_list ORDER BY date DESC');
?>
<!DOCTYPE html>
<html lang="ko">
<head>
    <meta charset="utf-8">
    <title>Enter List</title>
</head>
<body>
    <table width="880" border="1" align="center">
        <tr>
            <th>강의실</th>
            <th>학생증 ID</th>
            <th>체온</th>
            <th>입실 시간</th>
        </tr>
        <?php while ($row = mysqli_fetch_assoc($result)): ?>
        <tr align="center">
            <td><?= htmlspecialchars($row['room']) ?></td>
            <td><?= htmlspecialchars($row['RF_id']) ?></td>
            <td><?= htmlspecialchars($row['temperature']) ?></td>
            <td><?= htmlspecialchars($row['date']) ?></td>
        </tr>
        <?php endwhile; ?>
    </table>
</body>
</html>
<?php mysqli_close($db); ?>
