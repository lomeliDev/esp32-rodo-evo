#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Globals.h"
#include "Write.h"
#include "Bluetooth.h"

void write(byte _byte)
{
    writeBluetooth(_byte);
}