#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <StateMachine.h>
#include <Arduino.h>
#include "controller.h"
#include "sensors.h"

#define GO_BUTTON_PIN 3

#define LIGHT_SENSOR_PIN 9
#define LIGHT_SENSOR_THRESHOLD 10

#define RUN_DISTANCE 16500
#define UNLOAD_DISTANCE 6000
#define POST_UNLOAD_DISTANCE 1480

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
    stepper.move(RUN_DISTANCE);
  }
}

void unload_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Unload state");
    digitalWrite(EN_PIN, LOW);

    stepper.setSpeed(MAX_SPEED);
    stepper.move(UNLOAD_DISTANCE);
  }
}

void post_unload_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Post Unload state");
    digitalWrite(EN_PIN, LOW);

    stepper.setSpeed(MAX_SPEED * 10 / 8);
    stepper.move(POST_UNLOAD_DISTANCE);
  }
}

void hold_state()
{
  if (machine.executeOnce)
  {
    Serial.println("Hold state");
    stepper.stop();
  }

  if (!stepper.isRunning())
    digitalWrite(EN_PIN, HIGH);
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

/* Check for Overload */
bool overload()
{
  // check if motor is stalled
  return is_stalled();
}

bool stop_waiting_transition()
{
  // check if go button is pressed
  return digitalRead(GO_BUTTON_PIN) == LOW;
}

/* Transitions of the State Machine */
bool waiting_run_transition()
{
  // if a button is pressed
  return digitalRead(GO_BUTTON_PIN) == LOW;
}

bool run_waiting_transition()
{
  // if the stepper finished running but no pice was detected
  return !stepper.isRunning() ;
}

bool run_hold_transition()
{
  // check if light sensor is triggered or motor is at end
  return end_light_sensor();
}

bool hold_unload_transition()
{
  // check if light sensor is triggered or motor is at end
  return !ramp_light_sensor();
}

bool hold_run_transition()
{
  // check if light sensor is triggered or motor is at end
  return !end_light_sensor();
}

bool unload_post_unload_transition()
{
  // check if light sensor is triggered or motor is at end
  return ramp_light_sensor();
}

bool unload_run_transition()
{
  // check if light sensor is triggered or motor is at end
  return !stepper.isRunning();
}

bool post_unload_run_transition()
{
  // check if light sensor is triggered or motor is at end
  return !stepper.isRunning();
}

/* Setup Machine */
State *WAITING = machine.addState(&waiting_state);
State *RUNNING = machine.addState(&run_state);
State *HOLD = machine.addState(&hold_state);
State *UNLOAD = machine.addState(&unload_state);
State *POST_UNLOAD = machine.addState(&post_unload_state); // just move more steps
State *STOP = machine.addState(&stop_state);

void register_transitions()
{
  WAITING->addTransition(&waiting_run_transition, RUNNING);
  RUNNING->addTransition(&run_waiting_transition, WAITING);
  RUNNING->addTransition(&run_hold_transition, HOLD);
  HOLD->addTransition(&hold_unload_transition, UNLOAD);
  HOLD->addTransition(&hold_run_transition, RUNNING);

  UNLOAD->addTransition(&unload_post_unload_transition, POST_UNLOAD);

  UNLOAD->addTransition(&unload_run_transition, RUNNING);
  POST_UNLOAD->addTransition(&post_unload_run_transition, RUNNING);

  RUNNING->addTransition(&overload, STOP);
  UNLOAD->addTransition(&overload, STOP);

  STOP->addTransition(&stop_waiting_transition, WAITING);
}

#endif // STATE_MACHINEH