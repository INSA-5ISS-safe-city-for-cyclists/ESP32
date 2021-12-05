
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#include <Speed.h>
#include <Ultrasonic.h>

#include <Plotter.h>
Plotter p;


/* 1. Define the WiFi credentials */
#define WIFI_SSID "AndroidAPBD99"
#define WIFI_PASSWORD "chks9274"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCksEpMPPWTLieyCGwjYu_Y7iAjsSzMAKE"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://velo-59717-default-rtdb.europe-west1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "rZkxL693FfBaWI397V8gMLwrhbF52HF8v1barzi4"
/* 4. Define the user Email and password that alreadey registerd or added in your project */
//#define USER_EMAIL "test@test.com"
//#define USER_PASSWORD "123456789"

#define WARNING_SPEED 10
#define DISTANCE_BETWEEN_ULTRASONIC 40
#define D5 14
#define D6 12

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

Speed speed(DISTANCE_BETWEEN_ULTRASONIC);

Ultrasonic ultrasonic_0(D5);
Ultrasonic ultrasonic_1(D6);

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

double x;
double y;
double z;

void setup()
{

  //Serial.begin(115200);
  p.Begin();
  //p.AddTimeGraph("lol", 250, "dif_0", x);
  //p.AddTimeGraph("lol", 250, "dif_0", x, "dif_1", y);
  p.AddTimeGraph("lol", 250, "speed", x, "sens_0", y, "sens_1", z);
}

void loop()
{
  long range_0 = ultrasonic_0.MeasureInCentimeters();
  delay(10);
  long range_1 = ultrasonic_1.MeasureInCentimeters();
  delay(10);
  speed.set_range_cm(range_0, range_1);
  speed.compute_vehicles_speed();
  x = speed.get_vehicule_speed();
  y = speed.buff_0;
  z = speed.buff_1;
  p.Plot();
  //Serial.println(x);
  /*if (Firebase.ready() && speed.get_vehicule_speed() > WARNING_SPEED)
  {
    long speed_val = speed.get_vehicule_speed();
    long distance_val = speed.get_distance();
    
    FirebaseJson json;
    Serial.printf("Get int... %s\n", Firebase.getInt(fbdo, "/count") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    count = fbdo.to<int>() + 1;
    String id = String(count);
    json.set(id+"/ts/.sv", "timestamp");
    json.set(id+"/speed/", String(speed_val));
    json.set(id+"/distance/", String(distance_val));
    json.set(id+"/speed_range_0", String(speed.speed_range_0));
    json.set(id+"/speed_range_1", String(speed.speed_range_1));
    Serial.printf("Set json... %s\n", Firebase.updateNode(fbdo, "/velo/", json) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("Set int... %s\n", Firebase.setInt(fbdo, "/count", count) ? "ok" : fbdo.errorReason().c_str());
    speed.reset();
    Serial.println();
    delay(1000);

  }*/
}
