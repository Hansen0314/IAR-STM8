#ifndef GPIO_H
#define GPIO_H
#include "stm8s.h"

#define BACK_LIGHT_PROT   GPIOD
#define BACK_LIGHT_PIN_1  GPIO_PIN_2
#define BACK_LIGHT_PIN_2  GPIO_PIN_3
#define BACK_LIGHT_PIN_3  GPIO_PIN_4      
#define BACK_LIGHT_PIN_4  GPIO_PIN_7        

           
void Gpio_Init(void);
void Back_Light_Off();
void Back_Light_On();
#endif