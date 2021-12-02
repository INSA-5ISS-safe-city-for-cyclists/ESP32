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
#define RCHARACTERISTIC_UUID "fc31717c-0690-4b37-b0e0-4e54facc16d0"



//Initialization of variable
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEService *pService= NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;


//Heritage de la classe BLEServerCallbacks
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCharacteristicsCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if(value.length()>0) {
            Serial.println("**********");
            Serial.print("New value :");
            for (int i=0; i< value.length(); i++)
                Serial.print(value[i]);

            Serial.println();
            Serial.println("***********");

        }

    }
};

void send_data(){
    DynamicJsonDocument doc(1024);
    doc["timestamp"]=millis();
    doc["date"]="2/12/2021";
    doc["distance"]= 100;
    doc["object_speed"]=200;
    doc["latitude"]=50;
    doc["longitude"]=95;
    doc["bicycle_speed"]=15;

    String body;
    serializeJson(doc, body);
    Serial.println(body);

    pCharacteristic->setValue(body.c_str());
    pCharacteristic->notify();
}


void setup (){
    Serial.begin(9600);

    // Create the BLE Device
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    

    // Create the BLE Service
    pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic for the serveur to data
    pCharacteristic = pService->createCharacteristic(
                CHARACTERISTIC_UUID,
                BLECharacteristic::PROPERTY_READ   |
                BLECharacteristic::PROPERTY_WRITE  |
                BLECharacteristic::PROPERTY_NOTIFY |
                BLECharacteristic::PROPERTY_INDICATE
            );

    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());
    
    pCharacteristic->setCallbacks(new MyCharacteristicsCallbacks);

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}


void loop (){
    if (deviceConnected && oldDeviceConnected) {
        //pCharacteristic->setValue((uint8_t*)&value, 4); // SET VALUE TO MODIFY TO SEND DATA SENSOR
        send_data();
        //value++;
        Serial.println("Connected...");
        delay(5000); // bluetooth stack will go into congestion, if too many packets are sent --> 10 ms est le minimum
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Disconnected");
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        Serial.println("Connection...");
        oldDeviceConnected = deviceConnected;
    }
}
