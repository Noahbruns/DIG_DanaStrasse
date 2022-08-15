#ifndef DRIVER_H
#define DRIVER_H

#include <TMC2130Stepper.h>
//#include <TMC2130Stepper_REGDEFS.h>

#define EN_PIN 10  // Enable
#define DIR_PIN 5  // Direction
#define STEP_PIN 6 // Step
#define CS_PIN 9   // Chip select
#define DIAG_PIN 7 // Diagnostic

#define STALL_VALUE 25 // [0..255]

#define MICROSTEPS 16

#define CHOPPER_TIMING 3 4 1

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

TMC2130Stepper driver(CS_PIN); // Hardware SPI

bool is_stalled()
{
    static uint32_t last_time = 0;
    uint32_t ms = millis();

    /*DRV_STATUS_t drv_status{0};
    drv_status.sr = driver.DRV_STATUS();

    static uint32_t counter = 0;
    static uint32_t last_ms = 0;
    if (last_ms != ms)
    {
        last_ms = ms;

        if (drv_status.sg_result == 0)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }

        // if stalled for 10 ms
        if (counter > 50)
        {
            counter = 0;
            return false; //true;
        }
    }*/

    if ((ms - last_time) > 10)
    { // run every 0.1s
        last_time = ms;

        Serial.println(digitalRead(DIAG_PIN));
    }

    return false; // drv_status.sg_result == 0;
}

#endif // DRIVER_H