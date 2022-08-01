#include <Arduino.h>

#include "controller.h"
#include "driver.h"
#include "state_machine.h"

void setup() {
  Serial.begin(115200);

  // Add transitions
  register_transitions();

  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);
  pinMode(GO_BUTTON_PIN, INPUT);

  // Enable one according to your setup
  SPI.begin();                    // SPI drivers
  //SERIAL_PORT.begin(115200);      // HW UART drivers
  //driver.beginSerial(115200);     // SW UART drivers

  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
                                  // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(4);                 // Enables driver in software
  driver.blank_time(24);          // Set blank time to 24 microseconds
  driver.rms_current(600);        // Set motor RMS current
  driver.microsteps(MICROSTEPS);          // Set microsteps to 1/16th

  driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  //driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop

  // activate the stall guard
  driver.TCOOLTHRS(0xFFFFF); // 20bit max
  driver.THIGH(0);
  driver.semin(5);
  driver.semax(2);
  driver.sedn(0b01);
  driver.sgt(STALL_VALUE);

  // Init Stepper Controller
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(MAX_ACCELERATION);
  stepper.setPinsInverted(true, false, false);
}

void loop() {
  // process State machine
  machine.run();

  // Execute the Stepper Controller
  stepper.run();

  ramp_light_sensor();
}