#include <Speed.h>
//#include<BLE.h>
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
// BLE my_ble;

double speed_data;

void setup()
{
	// my_ble.init_BLE();
	// my_ble.start_BLE();
	Serial.begin(112500);

	p.Begin();
	my_lidar_1.initialize_Lidar(&Serial1, 18, 19);
	my_lidar_2.initialize_Lidar(&Serial2, 22, 23);

	p.AddTimeGraph("graph", 1000, "x label", x, "y ", y, "z", z);
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
	speed.compute_vehicles_speed();
	speed_data = speed.get_vehicule_speed();

	x = 0;
	y = 0;
	z = speed_data;

	p.Plot();

	//   my_ble.checking(range_0,speed_data); //check if a smartphone is connected and send json data
}