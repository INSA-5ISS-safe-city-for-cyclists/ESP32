#include "Request.h"


using namespace std;

Request::Request(/* args */)
{
}

Request::~Request()
{
}

void Request::wifi_init() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void Request::wifi_scan() {
    Serial.println("START SCAN ...");
    int n = WiFi.scanNetworks();
    Serial.print(n);
    Serial.println(" network(s) found");
    for (int i = 0; i < n; i++)
    {
        Serial.println(WiFi.SSID(i));
    }
    Serial.println();
    delay(5000);
}

void Request::Wifi_cnx() {
    WiFi.begin(this->SSID, this->PASS);
    Serial.println("Connecting .... ");
}


void Request::get_data(Speed spd, Sim gps) {
    DynamicJsonDocument doc(1024);
    doc["timestamp"]=gps.get_UTC_Time();
    doc["date"]=(String) gps.get_Date() + "T" + gps.get_UTC_Time() +"Z";
    doc["distance"]= spd.get_distance();
    doc["object_speed"]=spd.get_vehicule_speed();
    doc["latitude"]=gps.get_Latitude();
    doc["longitude"]=gps.get_Longitude();
    doc["bicycle_speed"]=gps.get_speed_bike();

    String body;
    serializeJson(doc, body);
    Serial.println(body);
    this->data.push_back(body);
}

void Request::post_DATA() {
    WiFiClient client;
    HTTPClient httpClient;

    httpClient.begin(client, this->url_data);
    httpClient.addHeader("Content-Type", "application/json");
    String body;
    for (size_t i = 0; i < this->data.size() ;i++)
    {
        body += this->data[i];
        if(i < this->data.size()-1) body += ",";
    }
    body = "[" + body + "]";
    Serial.println(body);
    this->code_http = httpClient.POST(body);
    String content = httpClient.getString();
    httpClient.end();
    Serial.println(content);
    delay(5000);
}