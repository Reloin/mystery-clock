/*
 * 硬设大赛代码，由章鞃玲(组长)，黄鉦皓，梁翊恒共同编写
 */
#define TIMER_INTERRUPT_DEBUG      0

#include <ESP8266WiFi.h>//ESP9266连接WiFi的库
#include <NTPClient.h>//取得时间的库
#include <WiFiUdp.h>//进行UDP交流的库
#include "ESP8266TimerInterrupt.h"


//此处输入wifi名和wifi密码
const char* ssid       = "Leong Home@unifi";//wifi名
const char* password   = "0129405519";//wifi密码

#define interval 1000 //以ms为单位检查现在的时间


//建立NTP客户端接收时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");

ESP8266Timer ITimer;

int hour, minute;

void setup()
{
    Serial.begin(9600);
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

void loop()
{
  
}

void checkTime(){
    timeClient.update();
    Serial.println(timeClient.getFormattedTime());
}
