MUST READ
==========
切记配置drv_uart.h里的错误输出端口 默认串口1
切记优先初始化drv_uart里的错误输出串口











V0.1
====
傻逼pn532












#20-11-09 00:25 （20-11-22 17:22）
加了PWM 试了半天 真的吃屎 加了C6x0 已测试
pwm为4和14

#20-11-10 21:51
加了遥控器和robomodule的外围 做大创用的

#20-11-22 17:22
加了key和beep 未测试 删了OBJ里的.o
修订了timer

#20-11-24 22:37
修订了uart 加了陀螺仪解包

#20-11-26 22:28
修订了joystick

Pin Config
==========




|Function|Pin|
|---|----|
|U2_RX|PA2|
|U2_TX|PA3|
|SPI1_NSS|PA4|
|SPI1_SCK(SCK)|PA5|
|SPI1_MOSI|PA6|
|SPI1_MISO/pwm14_ch1|PA7|
|U1_RX|PA9|
|U1_TX|PA10|
|CAN1_RX|PA11|
|CAN1_TX|PA12|
|SWDIO|PA13|
|SWCLK|PA14|
|NONE|PA15|
|||
|||
|||
|RFID_IRQ|PB1|
|BOOT1|PB2|
|NONE|PB3|
|NONE|PB4|
|CAN2_RX|PB5|
|CAN2_TX|PB6|
|RFID_Reset/pwm4_ch2|PB7|
|I2C2_SCL|PB10|
|I2C2_SDA|PB11|
|||
|||
|||
|LED0|PC0|
|LED1|PC1|
|LED2|PC2|
|LED3|PC3|
|KEY0|PC4|
|KEY1|PC5|
|U6_TX|PC6|
|U6_RX|PC7|
|||
|||
|||
|BEEP|PD0|
|U3_TX(232)|PD8|
|U3_RX(232)|PD9|
|||
|||
|||
|||
|||
|||
