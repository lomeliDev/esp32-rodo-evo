#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Globals.h"
#include "Write.h"
#include "Bluetooth.h"

void setup() {
  Serial.begin(38400);
  Serial.println("Se inicializo el ESP32");
  setupBluetooth();
  Serial.println("Se inicializo el Bluetooth");
}

void loop() {
  loopReadBluetooth();
  delay(10);
}