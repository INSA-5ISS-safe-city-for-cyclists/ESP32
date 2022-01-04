#include <BLE.h>


// Each services and characteristic have an UUID 
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define RCHARACTERISTIC_UUID "fc31717c-0690-4b37-b0e0-4e54facc16d0"

BLE myBLE;


void setup (){
    Serial.begin(9600);
    myBLE.init_BLE();
    myBLE.start_BLE();
}


void loop (){
    myBLE.checking();
}
