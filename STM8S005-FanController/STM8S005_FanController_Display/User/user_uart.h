#ifndef __UART_H
#define __UART_H

#include <stdio.h>
#include "stm8s.h"
#define FAN_OFF     0X01
#define FAN_ON_L    0X02
#define FAN_ON_M    0X03
#define FAN_ON_B    0X04
#define LED_P1_ON   0X05
#define LED_P1_OFF  0X06
#define LED_P2_ON   0X07
#define LED_P2_OFF  0X08
#define DOOR_UP_ON  0X09
#define DOOR_DO_ON  0X0B
#define DOOR_OFF   0x0c
#define ER_ON       0X0D
#define ER_OFF     0X0E
#define DP_HIGH     0X0F
#define DP_LOW     0X10
#define FR_HIGH    0X11
#define FR_LOW     0X12
#define A11_VALUE   0X13
#define A12_VALUE   0X14
#define A13_VALUE   0X15
void uart2Init();
void uart2SendByte(uint8_t data);
void uart2SendString(uint8_t* Data,uint16_t len);
uint8_t uart2ReceiveByte(void);
void Uart_IT_Receive_Hnadle(u8 data);
extern struct Peripheral Peripheral_Realy;
void Uart_Transmit_Hnadle(struct KEYHANDLE KeyHandle);
#endif