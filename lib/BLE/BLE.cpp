<<<<<<< Updated upstream
=======
#include "BLE.h"

BLE::BLE()
{
}

BLE::~BLE()
{
}

bool deviceConnected_=false;
bool olddeviceConnected_=false;

void BLE::init_BLE(){
    BLEDevice::init("My_ESP32");


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

}

void BLE::start_BLE(){
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

void BLE::send_data(){

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

void MyCharacteristicsCallbacks::onWrite(BLECharacteristic* pCharacteristic){
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

void MyServerCallbacks::onConnect(BLEServer* pServer){
     deviceConnected_ = true;
     BLEDevice::startAdvertising();
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer){
     deviceConnected_ = false;
}

void BLE::checking()
{
        if (deviceConnected_ && olddeviceConnected_) {
        //pCharacteristic->setValue((uint8_t*)&value, 4); // SET VALUE TO MODIFY TO SEND DATA SENSOR
        send_data();
        //value++;
        Serial.println("Connected...");
        delay(5000); // bluetooth stack will go into congestion, if too many packets are sent --> 10 ms est le minimum
    }
    // disconnecting
    if (!deviceConnected_ && olddeviceConnected_) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Disconnected");
        Serial.println("start advertising");
        olddeviceConnected_ = deviceConnected_;
    }
    // connecting
    if (deviceConnected_ && !olddeviceConnected_) {
        Serial.println("Connection...");
        olddeviceConnected_ = deviceConnected_;
    }
}
>>>>>>> Stashed changes
