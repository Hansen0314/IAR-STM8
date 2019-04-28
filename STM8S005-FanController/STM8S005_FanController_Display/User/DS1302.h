#ifndef DS1302_H
#define DS1302_H

#include "stm8s.h"
#include "delay.h"
//DS1302Ïà¹ØPIN
#define DS1302_PORT       GPIOC
#define DS1302_PORT_RST   GPIOC
#define DS1302_PORT_CLK   GPIOE
#define DS1302_RST_PIN  GPIO_PIN_2
#define DS1302_IO_PIN   GPIO_PIN_1
#define DS1302_CLK_PIN  GPIO_PIN_5

#define RESET 0
#define SET   1
#define CH    0

void Ds1302_Init();
struct ALLDATE ds1302_readTime( void );
u8 ds1302_readByte(u8 Addr);
#endif