# A-human-checker-based-on-51MCU
课程作业，用51单片机实现的红外人体检测装置
## 文件说明
* LCD.h 用于lcd屏幕显示的驱动文件
* Sxy_046_Check.c main函数
## 说明
### 实现内容
1.利用人体红外感应模块检测人体，并通过指示灯指示。
2.用另一个LED灯代替楼道灯，当人体接近时开启楼道灯。
3.当人体离开后，楼道灯延时熄灭，延时时间可通过按键设备
4.延时时间实时倒数显示，单位为秒。
5.增加声音感应功能。
6.白天不感应。
## 总体设计方案
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/e6adf254-7bfe-4c08-bc57-46005dc7351b)
本系统由单片机开发板、光线感应模块、声音感应模块和人体感应模块组成。其中用到单片机开发板上的按键模块、LED灯模块和LCD显示模块。通过光线感应模块判断是白天还是黑夜，若为黑夜则进入工作状态。默认工作状态时候，此时可以通过按键设置倒计时的时长。当声音和人体感应模块触发，代表路灯的灯亮起，人离开后，LCD屏幕显示倒计时，倒计时结束后回到默认工作状态。
## 硬件
1、LCD1602
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/054f6587-c993-4354-8019-f2fb47cee443)
2、LED模块和案件模块
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/a33860df-fcc6-4d85-ae14-860a390e87c4)
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/466b3222-d63b-4759-a60c-5ed14cfc1fa9)
3、人体感应模块
人体感应模块可以实现全自动感应：人进入其感应范围则输入高电平，人离开感应范围则自动延时关闭高电平，输出低电平。
它又有两种触发模式：
i.	不可重复触发方式：即感应输出高电平后，延时时间段已结束，输出将自动从高电平变成低电平；
ii.	可重复触发方式：即感应输出高电平后，在延时时间段内，如果有人体在其感应范围活动，其输出一直保持高电平，直到人离开后才演示将高电平变为低电平。

![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/0dab00a4-03dd-496f-9051-163105a04717)
4、光敏电阻模块
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/554a46c9-ef68-434d-bedf-1359b7c0c38d)
光敏电阻模块，当有光线时输出低电平，无光线时输出高电平。将输出脚连接P3.2INT0口请求外部中断。
5、声音检测模块
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/7b1a2931-cc25-400b-84a3-f5f6d5467f4a)
## 软件
1、框图
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/739d0d4c-515a-4199-8962-9c2c77b95fe3)
软件部分主要有5部分组成：主函数，定时器中断，外部中断0，按键控制和LCD显示。上电后系统初始化，包括设置LED灯初始状态，开外部中断，配置定时器并开定时器中断和LCD初始化。初始化完成立即进入主循环，此时若为白天，CPU立即响应外部中断请求在中断服务程序等待直到天黑，天黑后退出中断继续进入主循环。如果有人，则路灯点亮，人体传感器持续检测人体，当人离开后进入倒计时，根据定时器获得的1s定时依此递减直至为零后熄灭路灯回到主循环。同样，如果有声音，则直接进入倒计时，倒计时结束后回到查询循环。同时，每个状态都在LCD屏幕显示一次当前状态和倒计时。
2、定时器中断
定时器中断采用模式1，赋初值19456。考虑到晶振为11.0592MHz，有：
 ![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/2af9dee4-1ebc-4c89-ad17-bde98dea50f0)
所以，定时器赋初值19456，进中断20次后获得1s定时，即计数变量为20时满1s。中断服务程序中设置计数变量，每次进终端后计数变量加一，当计数满20且倒计时没有到0时候，若有人离开或者有声音则倒计时变量减一。倒计时到0后灭掉路灯。
3、按键控制
考虑到本系统只需要控制倒计时时长，简单采用两个独立按键步进控制。按下按键K1倒计时加一，按下按键K2倒计时减一。
4、LCD显示
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/5e469ee7-b313-486a-b04d-54a05830b78d)
## 照片
![image](https://github.com/AcidFish12/A-human-checker-based-on-51MCU/assets/75008732/c109b89b-a4eb-4f72-b2b1-ef7d1ffddbf3)






