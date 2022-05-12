#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "SoftwareSerial.h"

/*Serial*/
extern bool debug;

/*BLUETOOTH*/
extern std::string nameBluetooth;
extern uint32_t pinBluetooth;
extern int delayBluetooth;

#endif