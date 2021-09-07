#include "Stepper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>//ESP9266连接WiFi的库
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Define pins for stepper motor and touch pole
#define Stepper_pin1 D0
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6
#define Touch_pole D8

//此处输入wifi名和wifi密码
const char* ssid       = "Leong Home@unifi";//wifi名
const char* password   = "0129405519";//wifi密码

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
const int stepsPerRevolution = 50;
int h,m,set_duration_min,set_duration_min2,set_duration_sec,step_need;
Stepper motor(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);

void setup() {
  pinMode(Touch_pole, INPUT);

  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to Wifi...");
    }
    
  //初始化NTP客户端
  timeClient.begin();
  //计算我们时域的时间
  timeClient.setTimeOffset(28800);
  
  Serial.begin(9600);
  Clockset();
}

void Clockset(){
  motor.setSpeed(50);
  while(!digitalRead(Touch_pole)){
    motor.step(1); //move to 00:00
  }
  timeClient.update();
  h=timeClient.getHours();
  m=timeClient.getMinutes();
  Serial.printf("time: %d : %d \n", h, m);
  h=h%12;
  if(h<6||(h==6&&m==0)){
    motor.setSpeed(60);
    m+=(h*60);
    set_duration_min=m/45; //every 45mins move of minute hand consume 3min=10steps(no error)
    set_duration_min2=(m%45)/15; 
    if (set_duration_min2==0) set_duration_min2=0;
    else if (set_duration_min2==1) set_duration_min2=3; //every first 15mins move of minute hand consume 3steps (6seconds error)
    else if(set_duration_min2==2) set_duration_min2=7; //every second 15mins move of minute hand consume 7stepsv(6 seconds error) 
    
    set_duration_sec=m%15;
    if(set_duration_sec<5) set_duration_sec=1;
    else if(set_duration_sec==5&&set_duration_sec<10) set_duration_sec=2;
    else if(set_duration_sec==10&&set_duration_sec<15) set_duration_sec=3;
    
    if(m%3==0) step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec;
    else if(m%3==1) step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec+3;
    else step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec+7;  
    
  }
  else{
    motor.setSpeed(60)
    h-=6;
    m=360-(h*60+m);
    set_duration_min=m/45; //every 45mins move of minute hand consume 3min=10steps(no error)
    set_duration_min2=(m%45)/15; 
    if (set_duration_min2==0) set_duration_min2=0;
    else if (set_duration_min2==1) set_duration_min2=3; //every first 15mins move of minute hand consume 3steps (6seconds error)
    else if(set_duration_min2==2) set_duration_min2=7; //every second 15mins move of minute hand consume 7stepsv(6 seconds error) 
    
    set_duration_sec=m%15;
    if(set_duration_sec<5) set_duration_sec=1;
    else if(set_duration_sec==5&&set_duration_sec<10) set_duration_sec=2;
    else if(set_duration_sec==10&&set_duration_sec<15) set_duration_sec=3;
    
    if(m%3==0) step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec;
    else if(m%3==1) step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec+3;
    else step_need=(m/3*10)+set_duration_min*10+set_duration_min2+set_duration_sec+7;
    step_need=-step_need; //reverse
  }
  motor.step(step_need);
}

void loop() {
}
