#include <Arduino.h>

#define LS_RAMP_1_PIN A1
#define LS_END_PIN A0

#define LIGHT_SENSOR_THRESHOLD 100
#define LASER_SENSOR_THRESHOLD 1012

// returns true if workpiece detected
bool ramp_light_sensor()
{
    int s1 = analogRead(LS_RAMP_1_PIN);
    //int s2 = analogRead(LS_RAMP_2_PIN);

    /*static uint32_t last_time = 0;
    uint32_t ms = millis();

    if ((ms - last_time) > 100)
    { // run every 0.1s
        last_time = ms;

        Serial.print(s1, DEC);
        Serial.print(" ");
        Serial.print(s2, DEC);
        Serial.print(" ");
        Serial.print((s1 < LIGHT_SENSOR_THRESHOLD) && (s2 < LIGHT_SENSOR_THRESHOLD), DEC);
        Serial.println();
    }*/

    return (s1 < LIGHT_SENSOR_THRESHOLD); // && (s2 < LIGHT_SENSOR_THRESHOLD));
}

bool end_light_sensor()
{
    int s = analogRead(LS_END_PIN);

    static uint32_t last_time = 0;
    uint32_t ms = millis();

    /*if ((ms - last_time) > 100)
    { // run every 0.1s
        last_time = ms;

        Serial.print("end_light_sensor: ");
        Serial.println(s);
    }*/

    return (s < LASER_SENSOR_THRESHOLD);
}