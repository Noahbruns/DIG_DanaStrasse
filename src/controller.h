#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <AccelStepper.h>
#include <driver.h>

#define MAX_SPEED        2000//1600 // Maximum speed in steps per second
#define MAX_ACCELERATION 5000//5000

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

#endif  // CONTROLLER_H