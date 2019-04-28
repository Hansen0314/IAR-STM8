
#ifndef _HT1621_H
#define _HT1621_H

#include "stm8s.h"
#include "delay.h"
#include "KeyBorad.h"
#include "user_uart.h"
#define LCD_PROT GPIOC
#define CS_1_PIN GPIO_PIN_7
#define RD_PIN GPIO_PIN_6
#define WR_PIN GPIO_PIN_5
#define DATA_PIN GPIO_PIN_4
#define CS_2_PIN GPIO_PIN_3

#define DELAY_US_NUM 4

#define Set_Port_Val(LCD_PIN,Vaule) if(Vaule) GPIO_WriteHigh(LCD_PROT,LCD_PIN); else GPIO_WriteLow(LCD_PROT,LCD_PIN);

#define HT_BISA_COM			0x52		//(1<<1) | (0<<3) | (1<<4) | (0<<5) | (1<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_LCD_OFF			0x04		//(0<<1) | (1<<2) | (0<<4) | (0<<5) | (0<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_LCD_ON			0x06		//(1<<1) | (1<<2) | (0<<4) | (0<<5) | (0<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_WRITE_CMD		0x80		//(0<<0) | (0<<1) | (1<<2)  | (0<<3) | (1<<4) | (0<<5) | (1<<6) | (0<<7) | (1<<8)
#define HT_WRITE_DATA		0XA0
#define HT_SYS_EN			0x02
#define HT_RCOSC			0x30
#define HT_RCOSCEX          0x38
#define HT_SYS_DIS_EN 0X00
#define CS1_DIGITRON_START               1
#define CS1_DIGITRON_END                 11
#define CS2_12_15_DIGITRON_START         12
#define CS2_12_15_DIGITRON_END           15
#define CS2_16_19_DIGITRON_START         16
#define CS2_16_19_DIGITRON_END           19
#define CS2_20_24_DIGITRON_START         20
#define CS2_20_24_DIGITRON_END           24

#define CS1_T_START                      1
#define CS1_T_END                        20
#define CS2_T_START                      21
#define CS2_T_END                        33

#define CS1_S_START                      1
#define CS1_S_END                        2

#define CS1_P_START                      1
#define CS1_P_END                        7

void ht1621_Clear();
void ht1621_write(u8 cs ,u8 addr, u8 data);
void ht1621_Char_write1(u8 cs ,u8 addr, u8 data ,u8 status,u8 xin);
void ht1621_Char_write(u8 cs ,u8 addr, u8 data ,u8 status);
void ht1621_init();
void ht1621_char_display();
void Now_Time_Display(struct ALLDATE alldate , struct KEYHANDLE KeyHandle);
void Hepa_Set_Display(struct Hepa hepa,struct KEYHANDLE KeyHandle,struct Peripheral peripheral);
void Peripheral_Rceive_Display(struct Peripheral peripheral,u8 Fan_Seepd_Max_State);
void Fan_Speed_State_Display(u8 Fan_Seepd_State);
void Door_State_Display(u8 Door_State,struct Peripheral peripheral);
void Display_all(struct Peripheral peripheral,struct KEYHANDLE KeyHandle,struct Hepa Hepa,struct ALLDATE alldate);
extern const unsigned char Dis_Digitron_Addr[];
extern const unsigned char Cs1_Dis_Digitron_Num[];
extern const unsigned char P_Addr[];
extern const unsigned char P_Mask[];

extern const unsigned char S_Addr[];
extern const unsigned char S_Mask[];
extern const unsigned char T_Mask[];
extern const unsigned char T_Addr[];

extern const unsigned char Cs2_12_15_Dis_Digitron_Num[];
extern const unsigned char Cs2_16_19_Dis_Digitron_Num[];
extern const unsigned char Cs2_20_24_Dis_Digitron_Num[];

extern u8 Dis_Bling;
extern u16 hepa_time;
#endif
