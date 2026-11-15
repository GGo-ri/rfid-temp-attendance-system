/*
 * RFID + 비접촉 체온 측정 + Wi-Fi 전송 (Arduino + ESP8266 AT)
 *
 * 1. MFRC522로 학생증(RFID 카드)을 태그
 * 2. 등록된 카드면 MLX90614로 체온을 20회 측정해 평균
 * 3. ESP8266(AT 명령)으로 서버의 send_data.php 에 GET 요청 전송
 *
 * 아래 "설정" 부분을 환경에 맞게 바꿔서 사용하세요.
 */
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <Adafruit_MLX90614.h>

// ---------------- 설정 ----------------
const char WIFI_SSID[]     = "YOUR_WIFI_SSID";
const char WIFI_PASSWORD[] = "YOUR_WIFI_PASSWORD";
const char SERVER_IP[]     = "192.168.0.10";   // send_data.php 를 올려둔 서버(APM) 주소
const int  SERVER_PORT     = 80;
String lecture_room        = "10613";          // 강의실 번호

// 등록된 카드 UID (앞 4바이트). 본인 카드 값으로 바꾸세요. (rfid_uid_reader 로 확인 가능)
const byte ALLOWED_UIDS[][4] = {
  {0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00},
};
// --------------------------------------

#define RST_PIN 9
#define SS_PIN  10

MFRC522 mfrc(SS_PIN, RST_PIN);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SoftwareSerial esp(2, 3);   // 아두이노 RX(2) <- ESP8266 TX, 아두이노 TX(3) -> ESP8266 RX
String cmd;

bool isAllowed(const byte *uid) {
  for (unsigned i = 0; i < sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]); i++) {
    if (memcmp(uid, ALLOWED_UIDS[i], 4) == 0) return true;
  }
  return false;
}

void connectWiFi() {
  cmd = String("AT+CWJAP=\"") + WIFI_SSID + "\",\"" + WIFI_PASSWORD + "\"";
  esp.println(cmd);
  delay(3000);
  while (esp.find("OK") == false) {
    Serial.println("Wifi connecting...");
    delay(1000);
  }
  Serial.println("Wifi connected!");
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();
  mlx.begin();

  esp.begin(9600);
  delay(1000);
  esp.println("AT+CWMODE=3\r\n");
  connectWiFi();
}

void loop() {
  String temperature = "";
  String Rfid = "";
  int sum = 0;

  if (!mfrc.PICC_IsNewCardPresent()) return;
  if (!mfrc.PICC_ReadCardSerial())   return;

  for (int j = 0; j < 4; j++) {
    Rfid += String(mfrc.uid.uidByte[j]);
  }

  if (isAllowed(mfrc.uid.uidByte)) {
    Serial.println("Card detected!");
    Serial.print("Card number : ");
    Serial.println(Rfid);

    esp.println("AT+CIPMUX=1");
    delay(1000);
    esp.println(String("AT+CIPSTART=4,\"TCP\",\"") + SERVER_IP + "\"," + SERVER_PORT);
    delay(1000);
    if (esp.find("Error")) {
      Serial.println("TCP connect error");
      return;
    }

    // 체온 20회 측정 후 평균
    for (int i = 0; i < 20; i++) {
      sum += (int)(mlx.readObjectTempC());
      delay(100);
    }
    temperature = String(sum / 20);
    Serial.print("Temperature : ");
    Serial.println(temperature);

    cmd = String("GET http://") + SERVER_IP + "/send_data.php?room=" + lecture_room +
          "&rfid=" + Rfid + "&temp=" + temperature + " HTTP/1.0";
    esp.print("AT+CIPSEND=4," + String(cmd.length() + 4));
    delay(1000);
    esp.println(cmd);   // 요청줄 + CRLF
    esp.println();      // 헤더 끝(빈 줄). CIPSEND 길이(cmd + 4)는 CRLF 2번을 포함한 값
    delay(1000);
    Serial.println("Send!");
    delay(1000);
  } else {
    Serial.println("Unregistered card");
    delay(1000);
  }
}
