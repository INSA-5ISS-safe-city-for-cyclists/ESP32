
#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <stdlib.h>
#include <ArduinoJson.h>

// Each services and characteristic have an UUID 
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public BLEServerCallbacks {

    public: 
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);

    };

class MyCharacteristicsCallbacks: public BLECharacteristicCallbacks {
    public:
    void onWrite(BLECharacteristic* pCharacteristic);
};

class BLE {
    private:
    BLECharacteristic* pCharacteristic = NULL;
    BLEService *pService= NULL;


    public:
    BLE();
    ~BLE();
    BLEServer* pServer = NULL;

    void init_BLE();
    void start_BLE();
    void send_data(int distance,int vehicule_speed);
    void checking(int distance,int vehicule_speed);

};

#endif