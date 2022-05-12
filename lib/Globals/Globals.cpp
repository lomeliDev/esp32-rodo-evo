#include <Arduino.h>
#include "Globals.h"
#include "SoftwareSerial.h"

/*Serial*/
bool debug = true;

/*BLUETOOTH*/
std::string nameBluetooth = "Evogas";
uint32_t pinBluetooth = 999999;
int delayBluetooth = 30;