/*
 * 硬设大赛代码，由章鞃玲(组长)，黄鉦皓，梁翊恒共同编写
 */
#define TIMER_INTERRUPT_DEBUG      0
#include <ESP8266WiFi.h>//ESP9266连接WiFi的库
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <NTPClient.h>//取得时间的库
#include <WiFiUdp.h>//进行UDP交流的库
#include "Stepper.h"
#include "ESP8266TimerInterrupt.h"
#include <Arduino.h>
#include <U8g2lib.h> //OLED显示的库
#include <Wire.h>
#include <Ticker.h>

Ticker ticker;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,3,5,4);  //scl=5,sda=4

//此处输入wifi名和wifi密码
const char* ssid       = "Leong Home@unifi";//wifi名
const char* password   = "0129405519";//wifi密码

#define interval 1000 //以ms为单位检查现在的时间

//Define pins for stepper motor
#define Stepper_pin1 D0
#define Stepper_pin2 D4
#define Stepper_pin3 D5
#define Stepper_pin4 D6
#define Touch_pole D8

//Declare clock stepper motor
const int stepsPerRevolution = 50;
Stepper motor(stepsPerRevolution,Stepper_pin1,Stepper_pin2,Stepper_pin3,Stepper_pin4);
ESP8266Timer moveClock;

//建立NTP客户端接收时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");
//天气API,国家与城市 (例：雪兰莪，马来西亚)
String openWeatherMapApiKey = "c2a6791b4af4bce61b6829490ac78df8";
String city= "Selangor";
String countryCode="MY";
unsigned long lastTime = 0;
unsigned long timerDelay = 15*60*1000; //检查天气频率(ms)(15分钟)
String jsonBuffer;


int hour, minute,temp,degree,pos,idle=0,screen=0,alarm_status=0,alarm_h=0,alarm_m=0,b=1,flagh=0;
String weather,temph, tempm;
uint8_t symbol,t_symbol;
const int Ry=A0,button=13,beep=0;
unsigned long stime,timebudget;
static bool powerSave=false;



#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4
#define SNOW 5
#define MIST 6
#define MOON 7
#define WIFI 8
#define ALARM 9
#define UPDATE 10

uint8_t getSymbol(String weather_icon)
{
  // clear sky
  if (weather_icon == "01d"){
    triggerMelody(1);
    return SUN;
    }
  if (weather_icon == "01n")
    return MOON;
  //few clouds
  if (weather_icon == "02d" || weather_icon == "03d" || weather_icon == "04d"){
    triggerMelody(1);
    return SUN_CLOUD;
    }
  //broken clouds
  if (weather_icon=="02n" || weather_icon == "03n" || weather_icon == "04n"){
    triggerMelody(2);
    return CLOUD;
    }
  //shower rain
  if (weather_icon == "10d" || weather_icon == "10n")
    return RAIN;
  //rain
  if (weather_icon == "09d" || weather_icon == "09n"){
    //signal attiny to play melody
    triggerMelody(3);
    return RAIN;
    }
  //thunderstorm
  if (weather_icon == "11d" || weather_icon == "11n")
    return THUNDER; //雷
  //snow
  if (weather_icon == "13d" || weather_icon == "13n")
    return SNOW;
  //mist
  if (weather_icon == "50d" || weather_icon == "50n")
    return MIST; //雾
}

void triggerMelody(int t)
{
  digitalWrite(beep, HIGH);
  delayMicroseconds(t * 100 + 50);// remove last 2 digits, corresp to 10
  digitalWrite(beep,LOW);
}

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
  // fonts used:
  // u8g2_font_open_iconic_embedded_6x_t
  // u8g2_font_open_iconic_weather_6x_t
  // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
  
  switch(symbol)
  {
    case SUN:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 69);  
      break;
    case SUN_CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 65); 
      break;
    case CLOUD:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 64); 
      break;
    case RAIN:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 67); 
      break;
    case THUNDER:
      u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
      u8g2.drawGlyph(x, y, 67);
      break;
     case SNOW:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 68);
      break;
     case MIST:
      u8g2.setFont(u8g2_font_open_iconic_text_4x_t);
      u8g2.drawGlyph(x, y, 64);
      break;
     case MOON:
      u8g2.setFont(u8g2_font_open_iconic_weather_4x_t);
      u8g2.drawGlyph(x, y, 66);
      break;     
     case WIFI:
      u8g2.setFont(u8g2_font_open_iconic_www_4x_t);
      u8g2.drawGlyph(x, y, 81);
      break;
     case ALARM:
      u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
      u8g2.drawGlyph(x, y, 65);
      break;
     case UPDATE:
      u8g2.setFont(u8g2_font_open_iconic_embedded_4x_t);
      u8g2.drawGlyph(x, y, 79);
      break;
  }
}

void drawUpdate(){
  t_symbol=UPDATE;
  drawWeatherSymbol(55,40,t_symbol);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(25, 55);
  u8g2.print("UPDATE?");
}
void drawUpdate2(){
  t_symbol=UPDATE;
  drawWeatherSymbol(55,40,t_symbol);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(25, 55);
  u8g2.print("UPDATING...");
}

void drawConnection(){
  t_symbol=WIFI;
  drawWeatherSymbol(55,40,t_symbol);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(25, 55);
  u8g2.print("Connecting...");
}

void drawAlarmON(){
  t_symbol=ALARM;
  drawWeatherSymbol(55,40,t_symbol);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(0, 55);
  u8g2.print("Hold button to stop");
}


void drawWeather()
{
  drawWeatherSymbol(3, 50, symbol);
  u8g2.setFont(u8g2_font_logisoso22_tf);
  u8g2.setCursor(48, 50);
  u8g2.print(degree);
  u8g2.print("°C");    // requires enableUTF8Print()
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(0,13);
  u8g2.print(weather);
}

void drawAlarm()
{
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(0,13);
  u8g2.print("Alarm");
  u8g2.setFont(u8g2_font_logisoso22_tf);
  //scrolling text press button to set alarm
  if(alarm_status==0){
    u8g2.setCursor(0,43);
    u8g2.print("OFF");
    }
   if(alarm_status==1){
    u8g2.setCursor(7,43);
    temph=String(alarm_h);
    tempm=String(alarm_m);
    if (alarm_h<10){
        u8g2.print("0");
        u8g2.setCursor(21,43);
        u8g2.print(temph);
      }
      else u8g2.print(temph);
      u8g2.setCursor(55,43);
      u8g2.print(":");
      u8g2.setCursor(83,43);
      if (alarm_m<10){
        u8g2.print("0");
        u8g2.setCursor(97,43);
        u8g2.print(tempm);
      }
      else u8g2.print(tempm);
    }
}

void drawSetAlarmStatus()
{
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(0,13);
  u8g2.print("Set alarm status:");
  u8g2.setFont(u8g2_font_logisoso22_tf);
  u8g2.setCursor(0,43);
  if(alarm_status==0)u8g2.print("OFF");
  else u8g2.print("ON");
}
void drawSetAlarm()
{
  temph=String(alarm_h);
  tempm=String(alarm_m);
  u8g2.setFont(u8g2_font_shylock_nbp_tf);
  u8g2.setCursor(0,13);
  u8g2.print("Set hour and minute:");
  u8g2.setFont(u8g2_font_logisoso22_tf);
  u8g2.setCursor(7,43);
  if (alarm_h<10){
      u8g2.print("0");
      u8g2.setCursor(21,43);
      u8g2.print(temph);
  }
  else u8g2.print(temph);
  u8g2.setCursor(55,43);
  u8g2.print(":");
  u8g2.setCursor(83,43);
  if (alarm_m<10){
       u8g2.print("0");
       u8g2.setCursor(97,43);
       u8g2.print(tempm);
   }
   else u8g2.print(tempm);
}
void draw(int s)
{ 
  u8g2.clearBuffer();          // clear the internal memory
  switch(s){
    case 0:
     screen=0;
     u8g2.setPowerSave(1);
     break;
    case 1:
     screen=1;
     u8g2.setPowerSave(0);
     drawWeather();    // draw the icon and degree only once
     break;
    case 2:
     screen=2;
     u8g2.setPowerSave(0);
     drawAlarm();
     break;
    case 3:
     screen=3;
     u8g2.setPowerSave(0);
     drawSetAlarmStatus();
     break;
    case 4:
     drawSetAlarm();
     break;
    case 5:
     drawConnection();
     break;
    case 6:
     u8g2.setPowerSave(0);
     drawAlarmON();
     break;
    case 7:
     screen=4;
     drawUpdate();
     break;
    case 8:
     screen=5;
     drawUpdate2();
     break;
     
  }
  u8g2.sendBuffer();        // transfer internal memory to the display
}

int checkmove(void){
    int y0,y1;
    y0=504;
    y1=analogRead(Ry);
    if(y1<y0-100) return 1;//上
    else if(y1>y0+100) return 2; //下
    else return 0; //原位
}
int checkTime(){
    timeClient.update();
    hour=timeClient.getHours();
    minute=timeClient.getMinutes();
    if(alarm_h==hour&&alarm_m==minute){
      stime=millis();
      draw(6);
      return 1;
    }
    else return 0;
}
void ICACHE_RAM_ATTR setalarm(){
  draw(3);
}
void ICACHE_RAM_ATTR updatetime(){
  draw(8);
}
void alarm(){
  triggerMelody(10);
  delay(1000);
  b=digitalRead(button);
}

void Isr(){
    motor.step(1);
}

//从OpenWeatherMap获取数据
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
void setup()
{
    Serial.begin(9600);
    pinMode(button,INPUT_PULLUP);
    pinMode(beep,OUTPUT);
    pinMode(Touch_pole, INPUT);
    u8g2.begin();  
    u8g2.enableUTF8Print();
    motor.setSpeed(60);
    //clock will move for every 18s, as 1 complete revolution is 200 steps
    moveClock.attachInterruptInterval(18000000, Isr);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to Wifi...");
        draw(5);
    }
    
    //初始化NTP客户端
    timeClient.begin();
    //计算我们时域的时间
    timeClient.setTimeOffset(28800);

}
void loop() {
  // Send an HTTP GET request
  if (((millis()-lastTime) > timerDelay)||lastTime==0) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      //存信息
      const char* weatherdescription = myObject["weather"][0]["description"];
      const char* weathericon = myObject["weather"][0]["icon"];
      String weather_icon=String(weathericon);
      temp = myObject["main"]["temp"];
      degree= temp-273;
      symbol = getSymbol(weather_icon);
      weather=String(weatherdescription);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    if(lastTime==0){
      draw(1);
      idle=1;
    }
    lastTime = millis();
  }
  //屏幕运行
  else{
  if(alarm_status==1){
    if(checkTime()==1){
    detachInterrupt(13);
    do{
      b=digitalRead(button);
      alarm();
      yield();
    }while((b!=0)&&(millis()-stime<60*1000));//闹钟响一分钟后或者Hold button关闭
    alarm_status=0;
    draw(1);
    }
  }
  pos=checkmove();
  if ((pos!=0)&&(idle==0)){
  draw(1);
  stime=millis();
  idle=1;
  }
  //运行校对
  if (screen==5){
       //校对函数
        int set_duration_min,set_duration_sec;
        int step_need = 0;
        Serial.println("Move to 12");
        while(!digitalRead(Touch_pole)){
          motor.step(1); //move to 00:00
          yield();
        }
        Serial.println("Getting time");
        timeClient.update();
        hour = timeClient.getHours() % 12;
        minute = timeClient.getMinutes();
        Serial.printf("time: %d : %d \n", hour, minute);
        if(hour<6||(hour==6&&minute==0)){
          minute+=(hour*60);
          set_duration_min=minute/15; //every 15mins move of minute hand consume 1 min
          set_duration_sec=minute%15;
          if(set_duration_sec<5) set_duration_sec=1;
          else if(set_duration_sec==5&&set_duration_sec<10) set_duration_sec=2;
          else if(set_duration_sec==10&&set_duration_sec<15) set_duration_sec=3;
          
          if(minute%3==0) step_need=(minute/3*10);
          else if(minute%3==1) step_need=(minute/3*10)+3;
          else step_need=(minute/3*10)+7;  
        }
        else{
          hour-=6;
          minute=360-(hour*60+minute);
          set_duration_min=minute/15; //every 15mins move of minute hand consume 1 min
          set_duration_sec=minute%15;
          if(set_duration_sec<5) set_duration_sec=1;
          else if(set_duration_sec==5&&set_duration_sec<10) set_duration_sec=2;
          else if(set_duration_sec==10&&set_duration_sec<15) set_duration_sec=3;
          
          if(minute%3==0) step_need=(minute/3*10);
          else if(minute%3==1) step_need=(minute/3*10)+3;
          else step_need=(minute/3*10)+7; 
          step_need=-step_need; //reverse
        }
        Serial.printf("Steps needed: %d\n", step_need);
        if(step_need>0){
        while(step_need != 0){
          motor.step(1);
          step_need--;
          yield();
        }
        }
        else{
          while(step_need != 0){
          motor.step(-1);
          step_need++;
          yield();
        }
        }
        draw(7);//放最后一行
  }
  //屏幕亮起时移动rocker
  if ((pos!=0)&&(idle==1)&&(millis()-stime<120*1000)){
    if (screen==1){
      if(pos==1){draw(7);attachInterrupt(digitalPinToInterrupt(13),updatetime,FALLING);}
      else if(pos==2){
      draw(2);
      attachInterrupt(digitalPinToInterrupt(13),setalarm,FALLING);}
    }
     else if (screen==2){
      detachInterrupt(13);
      if(pos==1)draw(1);
      else if (pos==2){draw(7);attachInterrupt(digitalPinToInterrupt(13),updatetime,FALLING);}
    }
    else if(screen==4){
      detachInterrupt(13);
      if(pos==1){
      draw(2);
      attachInterrupt(digitalPinToInterrupt(13),setalarm,FALLING);}
      else if(pos==2)draw(1);
    }
    else if (screen==3){
     detachInterrupt(13);
     do{
      if(pos!=0){
       if(alarm_status==0) alarm_status=1;
      else alarm_status=0;
       draw(3);
       delay(500);
      }
     b=digitalRead(button);
     pos=checkmove();
     }while (b!=0);
     delay(300);
     if(alarm_status==1){
      pos=checkmove();
      //set alarm hour
      if(flagh==0){
        do{
       if(pos==1){
        alarm_h = alarm_h + 1;
        if(alarm_h > 23){alarm_h = 0;}
        }
       if(pos==2){
        alarm_h = alarm_h - 1;
        if(alarm_h < 0){alarm_h = 23;}
       }
       draw(4);
       delay(100);
       b=digitalRead(button);
       pos=checkmove();
       }while(b!=0);
      }
      flagh=1;
      delay(300);
      if(flagh=1){
       //set alarm minute
     do{
       if(pos==1){
        alarm_m = alarm_m + 1;
       if(alarm_m > 59){alarm_m = 0;}
       }
       if(pos==2){
        alarm_m = alarm_m - 1;
        if(alarm_m < 0){alarm_m = 59;}
       }
       draw(4);
       delay(100);
       pos=checkmove();
       b=digitalRead(button);
      }while(b!=0);
      }
     }
      draw(2);
      flagh=0;
     }

  stime=millis();
  idle=1;
  delay(300);
  }
  //一段时间没移动后进入Sleep mode
  if ((idle==1)&&(millis()-stime>120*1000)){//两分钟进入睡眠模式
    draw(0);
    idle=0;
  }
  }

}
