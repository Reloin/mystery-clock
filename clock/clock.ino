/*
 * 硬设大赛代码，由章鞃玲(组长)，黄鉦皓，梁翊恒共同编写
 */
#define TIMER_INTERRUPT_DEBUG      0

#include <ESP8266WiFi.h>//ESP9266连接WiFi的库
#include <NTPClient.h>//取得时间的库
#include <WiFiUdp.h>//进行UDP交流的库
#include "ESP8266TimerInterrupt.h"
#include "U8glib.h"
#include "TimerOne.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 


//此处输入wifi名和wifi密码
const char* ssid       = "your ssid";//wifi名
const char* password   = "YOUR_PWD";//wifi密码

#define interval 1000 //以ms为单位检查现在的时间


//建立NTP客户端接收时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");

ESP8266Timer ITimer;

int hour, minute,pos,idle=0,screen=0,alarm_status=0,alarm_h=0,alarm_m=0,b=1,flagh=0;
const int Ry=A0,button=D0;
unsigned long stime;
String temph, tempm;

void setup()
{
    Serial.begin(9600);
    pinMode(button,INPUT_PULLUP);
    u8g.setFont(u8g_font_unifont);
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

    if (ITimer.attachInterruptInterval(interval * 1000, checkTime))
    {
      Serial.print("Starting  ITimer OK, millis() = "); Serial.println(millis());
    }
    else
      Serial.println("Can't set ITimer. Select another freq. or timer");

}
void draw(int s) {
  if (s==0){
    screen=0;
    u8g.sleepOn();
  }
  if (s==1){
    screen=1;
    u8g.sleepOff();
    u8g.setPrintPos(0,20);
    u8g.print("weather");
  }
  if (s==2){
    screen=2;
    u8g.sleepOff();
    u8g.setPrintPos(0,20);
    u8g.print("alarm :");
    if(alarm_status==0){
      u8g.setPrintPos(70,20);
      u8g.print("OFF");
    }
    if(alarm_status==1){
      temph=String(alarm_h);
      tempm=String(alarm_m);
      u8g.setPrintPos(70,20);
      if (alarm_h<10){
        u8g.print("0");
        u8g.setPrintPos(78,20);
        u8g.print(temph);
      }
      else u8g.print(temph);
      u8g.setPrintPos(87,20);
      u8g.print(":");
      u8g.setPrintPos(94,20);
      if (alarm_m<10){
        u8g.print("0");
        u8g.setPrintPos(102,20);
        u8g.print(tempm);
      }
      else u8g.print(tempm);
    }
  }
  //set alarm status
  if (s>=3){
    screen=3;
    u8g.setPrintPos(0,10);
    u8g.print("set alarm:");
    u8g.setPrintPos(85,10);
    if(alarm_status==0)u8g.print("OFF");
    else u8g.print("ON");
    //set alarm hour and minute
    if (s==4){
    temph=String(alarm_h);
    tempm=String(alarm_m);
    u8g.setPrintPos(0,30);
    if (alarm_h<10){
      u8g.print("0");
      u8g.setPrintPos(8,30);
      u8g.print(temph);
      }
    else u8g.print(temph);
    u8g.setPrintPos(17,30);
    u8g.print(":");
    u8g.setPrintPos(24,30);
    if (alarm_m<10){
      u8g.print("0");
      u8g.setPrintPos(32,30);
      u8g.print(tempm);
      }
    else u8g.print(tempm);
    }
  }
}
int checkmove(void){
    int y0,y1;
    y0=504;
    y1=analogRead(Ry);
    if(y1<y0-100) return 1;//上
    else if(y1>y0+100) return 2; //下
    else return 0; //原位
}
void setalarm(){
  u8g.firstPage();  
  do {
      draw(3);
   } while( u8g.nextPage() );
}

void loop()
{
  pos=checkmove();
  //Sleep mode时移动rocker
  if ((pos!=0)&&(idle==0)){
     u8g.firstPage();  
  do {
    draw(1);
  } while( u8g.nextPage() );
  stime=millis();
  idle=1;
  }
  //屏幕亮起时移动rocker
  if ((pos!=0)&&(idle==1)&&(millis()-stime<30000)){
    if (screen==1){
      u8g.firstPage();  
      do {
          draw(2);
      } while( u8g.nextPage() );
      attachInterrupt(digitalPinToInterrupt(16),setalarm,LOW);
    }
    else if (screen==2){
       u8g.firstPage();  
       do {
          draw(1);
        } while( u8g.nextPage() );
    }
    else if (screen=3){
     detachInterrupt(digitalPinToInterrupt(16));
     do{
      if(pos!=0){
       if(alarm_status==0) alarm_status=1;
       else alarm_status=0;
       u8g.firstPage();  
       do {
          draw(3);
        } while( u8g.nextPage() );
        delay(500);
      }
     b=digitalRead(button);
     pos=checkmove();
     }while (b!=0);
     if(alarm_status==1){
      pos=checkmove();
      //set alarm hour
      if(flagh==0){
        delay(300);
        do{
       if(pos==1){
        alarm_h = alarm_h + 1;
        if(alarm_h > 23){alarm_h = 0;}
       }
       if(pos==2){
        alarm_h = alarm_h - 1;
        if(alarm_h < 0){alarm_h = 23;}
       }
       u8g.firstPage();  
       do {
          draw(4);
       } while( u8g.nextPage() );
       delay(100);
       b=digitalRead(button);
       pos=checkmove();
       }while(b!=0);
      }
      flagh=1;
      if(flagh=1){
       //set alarm minute
       delay(300);
      do{
       if(pos==1){
        alarm_m = alarm_m + 1;
        if(alarm_m > 59){alarm_m = 0;}
       }
       if(pos==2){
        alarm_m = alarm_m - 1;
        if(alarm_m < 0){alarm_m = 59;}
       }
       u8g.firstPage();  
       do {
          draw(4);
       } while( u8g.nextPage() );
       delay(100);
       pos=checkmove();
       b=digitalRead(button);
       }while(b!=0);
      }
     }
     u8g.firstPage();  
      do {
          draw(2);
        } while( u8g.nextPage() );
      flagh=0;
     }
  stime=millis();
  idle=1;
  delay(100);
  }
  //一段时间没移动后进入Sleep mode
  if ((idle==1)&&(millis()-stime>30000)){
     u8g.firstPage();  
  do {
    draw(0);
  } while( u8g.nextPage() );
  idle=0;
  }
}

void checkTime(){
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
}
