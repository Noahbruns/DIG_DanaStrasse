#include <Arduino.h>

#define LS_RAMP_PIN A0
#define LS_END_PIN A1

#define RAMP_SENSOR_THRESHOLD 400
#define END_SENSOR_THRESHOLD 1010

// returns true if workpiece detected
bool ramp_light_sensor()
{
    int s1 = analogRead(LS_RAMP_PIN);

    return (s1 < RAMP_SENSOR_THRESHOLD);
}

bool end_light_sensor()
{
    int s = analogRead(LS_END_PIN);

    return (s < END_SENSOR_THRESHOLD);
}

void debug() {
    static uint32_t last_ms = 0;
    uint32_t ms = millis();

    int s_ramp = analogRead(LS_RAMP_PIN);
    int s_end = analogRead(LS_END_PIN);

    if ((ms - last_ms) > 100)
    { // run every 0.1s
        last_ms = ms;

        Serial.print("ramp: ");
        Serial.print(s_ramp);
        Serial.print(", end: ");
        Serial.println(s_end);
    }
}