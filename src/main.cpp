 #define TX_MODE 1
// #define RX_MODE 2

#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

const char* msg = "1";

#ifdef TX_MODE

#include <ESP8266WiFi.h>

#define PIR_PIN D5
#define TRANSMITTER_PIN D6

// parametreler RH_ASK.h dosyasindan alinabilir
RH_ASK driverTx(2000, D3, TRANSMITTER_PIN, D4, true);

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  // sadece pinleri baslatiyor. rf takili olmasa bile calisiyor
  if (!driverTx.init()) {
    Serial.println("RF driver initialization failed");
  }
  driverTx.setModeTx();
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == 1) {
    driverTx.send((uint8_t *)msg, strlen(msg));
    driverTx.waitPacketSent();
  }
  delay(250);
}

#elif RX_MODE

#include <ESP8266WiFi.h>

#define RECEIVER_PIN D6
#define LED_PIN D5

RH_ASK driverRx(2000, RECEIVER_PIN, 4, 5, false);

uint8_t buf[5];
uint8_t buflen = sizeof(buf);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  if (!driverRx.init()) {
    Serial.println("RF driver initialization failed");
  }
  driverRx.setModeRx();
}

void loop() {
  if (driverRx.recv(buf, &buflen)) {
    digitalWrite(LED_PIN, HIGH);
    delay(1500);
    digitalWrite(LED_PIN, LOW);
  }
}

#endif