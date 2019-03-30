#ifndef GPIO_H
#define GPIO_H
#include "stm8s.h"

#define FAN_PORT   GPIOB
#define FAN_L_PIN  GPIO_PIN_0
#define FAN_M_PIN  GPIO_PIN_1
#define FAN_B_PIN  GPIO_PIN_2      
          
#define LED_PORT   GPIOD
#define LED_P1_PIN  GPIO_PIN_3       //照明灯
#define LED_P2_PIN  GPIO_PIN_4       //杀菌灯

#define DOOR_UP_PORT     GPIOD   
#define DOOR_UP_PIN      GPIO_PIN_7   //门上升

#define DOOR_DO_PORT     GPIOF
#define DOOR_DO_PIN      GPIO_PIN_4   //门下降

#define ER_PORT          GPIOC
#define ER_PIN           GPIO_PIN_2    //故障

#define DP_PORT          GPIOC
#define DP_PIN           GPIO_PIN_4    //压差（输入）

#define FR_PORT          GPIOC
#define FR_PIN           GPIO_PIN_3    //风机（输入）

#define DOOR_UP_IN_PORT     GPIOD   
#define DOOR_UP_IN_PIN      GPIO_PIN_0   //门上升

#define DOOR_DO_IN_PORT     GPIOE
#define DOOR_DO_IN_PIN      GPIO_PIN_5   //门下降      

void Gpio_Init(void);

#endif