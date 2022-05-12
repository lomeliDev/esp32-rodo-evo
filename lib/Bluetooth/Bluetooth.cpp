#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Bluetooth.h"
#include "Globals.h"
#include "Write.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool rx_received = false;
std::string rxValue;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };
    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {

        if (pCharacteristic->getValue().length() > 0)
        {
            uint lengthBLE = pCharacteristic->getValue().length();
            byte msg[lengthBLE];
            for (size_t i = 0; i < lengthBLE; i++)
            {
                msg[i] = pCharacteristic->getValue()[i];
            }

            if (debug == true)
            {
                Serial.write(msg, lengthBLE);
            }

            writeBluetoothText("hola");

        }

        rx_received = true;
    }
};

class MySecurity : public BLESecurityCallbacks
{
    uint32_t onPassKeyRequest()
    {
        ESP_LOGI(LOG_TAG, "PassKeyRequest");
        return 123456;
    }
    void onPassKeyNotify(uint32_t pass_key)
    {
        ESP_LOGI(LOG_TAG, "The passkey Notify number:%d", pass_key);
    }
    bool onConfirmPIN(uint32_t pass_key)
    {
        ESP_LOGI(LOG_TAG, "The passkey YES/NO number:%d", pass_key);
        vTaskDelay(5000);
        return true;
    }
    bool onSecurityRequest()
    {
        ESP_LOGI(LOG_TAG, "SecurityRequest");
        return true;
    }
    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
    {
        ESP_LOGI(LOG_TAG, "Starting BLE work!");
    }
};

void initBluetooth()
{
    BLEDevice::init(nameBluetooth);
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
    BLEDevice::setSecurityCallbacks(new MySecurity());
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();
    pServer->getAdvertising()->start();
    BLESecurity *pSecurity = new BLESecurity();
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint32_t passkey = pinBluetooth;
    uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
    pSecurity->setCapability(ESP_IO_CAP_OUT);
    pSecurity->setKeySize(16);
    esp_ble_gap_set_security_param(ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option, sizeof(uint8_t));
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));
}

void setupBluetooth()
{
    initBluetooth();
}

void writeBluetooth(byte _byte)
{
    if (deviceConnected == true)
    {
        byte msg[1];
        msg[0] = _byte;
        pTxCharacteristic->setValue(msg, 1);
        pTxCharacteristic->notify();
    }
}

void writeBluetoothText(String _text)
{
    if (deviceConnected == true)
    {
        pTxCharacteristic->setValue("{status: 1}");
        pTxCharacteristic->notify();
    }
}

void logBluetooth()
{
    if (deviceConnected == true)
    {
        pTxCharacteristic->setValue("pin");
        pTxCharacteristic->notify();
        delay(50);
        pTxCharacteristic->setValue("pong");
        pTxCharacteristic->notify();
        delay(50);
    }
}

void loopReadBluetooth()
{
    if (rx_received == true)
    {
        rx_received = false;
    }
}