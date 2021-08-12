#include "Stepper.h"
#include "ESP8266TimerInterrupt.h"
#define Stepper_pin1 D3
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6
const int stepsPerRevolution = 50;
Stepper Minhand(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);
ESP8266Timer ITimer;
void setup() {
  Minhand.setSpeed(60);
  ITimer.attachInterruptInterval(18000000, Isr);
}
void Isr(){
    Minhand.step(1);
}

void loop() {
}
