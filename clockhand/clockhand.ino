#include "Stepper.h"
#include "ESP8266TimerInterrupt.h"


//Define pins for stepper motor
#define Stepper_pin1 D3
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6

//Declare clock stepper motor
const int stepsPerRevolution = 50;
Stepper motor(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);
ESP8266Timer moveClock;

void setup() {
  motor.setSpeed(60);
  //clock will move for every 18s, as 1 complete revolution is 200 steps
  moveClock.attachInterruptInterval(18000000, Isr);
}
void Isr(){
    motor.step(1);
}

void loop() {
}
