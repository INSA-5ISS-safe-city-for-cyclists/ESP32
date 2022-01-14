#include "Speed.h"

int compteur1=0;
int compteur2=0;

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

    this->speed_range_0 = this->last_range_0 - this->range_cm_0;
    this->speed_range_1 = this->last_range_1 - this->range_cm_1;

    /////////////////////////////////////////////////////////////////SENS DIRECT
    //ACTIVATION 1EME CAPTEUR
    if (range_cm_0 < 200 && !flag_0 && !flag_1)
    {

        this->timestamp_sensor_0 = micros();
        this->flag_0 = true;
        this->buff_0 = this->range_cm_0;
        Serial.println("Capteur 1");
        // Serial.print("Flag0 :");
        // Serial.println(flag_0);
        // Serial.print("Flag1 :");
        // Serial.println(flag_1);
        Serial.print("Buff :");
        Serial.println(buff_0);
        Serial.println("************************************");
    }

    if (micros() - this->timestamp_sensor_0 > 1000000 && flag_0 && !flag_1)
    {
        this->flag_0 = false;
        Serial.print("Flag0 timeout :");
        Serial.println(flag_0);
    }

    //ACTIVATION 2EME CAPTEUR

    if (range_cm_1 < 200 && flag_0 && !flag_1)
    {
        this->timestamp_sensor_1 = micros();
        this->flag_1 = true;
        this->buff_1 = this->range_cm_1;
        this->sens = -1;
        Serial.println("Capteur 2");
        // Serial.print("Flag0 :");
        // Serial.println(flag_0);
        // Serial.print("Flag1 :");
        // Serial.println(flag_1);
        Serial.print("Buff :");
        Serial.println(buff_1);
        Serial.println("************************************");
    }

    ////////////////////////////////////////////////  SENS INVERSE
    //ACTIVATION 2EME CAPTEUR
    if (range_cm_1 < 200 && !flag_0 && !flag_1)
    {
        this->timestamp_sensor_1 = micros();
        this->flag_1 = true;
        this->buff_1 = this->range_cm_1;
        Serial.println("Capteur 1");
        // Serial.print("Flag0 :");
        // Serial.println(flag_0);
        // Serial.print("Flag1 :");
        // Serial.println(flag_1);
        Serial.print("Buff :");
        Serial.println(buff_1);
        Serial.println("************************************");
    }
    if (micros() - this->timestamp_sensor_1 > 1000000 && flag_1 && !flag_0)
    {
        this->flag_1 = false;
        Serial.print("Flag1 timeout :");
        Serial.println(flag_1);
    }
    //ACTIVATION 1ER CAPTEUR

    if (range_cm_0 < 200 && flag_1 && !flag_0)
    {
        this->timestamp_sensor_0 = micros();
        this->flag_0 = true;
        this->buff_0 = this->range_cm_0;
        this->sens = 1;
        Serial.println("Capteur 2");
        // Serial.print("Flag0 :");
        // Serial.println(flag_0);
        // Serial.print("Flag1 :");
        // Serial.println(flag_1);
        Serial.print("Buff :");
        Serial.println(buff_0);
        Serial.println("************************************");
    }

    ///////////////////////////////////////CALCUL VITESSE

    // Serial.print("Erreur : ");Serial.println(erreur);

    //CALCUL DE LA VITESSE APRES ACTIVATION DES DEUX CAPTEURS
    if (flag_0 && flag_1)
    {
        // erreur = abs(buff_1 - buff_0);
        // Serial.print("erreur : ");Serial.println(erreur);
        if (erreur < 40)
        {
            this->vehicles_speed = this->sensors_distance_cm * 1E1 * 3600 / abs(this->timestamp_sensor_0 - this->timestamp_sensor_1);
            this->vehicles_speed *= this->sens;
            this->flag_0 = false;
            this->flag_1 = false;

            // Serial.print("erreur: ");
            // Serial.println(erreur);
            if (abs(this->vehicles_speed) > 100)
            {
                this->vehicles_speed = 0;
            }
            else
            {
                // range_cm_0 < range_cm_1 ? dist_mini = range_cm_0 : dist_mini = range_cm_1;
                buff_0 > buff_1 ? dist_mini = buff_1 : dist_mini = buff_0;
                // Serial.print("dist_mini");
                // Serial.println(dist_mini);
                callback();
                delay(500);
            }

            Serial.print("speed: ");
            Serial.println(vehicles_speed);
        }
        else
        {
            this->flag_0 = false;
            this->flag_1 = false;
        }
    }

    this->last_range_0 = this->range_cm_0;
    this->last_timestamp_range_0 = millis();

    this->last_range_1 = this->range_cm_1;
    this->last_timestamp_range_1 = millis();
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