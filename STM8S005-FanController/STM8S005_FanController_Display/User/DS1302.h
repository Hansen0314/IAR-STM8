#ifndef DS1302_H
#define DS1302_H

#include "stm8s.h"
#include "delay.h"

#define DS1302_IIC_SDA_PROT GPIOC
#define DS1302_IIC_SDA_PIN GPIO_PIN_1
#define DS1302_IIC_SCL_PROT GPIOE
#define DS1302_IIC_SCL_PIN GPIO_PIN_5
#define RESET 0
#define SET   1
#define CH    0

void Ds1302_Init();
struct ALLDATE ds1302_readTime( void );
u8 ds1302_readByte(u8 Addr);
#endif