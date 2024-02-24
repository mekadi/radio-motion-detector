// #define TX_MODE 1
 #define RX_MODE 2

const char msg[] = "LED ON";

#ifdef TX_MODE

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <RH_ASK.h>
#include <SPI.h>

#define PIR_PIN D5
#define TRANSMITTER_PIN D6

// parametreler RH_ASK.h dosyasindan alinabilir
RH_ASK driverTx(2000, D3, TRANSMITTER_PIN, D4, true);

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);

  if (!driverTx.init()) {
    Serial.println("RF driver initialization failed");
  }
  driverTx.setModeTx();
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);
  Serial.println(motionDetected);
  if (motionDetected == 1) {
    driverTx.send((uint8_t *)msg, strlen(msg));
    driverTx.waitPacketSent();
  }
  delay(250);
}

#elif RX_MODE

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <RH_ASK.h>
#include <SPI.h>

#define RECEIVER_PIN D6
#define LED_PIN D5

RH_ASK driverRx(2000, RECEIVER_PIN, 4, 5, false);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // sadece pinleri baslatiyor. rf takili olmasa bile calisiyor
  if (!driverRx.init()) {
    Serial.println("RF driver initialization failed");
  }
  driverRx.setModeRx();
}

void loop() {
  //uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buf[10];
  uint8_t buflen = sizeof(buf);

  if (driverRx.recv(buf, &buflen)) {
    //buf[buflen] = '\0';
    //Serial.print("Received: ");
    //Serial.println((char*)buf);

    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
}

#endif