 #define TX_MODE 1
// #define RX_MODE 2

const char msg[] = "LED ON";

#define LED_ON_CODE 123456

#ifdef TX_MODE

// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <RH_ASK.h>
// #include <SPI.h>

// #define PIR_PIN D5
// #define TRANSMITTER_PIN D6

// // parametreler RH_ASK.h dosyasindan alinabilir
// RH_ASK driverTx(2000, TRANSMITTER_PIN, D3, D4, true);

// void setup() {
//   Serial.begin(115200);
//   pinMode(PIR_PIN, INPUT);

//   // init fonkundaki pin baslatmalari IPTAL EDILMELI
//   if (!driverTx.init()) {
//     Serial.println("RF driver initialization failed");
//   }
//   driverTx.setModeTx();
// }

// void loop() {
//   int motionDetected = digitalRead(PIR_PIN);
//   Serial.println(motionDetected);
//   if (motionDetected == HIGH) {
//     driverTx.send((uint8_t *)msg, strlen(msg));
//     driverTx.waitPacketSent();
//   }
//   delay(250);
// }


#include <ESP8266WiFi.h>
#include "RCSwitch.h"

#define PIR_PIN D5
#define TRANSMITTER_PIN D6

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  // Initialize the RCSwitch library with the transmitter pin
  mySwitch.enableTransmit(TRANSMITTER_PIN);
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);
  Serial.println(motionDetected);

  if (motionDetected == HIGH) {
    mySwitch.send(LED_ON_CODE, 24); // 24 is the number of bits in the code
  }
  
  delay(500);
}




#elif RX_MODE

// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <RH_ASK.h>
// #include <SPI.h>

// #define RECEIVER_PIN D6
// #define LED_PIN D5

// RH_ASK driverRx(2000, D3, RECEIVER_PIN, D4, false);

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_PIN, OUTPUT);

//   // init fonkundaki pin baslatmalari IPTAL EDILMELI
//   if (!driverRx.init()) {
//     Serial.println("RF driver initialization failed");
//   }
//   driverRx.setModeRx();
// }

// void loop() {
//   //uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
//   uint8_t buf[10];
//   uint8_t buflen = sizeof(buf);

//   if (driverRx.recv(buf, &buflen)) {
//     buf[buflen] = '\0';
//     Serial.print("Received: ");
//     Serial.println((char*)buf);

//     digitalWrite(LED_PIN, HIGH);
//     delay(500);
//     digitalWrite(LED_PIN, LOW);
    
//     // if (strcmp((char*)buf, msg) == 0) {
//     //   digitalWrite(LED_PIN, HIGH);
//     //   delay(500);
//     //   digitalWrite(LED_PIN, LOW);
//     // }
//   }
// }




#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "RCSwitch.h"

// Define the pin connected to the RF receiver
#define RECEIVER_PIN D6

// Define the pin connected to the LED
#define LED_PIN D5

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);

  // Initialize the RCSwitch library with the receiver pin
  mySwitch.enableReceive(RECEIVER_PIN);

  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Check if a signal has been received
  if (mySwitch.available()) {
    // Read the received value
    unsigned long receivedValue = mySwitch.getReceivedValue();

    // Check if the received value matches the LED on code
    if (receivedValue == LED_ON_CODE) {
      Serial.println("Received LED ON code. Turning LED on.");

      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
    }

    // Reset the received value for the next transmission
    mySwitch.resetAvailable();
  }
}


#endif