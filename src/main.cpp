#include <Speed.h>
#include <BLE.cpp>
#include <LIDAR.h>
#include "Plotter.h"

// SPEED DEFINE
#define WARNING_SPEED 10
#define DISTANCE_BETWEEN_LIDAR 40
double x;
double y;
double z;

// Also declare plotter as global
Plotter p;

Speed speed(DISTANCE_BETWEEN_LIDAR);
LIDAR my_lidar_1, my_lidar_2;
BLE my_ble;

double speed_data;

void sendData(void) {
	Serial.println("SEND DATA DONE");
	my_ble.send_data(speed.get_distance(), speed.get_vehicule_speed());
}

void setup()
{

	Serial.begin(112500);

	my_ble.init_BLE();
	my_ble.start_BLE();

	// p.Begin();
	my_lidar_1.initialize_Lidar(&Serial1, 18, 19);
	my_lidar_2.initialize_Lidar(&Serial2, 22, 23);

	// p.AddTimeGraph("graph", 1000, "x label", x, "y ", y, "z", z);
}

void loop()
{
	long range_0 = my_lidar_1.get_distance_inCm(&Serial1);
	delay(1);
	long range_1 = my_lidar_2.get_distance_inCm(&Serial2);
	delay(1);
	//   Serial.println(range_0);
	//   Serial.println("-----");
	//   Serial.println(range_1);
	speed.set_range_cm(range_0, range_1);
	//speed.compute_vehicles_speed(sendData);
	// speed_data = speed.get_vehicule_speed();

	// x = 0;
	// y = 0;
	// z = speed_data;

	// p.Plot();

	// my_ble.checking(speed.get_distance(), speed.get_vehicule_speed()); // check if a smartphone is connected and send json data

	if (deviceConnected_ && olddeviceConnected_)
	{

		long range_0 = my_lidar_1.get_distance_inCm(&Serial1);
		delay(1);
		long range_1 = my_lidar_2.get_distance_inCm(&Serial2);
		delay(1);
		/*
		Serial.println(range_0);
		Serial.println("-----");
		Serial.println(range_1);
		*/
		speed.set_range_cm(range_0, range_1);
		speed.compute_vehicles_speed(&sendData);

		// pCharacteristic->setValue((uint8_t*)&value, 4); // SET VALUE TO MODIFY TO SEND DATA SENSOR
		/*if (speed.get_distance() < 100)
		{
			Serial.println("SEND DATA DONE");

			Serial.println(speed.get_distance());
			//my_ble.send_data(speed.get_distance(), speed.get_vehicule_speed());
			speed.reset();
			delay(3000);
			speed.set_range_cm(0,0);
			// value++;
		}*/
	}
	// disconnecting
	if (!deviceConnected_ && olddeviceConnected_)
	{
		delay(500);							// give the bluetooth stack the chance to get things ready
		my_ble.pServer->startAdvertising(); // restart advertising
		Serial.println("Disconnected");
		Serial.println("start advertising");
		olddeviceConnected_ = deviceConnected_;
	}
	// connecting
	if (deviceConnected_ && !olddeviceConnected_)
	{
		Serial.println("Connection...");
		olddeviceConnected_ = deviceConnected_;
	}
}
