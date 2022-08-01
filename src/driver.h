#ifndef DRIVER_H
#define DRIVER_H

#include <TMCStepper.h>

#define EN_PIN 10  // Enable
#define DIR_PIN 5  // Direction
#define STEP_PIN 6 // Step
#define CS_PIN 9   // Chip select
#define DIAG_PIN 7 // Diagnostic

/*
#define SW_MOSI          66 // Software Master Out Slave In (MOSI)
#define SW_MISO          44 // Software Master In Slave Out (MISO)
#define SW_SCK           64 // Software Slave Clock (SCK)
#define SW_RX            63 // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX            40 // TMC2208/TMC2224 SoftwareSerial transmit pin
#define SERIAL_PORT   Serial // TMC2208/TMC2224 HardwareSerial port
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
*/

#define STALL_VALUE 25 // [0..255]

#define MICROSTEPS 8

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

// Select your stepper driver type
TMC2130Stepper driver(CS_PIN, R_SENSE); // Hardware SPI
// TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
// TMC2660Stepper driver(CS_PIN, R_SENSE);                           // Hardware SPI
// TMC2660Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
// TMC5160Stepper driver(CS_PIN, R_SENSE);
// TMC5160Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
// TMC2208Stepper driver(&SERIAL_PORT, R_SENSE);                     // Hardware Serial
// TMC2208Stepper driver(SW_RX, SW_TX, R_SENSE);                     // Software serial
// TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
// TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS);

using namespace TMC2130_n;

bool is_stalled()
{
    static uint32_t last_time = 0;
    uint32_t ms = millis();

    DRV_STATUS_t drv_status{0};
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
    }

    if ((ms - last_time) > 10)
    { // run every 0.1s
        last_time = ms;

        Serial.println(drv_status.sg_result, DEC);
    }

    return false; // drv_status.sg_result == 0;
}

#endif // DRIVER_H