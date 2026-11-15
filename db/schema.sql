-- 출입 기록 테이블
CREATE DATABASE IF NOT EXISTS `test` DEFAULT CHARACTER SET utf8mb4;
USE `test`;

CREATE TABLE IF NOT EXISTS `enter_list` (
  `room`        varchar(50) DEFAULT NULL,                -- 강의실 번호
  `RF_id`       varchar(50) DEFAULT NULL,                -- 학생증 카드 UID
  `temperature` varchar(50) DEFAULT NULL,                -- 측정 체온(℃)
  `date`        datetime    DEFAULT CURRENT_TIMESTAMP    -- 기록 시간
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 동작 확인용 예시 데이터
INSERT INTO `enter_list` (room, RF_id, temperature) VALUES
  ('10613', '1234567', '36'),
  ('10613', '7654321', '37');
