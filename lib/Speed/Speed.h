#ifndef SPEED_H
#define SPEED_H

#include <Arduino.h>

class Speed
{
public:
    /* data */
    int sensors_distance_cm;
    long range_cm_0=0;
    long range_cm_1=0;
    long last_range_0 = 0;
    long last_range_1 = 0;
    long last_timestamp_range_0 = 1;
    long last_timestamp_range_1 = 1;
    long timestamp_sensor_0 = 1;
    long timestamp_sensor_1 = 0;
    long vehicles_speed = 0;
    long distance = 0;
    bool flag_0 = false;
    bool flag_1 = false;
    long buff_0 = 0;
    long buff_1 = 0;
    long dist_mini=0;
    int8_t sens = 0;
    int erreur=0;

public:
    long speed_range_0 = 0;
    long speed_range_1 = 0;

    Speed(int sensors_cm);
    ~Speed(){};
    void compute_vehicles_speed(void (*callback)(void));
    void set_range_cm(long range_0, long range_1);
    long get_distance();
    void reset();
    long get_vehicule_speed();
    void set_range_0(long range_0);
    void set_range_1(long range_1);
};

#endif
