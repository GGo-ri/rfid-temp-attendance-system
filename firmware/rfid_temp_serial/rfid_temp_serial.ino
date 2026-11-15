/*
 * RFID + 비접촉 체온 측정 (시리얼 모니터 출력 버전)
 * Wi-Fi/서버 없이 센서 동작만 확인할 때 쓴 최종 데모용 스케치.
 *
 * 등록된 카드를 태그하면 체온을 20회 측정해 평균을 출력하고,
 * 등록되지 않은 카드는 거부 메시지를 출력합니다.
 */
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_MLX90614.h>

#define RST_PIN 9
#define SS_PIN  10

// 등록된 카드 UID (앞 4바이트). 본인 카드 값으로 바꾸세요.
const byte ALLOWED_UIDS[][4] = {
  {0x00, 0x00, 0x00, 0x00},
};

MFRC522 mfrc(SS_PIN, RST_PIN);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

bool isAllowed(const byte *uid) {
  for (unsigned i = 0; i < sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]); i++) {
    if (memcmp(uid, ALLOWED_UIDS[i], 4) == 0) return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  mlx.begin();
}

void loop() {
  String Rfid = "";
  int sum = 0;

  if (!mfrc.PICC_IsNewCardPresent()) return;
  if (!mfrc.PICC_ReadCardSerial())   return;

  for (int j = 0; j < 4; j++) {
    Rfid += String(mfrc.uid.uidByte[j]);
  }

  if (isAllowed(mfrc.uid.uidByte)) {
    Serial.println("안녕하세요!");
    Serial.print("카드 번호 : ");
    Serial.println(Rfid);
    Serial.println("체온 센서에 손목을 대고 체온을 측정해 주세요.");
    delay(3000);
    for (int i = 0; i < 20; i++) {
      sum += (int)(mlx.readObjectTempC());
      delay(100);
    }
    Serial.print("당신의 온도는 ");
    Serial.print(sum / 20);
    Serial.println("도 입니다.");
    Serial.println();
  } else {
    Serial.println("우리 학교 학생이 아닙니다.");
    Serial.println();
    delay(1000);
  }
}
