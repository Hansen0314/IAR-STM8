
#ifndef _HT1621_H
#define _HT1621_H

#include "stm8s.h"
#include "delay.h"
#define LCD_PROT GPIOC
#define CS_1_PIN GPIO_PIN_7
#define RD_PIN GPIO_PIN_6
#define WR_PIN GPIO_PIN_5
#define DATA_PIN GPIO_PIN_4
#define CS_2_PIN GPIO_PIN_7

#define DELAY_US_NUM 4

#define Set_Port_Val(LCD_PIN,Vaule) if(Vaule) GPIO_WriteHigh(LCD_PROT,LCD_PIN); else GPIO_WriteLow(LCD_PROT,LCD_PIN);

#define HT_BISA_COM			0x52		//(1<<1) | (0<<3) | (1<<4) | (0<<5) | (1<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_LCD_OFF			0x04		//(0<<1) | (1<<2) | (0<<4) | (0<<5) | (0<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_LCD_ON			0x06		//(1<<1) | (1<<2) | (0<<4) | (0<<5) | (0<<6) | (0<<7) | (0<<8) | (0<<9) | (0<<10) | (1<<11)
#define HT_WRITE_CMD		        0x80		//(0<<0) | (0<<1) | (1<<2)  | (0<<3) | (1<<4) | (0<<5) | (1<<6) | (0<<7) | (1<<8)
#define HT_WRITE_DATA		        0XA0
#define HT_SYS_EN			0x02
#define HT_RCOSC			0x30
#define HT_RCOSCEX                      0x38
//#define T1_MASK 
//
void ht1621_write(u8 cs ,u8 addr, u8 data);
void ht1621_Char_write(u8 cs ,u8 addr, u8 data ,u8 status);
void ht1621_init();
u8 ht1621_read(u8 addr);
extern const unsigned char Cs1_Dis_Digitron_Addr[];
extern const unsigned char Cs1_Dis_Digitron_Num[];
extern const unsigned char P_Addr[];
extern const unsigned char P_Mask[];

extern const unsigned char S_Addr[];
extern const unsigned char S_Mask[];
extern const unsigned char T_Mask[];
extern const unsigned char T_Addr[];

#endif
