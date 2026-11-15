/*
 * 카드 UID 확인용 스케치.
 * 학생증을 태그하면 UID 앞 4바이트를 시리얼 모니터(9600)에 출력합니다.
 * 출력된 { 0x.., 0x.., 0x.., 0x.. } 를 그대로 복사해
 * rfid_temp_wifi / rfid_temp_serial 의 ALLOWED_UIDS 에 붙여 넣으면 카드가 등록됩니다.
 */
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN  10

MFRC522 mfrc(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  Serial.println("카드를 태그하세요.");
}

void loop() {
  if (!mfrc.PICC_IsNewCardPresent()) return;
  if (!mfrc.PICC_ReadCardSerial())   return;

  Serial.print("{ ");
  for (int i = 0; i < 4; i++) {
    Serial.print("0x");
    if (mfrc.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(mfrc.uid.uidByte[i], HEX);
    if (i < 3) Serial.print(", ");
  }
  Serial.print(" }   (10진수: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(mfrc.uid.uidByte[i]);
    if (i < 3) Serial.print(" ");
  }
  Serial.println(")");

  mfrc.PICC_HaltA();
  delay(500);
}
