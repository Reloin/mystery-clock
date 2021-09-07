#include "Stepper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

//Define pins for stepper motor and touch pole
#define Stepper_pin1 D0
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6
#define Touch_pole D8

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
const int stepsPerRevolution = 50;
int h,m,set_duration_min,set_duration_sec,step_need;
Stepper motor(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);

void setup() {
  pinMode(Touch_pole, INPUT);
  Clockset();
}

void Clockset(){
  motor.setSpeed(120);
  while(!digitalRead(Touch_pole)){
    motor.step(1); //move to 00:00
  }
  timeClient.update();
  h=timeClient.getHours();
  m=timeClient.getMinutes();
  h=h%12;
  if(h<6||(h==6&&m==0)){
    motor.setSpeed(120);
    m+=(h*60);
    set_duration_min=m/30; //every 30mins move of minute hand consume 1 min
    set_duration_sec=m%30;
    if(set_duration_sec<10) set_duration_sec=1;
    else if(set_duration_sec==10&&set_duration_sec<20) set_duration_sec=2;
    else if(set_duration_sec==20&&set_duration_sec<30) set_duration_sec=3;
    
    if(m%3==0) step_need=(m/3*10)+set_duration_min*3+set_duration_sec;
    else if(m%3==1) step_need=(m/3*10)+set_duration_min*3+set_duration_sec+3;
    else step_need=(m/3*10)+set_duration_min*3+set_duration_sec+7;  
    
  }
  else{
    motor.setSpeed(-120); //reverse
    h-=6;
    m=360-(h*60+m);
    set_duration_min=m/30; //every 30mins move of minute hand consume 1 min
    set_duration_sec=m%30;
    if(set_duration_sec<10) set_duration_sec=1;
    else if(set_duration_sec==10&&set_duration_sec<20) set_duration_sec=2;
    else if(set_duration_sec==20&&set_duration_sec<30) set_duration_sec=3;
    
    if(m%3==0) step_need=(m/3*10)+set_duration_min*3+set_duration_sec;
    else if(m%3==1) step_need=(m/3*10)+set_duration_min*3+set_duration_sec+3;
    else step_need=(m/3*10)+set_duration_min*3+set_duration_sec+7;  
  }
  motor.step(step_need);
}

void loop() {
}
