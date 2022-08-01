#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <StateMachine.h>
#include <Arduino.h>
#include "controller.h"
#include "sensors.h"

#define GO_BUTTON_PIN 3

#define LIGHT_SENSOR_PIN 9
#define LIGHT_SENSOR_THRESHOLD 10

#define RUN_DISTANCE 2000 * MICROSTEPS

StateMachine machine = StateMachine();

/* States of the State Machine */
void waiting_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Waiting state");
    stepper.stop();
  }

  if (!stepper.isRunning())
    digitalWrite(EN_PIN, HIGH);
}

void run_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Run state");
    digitalWrite(EN_PIN, LOW);

    stepper.setSpeed(MAX_SPEED);
    //stepper.runSpeed();
    stepper.move(RUN_DISTANCE);
  }
}

void stop_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Stop state");
    digitalWrite(EN_PIN, HIGH);
    stepper.stop();
  }
}

/* Transitions of the State Machine */
bool waiting_run_transition()
{
  // check if go button is pressed
  return digitalRead(GO_BUTTON_PIN) == LOW && !ramp_light_sensor();
}

bool run_stop_transition()
{
  // check if motor is stalled
  return is_stalled();
}

bool stop_waiting_transition()
{
  // check if go button is pressed
  return digitalRead(GO_BUTTON_PIN) == LOW;
}

bool run_waiting_transition()
{
  // check if light sensor is triggered or motor is at end
  return digitalRead(GO_BUTTON_PIN) == HIGH && ( !stepper.isRunning() || ramp_light_sensor() );
}

/* Setup Machine */
State *WAITING = machine.addState(&waiting_state);
State *RUNNING = machine.addState(&run_state);
State *STOP = machine.addState(&stop_state);

void register_transitions()
{
  WAITING->addTransition(&waiting_run_transition, RUNNING);
  RUNNING->addTransition(&run_stop_transition, STOP);
  STOP->addTransition(&stop_waiting_transition, WAITING);
  RUNNING->addTransition(&run_waiting_transition, WAITING);
}

#endif // STATE_MACHINEH