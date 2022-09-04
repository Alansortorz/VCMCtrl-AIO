# VCMCtrl-AIO - 直线电机的一体化闭环驱动
![image-20220904195708891](5.Images/VCM.png)

> 这里我设计的是一个音圈电机（Voice Coil Motor）的一体化闭环驱动。

## 1  驱动硬件说明

### 1.1  接口说明 

![HardwareInterface](5.Images/HardwareInterface.png)

### 1.2  引脚配置图

![Pinpoint](5.Images/Pinpoint.png)

### 1.3  传感器说明

| 序号 |     传感器     | 说明 |
| :--: | :------------: | :--: |
|  1   |   光栅传感器   |      |
|  2   | 姿态传感器IMU  |      |
|  3   | 电流采样传感器 |      |





## 关于串口助手

串口助手软件种类很多，这里推荐以下三种：

- 野火多功能串口助手 - 只能显示数据，特点是可用于PID调参，图像处理，网络信号调试等。
- seialplot - 只能显示曲线，特点是图像可伸缩。
- SerialChart - 即可显示数据，也可显示曲线。

SerialChart配置文件详解

```shell
[_setup_]
port=COM6 #串口号
baudrate=115200 #串口波特率

width=1900 #下面曲线显示栏宽度
height=400 #下面曲线显示栏高度
background_color = white #背景颜色

grid_h_origin = 200
grid_h_step = 10
grid_h_color = #EEE
grid_h_origin_color = #AAA
grid_v_origin = 0
grid_v_step = 10
grid_v_color = #EEE
grid_v_origin_color = #CCC 

[_default_]
min=-5 #下面图象显示栏的下界
max=15 #下面图像显示栏的上界

[ch1]
color = red #通道1曲线

[ch2]
color = blue #通道2曲线
#printf函数输出的数据之间用`,`分割。
```



















