# 红外循迹小车 #
## 引脚分配 ##
### GPIOA ###
*PA0* **TIM2_CH1** 输出PWM波

*PA1* **TIM2_CH1** 输出PWM波

*PA2* **TIM2_CH3** 

*PA3* **TIM2_CH4** 

*PA4* 电机方向控制

*PA5* 电机方向控制

*PA6* **TIM3_CH1** 测速1

*PA7* **TIM3_CH2** 测速1

*PA8*

*PA9* **USART1_TX** 串口通信

*PA10* **USART1_RX** 串口通信

*PA11*

*PA12* 红外传感器1

*PA13*

*PA14*

*PA15* 红外传感器2

### GPIOB ###
*PB0* 电机方向控制

*PB1* 电机方向控制

*PB2*

*PB3* 红外传感器3

*PB4* 红外传感器4

*PB5* 红外传感器5

*PB6* **TIM4_CH1** 测速2

*PB7* **TIM4_CH2** 测速2

*PB8*

*PB9*

*PB10* **OLED_SDA**

*PB11* **OLED_SCL**

*PB12* **KEY_1** 上/数值增加

*PB13* **KEY_2** 下/数值减少

*PB14* **KEY_3** 确定/编辑/退出编辑

*PB15* **KEY_4** 取消/返回上一级（并退出编辑）

### GPIOC ###
*PC13*

*PC14*

*PC15*

## 计时器分配 ##

**TIM1** 高级定时器，定时中断

**TIM2** 输出PWM波

**TIM3** 对电机1测速

**TIM4** 对电机2测速
