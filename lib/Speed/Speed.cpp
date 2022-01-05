#include "Speed.h"

Speed:: Speed(int sensor_cm ) {
    this->sensors_distance_cm = sensor_cm;
}

void Speed::set_range_cm(long range_0, long range_1) {
    this->range_cm_0 = range_0;
    this->range_cm_1 = range_1;
}

void Speed::compute_vehicles_speed(void (*callback)(void)) {

    //this->flag_0 = false;
    //this->flag_1 = false;

    bool flag_2 = false;

    this->speed_range_0 = this->last_range_0 - this->range_cm_0;
    this->speed_range_1 = this->last_range_1 - this->range_cm_1;

    if(range_cm_0 < 100 && this->speed_range_0 > 50) {this->timestamp_sensor_0 = micros();  this->flag_0 = true; this->buff_0 = this->speed_range_0;}
    if(range_cm_1 < 100 && this->speed_range_1 > 50) {this->timestamp_sensor_1 = micros();  this->flag_1 = true; this->buff_1 = this->speed_range_1;}

    if(flag_0 && flag_1){
        this->vehicles_speed = this->sensors_distance_cm * 1E1 * 3600 / abs(this->timestamp_sensor_0 - this->timestamp_sensor_1);
        this->flag_0 = false;
        this->flag_1 = false;
        callback();
    }

    if(this->vehicles_speed > 200) this->vehicles_speed = 0;

    this->last_range_0 = this->range_cm_0;
    this->last_timestamp_range_0 = millis();

    this->last_range_1 = this->range_cm_1;
    this->last_timestamp_range_1 = millis();
    //Serial.print(" flg: ");Serial.print(this->flag_0);Serial.print(this->flag_1);Serial.print(flag_2);
    //Serial.print(" spd: ");Serial.println(this->vehicles_speed);
    //if(this->vehicles_speed > 120) this->vehicles_speed = 0;
}

long Speed::get_vehicule_speed() {
    return this->vehicles_speed;
}

void Speed::reset() {
    Serial.print(this->range_cm_0);Serial.print(" cm_0: ");
    Serial.print(this->range_cm_1);Serial.print(" cm_1: ");
    Serial.print("vehicles speed: "); Serial.print(this->vehicles_speed); Serial.println("Km/h");
    this->range_cm_0 = 0;
    this->range_cm_1 = 0;
    this->vehicles_speed = 0;
}

long Speed:: get_distance() {
    long min;
    this->range_cm_0 < this->range_cm_1 ? min = this->range_cm_0 : min = this->range_cm_1;
    return min;
}