#include "Speed.h"

Speed::Speed(int sensor_cm)
{
    this->sensors_distance_cm = sensor_cm;
}

void Speed::set_range_cm(long range_0, long range_1)
{
    this->range_cm_0 = range_0;
    this->range_cm_1 = range_1;
}

void Speed::set_range_0(long range_0)
{
    this->range_cm_0 = range_0;
}

void Speed::set_range_1(long range_1)
{
    this->range_cm_1 = range_1;
}

void Speed::compute_vehicles_speed(void (*callback)(void))
{

    //this->flag_0 = false;
    //this->flag_1 = false;

    int erreur = 0;

    this->speed_range_0 = this->last_range_0 - this->range_cm_0;
    this->speed_range_1 = this->last_range_1 - this->range_cm_1;

    // Serial.println(speed_range_0);
    // Serial.println(speed_range_1);

    ///////////////////////////////////////////// SENS DIRECT
    //ACTIVATION 1ER CAPTEUR
    // if (range_cm_0 < 100 && !flag_0 && !flag_1)
    // {
    //     this->timestamp_sensor_0 = micros();
    //     this->flag_0 = true;
    //     this->buff_0 = this->range_cm_0;
    //     // Serial.print("Flag0 :");
    //     // Serial.println(flag_0);
    //     // Serial.print("Flag1 :");
    //     // Serial.println(flag_1);
    // }

    //ACTIVATION 2EME CAPTEUR

    // if (range_cm_1 < 100 && flag_0 && !flag_1)
    // {
    //     this->timestamp_sensor_1 = micros();
    //     this->flag_1 = true;
    //     this->buff_1 = this->range_cm_1;
    //     // Serial.print("Flag0 :");
    //     // Serial.println(flag_0);
    //     // Serial.print("Flag1 :");
    //     // Serial.println(flag_1);
    // }

    /////////////////////////////////////////////// SENS INVERSE
    //ACTIVATION 2EME CAPTEUR
    if (range_cm_1 < 100 && !flag_0 && !flag_1 && this->speed_range_1 > 50)
    {
        this->timestamp_sensor_1 = micros();
        this->flag_1 = true;
        this->buff_1 = this->range_cm_1;
        Serial.println("Capteur 1");
        Serial.print("Flag0 :");
        Serial.println(flag_0);
        Serial.print("Flag1 :");
        Serial.println(flag_1);
    }
    if (micros() - this->timestamp_sensor_1 > 1500000 && flag_1 && !flag_0)
    {
        this->flag_1 = false;
        Serial.print("Flag1 :");
        Serial.println(flag_1);
    }

    //ACTIVATION 1ER CAPTEUR

    if (range_cm_0 < 100 && flag_1 && !flag_0 && this->speed_range_0 > 50)
    {
        this->timestamp_sensor_0 = micros();
        this->flag_0 = true;
        this->buff_0 = this->range_cm_0;
        Serial.println("Capteur 2");
        Serial.print("Flag0 :");
        Serial.println(flag_0);
        Serial.print("Flag1 :");
        Serial.println(flag_1);
    }
    if (micros() - this->timestamp_sensor_0 > 1500000 && flag_0 && !flag_1)
    {
        this->flag_0 = false;
        Serial.print("Flag0 :");
        Serial.println(flag_0);
    }

    ///////////////////////////////////////CALCUL VITESSE

    // erreur = abs(buff_1 - buff_0);

    //CALCUL DE LA VITESSE APRES ACTIVATION DES DEUX CAPTEURS
    if (flag_0 && flag_1 && (erreur < 45))
    {
        this->vehicles_speed = this->sensors_distance_cm * 1E1 * 3600 / abs(this->timestamp_sensor_0 - this->timestamp_sensor_1);
        this->flag_0 = false;
        this->flag_1 = false;
        // if(this->vehicles_speed > 1) {
        //     range_cm_0 < range_cm_1 ? range_cm_0=range_cm_0 : range_cm_0=range_cm_1;
        //     callback();
        // }
        Serial.print("erreur: ");
        Serial.println(erreur);
        if (this->vehicles_speed > 130)
            this->vehicles_speed = 0;
        Serial.print("speed: ");
        Serial.println(vehicles_speed);
    }

    this->last_range_0 = this->range_cm_0;
    this->last_timestamp_range_0 = millis();

    this->last_range_1 = this->range_cm_1;
    this->last_timestamp_range_1 = millis();
    //Serial.print(" flg: ");Serial.print(this->flag_0);Serial.print(this->flag_1);Serial.print(flag_2);
    //Serial.print(" spd: ");Serial.println(this->vehicles_speed);
    //if(this->vehicles_speed > 120) this->vehicles_speed = 0;
}

long Speed::get_vehicule_speed()
{
    return this->vehicles_speed;
}

void Speed::reset()
{
    Serial.print(this->range_cm_0);
    Serial.print(" cm_0: ");
    Serial.print(this->range_cm_1);
    Serial.print(" cm_1: ");
    Serial.print("vehicles speed: ");
    Serial.print(this->vehicles_speed);
    Serial.println("Km/h");
    this->range_cm_0 = 0;
    this->range_cm_1 = 0;
    this->vehicles_speed = 0;
}

long Speed::get_distance()
{
    long min;
    this->range_cm_0 < this->range_cm_1 ? min = this->range_cm_0 : min = this->range_cm_1;
    return min;
}