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
- [ ] 钟的框架。
- [ ] 时针旋转。
- [ ] 外部供电。

### 软件实现清单
- [x] 让esp8266连上网，并使用网络获取现在的时间。by @Reloin
- [ ] 电机的控制，电机可以转动时针以较准地显示现在时间。
- [ ] 自动校准系统。

## Features任务清单
- [ ] 电池供电。
- [ ] 连接OLED屏幕。
- [ ] 连接Joystick控制OLED屏幕。
- [ ] 连接蜂鸣器控制闹钟。
- [ ] LED灯光效果。
- [ ] 挥手停止闹钟。
