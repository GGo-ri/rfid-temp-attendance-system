# 배선 참고표 (Arduino Uno 기준)

코드(`firmware/`)에 정의된 핀과 Uno 의 기본 SPI/I2C 핀을 바탕으로 정리한 **참고용 표**입니다. 실제 제작한 배선도나 회로도에서 옮긴 것이 아니므로, 부품 모듈에 따라 다를 수 있습니다. 조립할 때는 사용하는 모듈의 데이터시트를 함께 확인하세요.

| 부품 | 부품 핀 | Arduino 핀 | 비고 |
|---|---|---|---|
| MFRC522 (RFID) | SDA(SS) | D10 | 코드의 `SS_PIN` |
| | SCK | D13 | SPI |
| | MOSI | D11 | SPI |
| | MISO | D12 | SPI |
| | RST | D9 | 코드의 `RST_PIN` |
| | 3.3V / GND | 3.3V / GND | 5V에 연결하지 말 것 |
| MLX90614 (I2C) | SDA | A4 | Uno 의 I2C 핀 |
| | SCL | A5 | Uno 의 I2C 핀 |
| | VCC / GND | 모듈 사양에 맞는 전원 / GND | |
| ESP8266 (Wi-Fi) | TX | D2 | 코드의 `SoftwareSerial esp(2, 3)` 에서 아두이노 RX |
| | RX | D3 | 아두이노 TX. 3.3V 레벨이 필요하면 분압 저항 사용 |
| | VCC / CH_PD(EN) | 3.3V | 전류가 부족하면 별도 3.3V 전원 사용 |
| | GND | GND | 아두이노와 GND 공통 |

## 참고

- 코드는 ESP8266 과 9600bps(`esp.begin(9600)`)로 통신합니다. 모듈의 AT 펌웨어 통신 속도가 이와 같아야 합니다.
