#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Globals.h"
#include "Write.h"
#include "Bluetooth.h"

void setup() {
  Serial.begin(38400);
  Serial.write("Se inicializo el ESP32");
  setupBluetooth();
  Serial.write("Se inicializo el Bluetooth");
}

void loop() {
  loopReadBluetooth();
  logBluetooth();
  delay(10);
}