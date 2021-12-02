#ifndef REQUEST_H
#define REQUEST_H

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Speed.h>
#include <Sim.h>
#include <vector>

using namespace std;

class Request
{
private:
    /* data */
    const char* SSID = "SSID";
    const char* PASS = "PASSWORD";
    String url_data = "http://SERVER_IP_ADSRESS:3000/api/markers";
    vector<String> data;

public:
    int code_http;
    Request(/* args */);
    ~Request();
    void wifi_init();
    void wifi_scan();
    void Wifi_cnx();
    void get_data(Speed spd, Sim gps);
    void post_DATA();
};




#endif