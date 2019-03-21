#ifndef KEYBORAD_H_
#define KEYBORAD_H_
#include "stm8s.h"
#include "delay.h"

#define KEYBORAD_PROT GPIOB
#define KEYBORAD_H_4_PIN  GPIO_PIN_0
#define KEYBORAD_H_3_PIN  GPIO_PIN_1
#define KEYBORAD_H_2_PIN  GPIO_PIN_2      
#define KEYBORAD_H_1_PIN  GPIO_PIN_3
#define KEYBORAD_L_2_PIN  GPIO_PIN_4
#define KEYBORAD_L_1_PIN  GPIO_PIN_5
#define KEYMASK         0XF0
#define KEYNULL         0X00
#define KEY_L_1_MASK    0X2F
#define KEY_L_2_MASK    0X1F
#define S1_DOWN_VALUE   0X27
#define S2_DOWN_VALUE   0X2B
#define S3_DOWN_VALUE   0X2D
#define S4_DOWN_VALUE   0X2E

#define S5_DOWN_VALUE   0X17
#define S6_DOWN_VALUE   0X1B
#define S7_DOWN_VALUE   0X1D
#define S8_DOWN_VALUE   0X1E
#define KEY_DELAY_MS    100
#define FAN_W_STATE           0X01
#define FAN_M_STATE           0X02
#define FAN_H_STATE           0X03
#define FAN_OFF_STATE         0X00
#define DOOR_UP_STATE   0X02 
#define DOOR_DO_STATE   0X01          

extern struct KEYHANDLE KeyHandle;
void KeyBorad_Hnadle(u8 KeyVaul);
u8 KeyBorad_Scan(void);
void KeyBorad_PinInit(void);
#endif