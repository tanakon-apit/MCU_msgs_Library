#include "MCU_msgs.h"

#define RGB_LED_PIN 21

uint32_t Update_Time = 0;
uint32_t RGB_Time = 0;

MCU_msgs esp_msgs;
MCU_msgs ser_msgs;

uint8_t tx_msg_buffer[100];

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  while (!Serial)
    ;

  MCU_msgs_Init(&esp_msgs, 3, 71, 2);
  MCU_msgs_Init(&ser_msgs, 3, 10, 2);

  Update_Time = millis() + 10;
  RGB_Time = millis() + 10;
}

void loop() {
  uint32_t Time = millis();

  if (Time >= RGB_Time) {
    RGB_Time += 10;
    uint8_t R = (16 * sin((2 * M_PI * 0.5 * Time / 1000.0))) + 16;
    uint8_t G = (16 * sin((2 * M_PI * 0.5 * Time / 1000.0) + (2 * M_PI / 3.0))) + 16;
    uint8_t B = (16 * sin((2 * M_PI * 0.5 * Time / 1000.0) + (4 * M_PI / 3.0))) + 16;
    neopixelWrite(RGB_LED_PIN, R, G, B);
  }

  if (Serial.available()) {
    decode_buffer2msg(&ser_msgs, Serial.read());
    if (ser_msgs.status) {
      esp_msgs.buff[0].f64 = ser_msgs.buff[0].f64;
      esp_msgs.buff[1].f64 = ser_msgs.buff[1].f64;
      esp_msgs.buff[2].f64 = ser_msgs.buff[2].f64;
    }
  }

  if (Time >= Update_Time) {
    Update_Time += 10;
    encode_msg2buffer(&esp_msgs, tx_msg_buffer, sizeof(tx_msg_buffer));
    Serial.write(tx_msg_buffer, (esp_msgs.buff_size * 8) + 2);
    Serial.println(' ');
  }
}
