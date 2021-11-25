#include "Speed.h"


Speed:: Speed(int sensor_cm) {
    this->sensors_distance_cm = sensor_cm;
}

void Speed::set_range_cm(long range_0, long range_1) {
    this->range_cm_0 = range_0;
    this->range_cm_1 = range_1;
}

void Speed::compute_vehicles_speed() {


    int32_t speed_range_0 = (this->last_range_0 - this->range_cm_0) * 1E3 / (millis()- this->last_timestamp_range_0);
    int32_t speed_range_1 = (this->last_range_1 - this->range_cm_1) * 1E3 / (millis()- this->last_timestamp_range_1);



    if(speed_range_0 > 1000) {this->timestamp_sensor_0 = micros();  this->flag_0 = true;}
    if(speed_range_1 > 1000) {this->timestamp_sensor_1 = micros();  this->flag_1 = true;}
    
    
    if(flag_0 && flag_1){
        this->vehicles_speed = this->sensors_distance_cm * 1E1 * 3600 / abs(this->timestamp_sensor_0 - this->timestamp_sensor_1);
        this->flag_0 = false;
        this->flag_1 = false;
    }

    this->last_range_0 = this->range_cm_0;
    this->last_timestamp_range_0 = millis();

    this->last_range_1 = this->range_cm_1;
    this->last_timestamp_range_1 = millis();

    if(this->vehicles_speed > 5000) this->vehicles_speed = 0;
}

long Speed::get_vehicule_speed() {
    return this->vehicles_speed;
}

void Speed::reset() {
    Serial.print(this->range_cm_0);Serial.print(" cm_0: ");
    Serial.print(this->range_cm_1);Serial.print(" cm_1: ");
    Serial.print("vehicles speed: "); Serial.print(this->vehicles_speed); Serial.println("Km/h");
    this->vehicles_speed = 0;
}

long Speed:: get_distance() {
    long min;
    this->range_cm_0 < this->range_cm_1 ? min = this->range_cm_0 : min = this->range_cm_1;
    return min;
}