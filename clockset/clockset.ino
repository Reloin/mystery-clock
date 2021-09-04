#include "Stepper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

//Define pins for stepper motor and touch pole
#define Stepper_pin1 D3
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6
#define Touch_pole D7

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
const int stepsPerRevolution = 50;
int hour,minute,set_duration_min,set_duration_sec,step_need;
Stepper motor(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);
ESP8266Timer moveClock;

void setup() {
  
}

void Clockset(){
  while(digitalRead(Touch_pole)!=1){
    motor.step(1); //move to 00:00
  }
  timeClient.update();
  hour=timeClient.gerHours();
  minute=timeClient.getMinutes();
  if(hour<6||(hour==6&&minute==0)){
    motor.setSpeed(120);
    minute+=(hour*60);
    set_duration_min=minute/30; //every 30mins move of minute hand consume 1 min
    set_duration_sec=minute%30;
    if(set_duration_sec<10) set_duration_sec=1;
    else if(set_duration_sec==10&&set_duration_sec<20) set_duration_sec=2;
    else if(set_duration_sec==20&&set_duration_sec<30) set_duration_sec=3;
    
    if(minute%3==0) step_need=(minute/3*10)+set_duration_min*3+set_duration_sec;
    else if(minute%3==1) step_need=(minute/3*10)+set_duration_min*3+set_duration_sec+3;
    else step_need=(minute/3*10)+set_duration_min*3+set_duration_sec+7;  
    
  }
  else{
    motor.setSpeed(-120); //reverse
    hour-=6;
    minute=360-(hour*60+minute);
    set_duration_min=minute/30; //every 30mins move of minute hand consume 1 min
    set_duration_sec=minute%30;
    if(set_duration_sec<10) set_duration_sec=1;
    else if(set_duration_sec==10&&set_duration_sec<20) set_duration_sec=2;
    else if(set_duration_sec==20&&set_duration_sec<30) set_duration_sec=3;
    
    if(minute%3==0) step_need=(minute/3*10)+set_duration_min*3+set_duration_sec;
    else if(minute%3==1) step_need=(minute/3*10)+set_duration_min*3+set_duration_sec+3;
    else step_need=(minute/3*10)+set_duration_min*3+set_duration_sec+7;  
  }
  motor.step(step_need);
}

void loop() {
}
