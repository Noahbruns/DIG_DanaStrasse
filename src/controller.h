#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <AccelStepper.h>
#include <driver.h>

#define MAX_SPEED        200 * MICROSTEPS // Maximum speed in steps per second
#define MAX_ACCELERATION 200 * MICROSTEPS

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

#endif  // CONTROLLER_H