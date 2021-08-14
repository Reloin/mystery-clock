# mystery-clock
硬设的代码都放这里

## Git的基本指令
可以用`git commit -m "message here"`推去自己电脑备份

`git push`将已commit的代码推去Github，我们都看得到

`git pull`将github上别人push的代码拉来自己的电脑

## 使用的库
* ESP8266
* NTPClient

## Function任务清单(MVP)

### 硬件实现清单
- [x] 钟的框架。
- [x] 时针旋转。
- [x] 外部供电。

### 软件实现清单
- [x] 让esp8266连上网，并使用网络获取现在的时间。by @Reloin
- [x] 电机的控制，电机可以转动时针以较准地显示现在时间。
- [ ] 自动校准系统。

## Features任务清单
- [x] 电池供电。
- [x] 连接OLED屏幕。
- [x] 连接Joystick控制OLED屏幕。
- [x] 获取并显示当天天气。
- [x] 闹钟功能。
- [x] 连接扬声器。
- [x] LED灯光效果。
- [ ] 挥手停止闹钟。
