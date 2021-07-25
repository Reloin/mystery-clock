/*
 * 硬设大赛代码，由章鞃玲(组长)，黄鉦皓，梁翊恒共同编写
 */

#include <ESP8266WiFi.h>//ESP9266连接WiFi的库
#include <NTPClient.h>//取得时间的库
#include <WiFiUdp.h>//进行UDP交流的库
#include <TimerOne.h>//Time Interupt的库

//此处输入wifi名和wifi密码
const char* ssid       = "YOUR_SSID";//wifi名
const char* password   = "YOUR_PASS";//wifi密码

//建立NTP客户端接收时间
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org");

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
}

void loop()
{
    //
}

//检查时间的函数
void checkTime(){
    timeClient.update();
    timeClient.getHours();//取得int类的小时
    timeCleint.getMinustes();//取得int类的分钟
}
