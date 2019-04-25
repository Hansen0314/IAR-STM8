#include "DS1302.h"
void ds1302_port_init(void)
{
        GPIO_Init(DS1302_PORT_CLK,DS1302_CLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//RTC_CLK
        GPIO_Init(DS1302_PORT_RST,DS1302_RST_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//        
        GPIO_Init(DS1302_PORT,DS1302_IO_PIN, GPIO_MODE_IN_FL_NO_IT);      //RTC_DATA   
        
        GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN); 
        GPIO_WriteLow(DS1302_PORT,DS1302_CLK_PIN); 
        
}
 
void read_IO(void){
        GPIO_Init(DS1302_PORT,DS1302_IO_PIN, GPIO_MODE_IN_FL_NO_IT);   //RTC_DATA        
}
  
void write_IO(void){ 
        GPIO_Init(DS1302_PORT,DS1302_IO_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);//RTC_DATA
}
 
 
void ds1302_write_byte(unsigned char temp) 
{
   unsigned char i;
   for (i=0;i<8;i++)     
   { 
       // delay_10us(5);
       GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN);
        
       if(temp&0x01) {
           GPIO_WriteHigh(DS1302_PORT,DS1302_IO_PIN); 
       }else {
           GPIO_WriteLow(DS1302_PORT,DS1302_IO_PIN);
       }     	     
       temp>>=1; 
      //  delay_10us(10);
       GPIO_WriteHigh(DS1302_PORT_CLK,DS1302_CLK_PIN); 
             
    }
}  
unsigned char ds1302_read_byte(void) 
{
  unsigned char i,temp=0;
  for (i=0;i<8;i++) 		
  {		
     GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN);
     temp>>=1;
     if(GPIO_ReadInputPin(DS1302_PORT,DS1302_IO_PIN))
     {
          temp|=0x80;	
     }
   // delay_10us(5);
     GPIO_WriteHigh(DS1302_PORT_CLK,DS1302_CLK_PIN); 
  } 
  return temp;
} 

void ds1302_writeByte(u8 address,u8 dat)
{
    write_IO();
    GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);   //写地址，写数据 RST保持高电平
    GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN);
    GPIO_WriteHigh(DS1302_PORT_RST,DS1302_RST_PIN);
    //delay_10us(1);
    ds1302_write_byte(address);	
    ds1302_write_byte(dat);		
    //delay_10us(1);
    GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);
}
u8 ds1302_readByte(u8 address)
{
    unsigned char ret;
    write_IO();
    GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);
    GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN);
    GPIO_WriteHigh(DS1302_PORT_RST,DS1302_RST_PIN);
    // delay_10us(1);
    ds1302_write_byte(address);  //读标志：地址最后一位为1
    read_IO();
    ret = ds1302_read_byte();
     //delay_10us(1);
    GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);
    return (ret);	
}
unsigned char BCDToDec(unsigned char mData)
{

	unsigned char Dec = 0;
	Dec = mData & 0x0f; //取得最低位
	mData = mData & 0x70; //剔除最高位和最低四位
	Dec += mData>>1; //先左移1代表*2
	Dec += mData>>3; //再左移3代表*8，所以总的相当于乘以10
	return Dec;
}
unsigned char DecToBCD(unsigned char mData)
{
	unsigned char BCD = 0;
	while(mData >= 10)
	{
		mData -= 10;
		BCD ++;
	};
	BCD <<= 4;
	BCD |= mData;
	return BCD;
}
struct ALLDATE convertToDisplayTime(struct ALLDATE allDate)
{
      struct ALLDATE mAllDate;
      mAllDate.yd.year = (((allDate.yd.year&0x80)>>4)*10) + BCDToDec(allDate.yd.year);
      mAllDate.yd.day = BCDToDec(allDate.yd.day);
      mAllDate.md.month = BCDToDec(allDate.md.month);
      mAllDate.md.date = BCDToDec(allDate.md.date);
      mAllDate.hms.hour = BCDToDec(allDate.hms.hour);
      mAllDate.hms.min = BCDToDec(allDate.hms.min);
      mAllDate.hms.sec = BCDToDec(allDate.hms.sec);
      return mAllDate;
}
struct ALLDATE convertToSetTime(struct ALLDATE allDate)
{
	struct ALLDATE mAllDate;
	mAllDate.yd.year = DecToBCD(allDate.yd.year);
	mAllDate.yd.day = DecToBCD(allDate.yd.day);
	mAllDate.md.month = DecToBCD(allDate.md.month);
	mAllDate.md.date = DecToBCD(allDate.md.date);
	mAllDate.hms.hour = DecToBCD(allDate.hms.hour);
	mAllDate.hms.min = DecToBCD(allDate.hms.min);
	mAllDate.hms.sec = CH + DecToBCD(allDate.hms.sec);
	return mAllDate;
}

void ds1302_setTime(struct ALLDATE allDate)
{
	allDate = convertToSetTime(allDate);
	ds1302_writeByte(0x8e, 0x00);//control为的最高位wp
	ds1302_writeByte(0x8c, allDate.yd.year); //year
	ds1302_writeByte(0x8a, allDate.yd.day);//day
	ds1302_writeByte(0x88, allDate.md.month); //month
	ds1302_writeByte(0x86, allDate.md.date); //date
	ds1302_writeByte(0x84, allDate.hms.hour); //hour
	ds1302_writeByte(0x82, allDate.hms.min); //min
	ds1302_writeByte(0x80, allDate.hms.sec); //sec
}
struct ALLDATE ds1302_readTime( void )
{
	struct ALLDATE allDate;
	allDate.yd.year = ds1302_readByte(0x8d); //year
	allDate.yd.day = ds1302_readByte(0x8b); //day
	allDate.md.month = ds1302_readByte(0x89); //month
	allDate.md.date = ds1302_readByte(0x87); //date
	allDate.hms.hour = ds1302_readByte(0x85); //hour
	allDate.hms.min = ds1302_readByte(0x83); //min
	allDate.hms.sec = ds1302_readByte(0x81); //sec 
	allDate = convertToDisplayTime(allDate);
	return allDate;

}
void Ds1302_Init()
{
	struct ALLDATE allDate;
	allDate.yd.year = 18;
	allDate.yd.day = 5;
	allDate.md.month = 10;
	allDate.md.date = 20;
        allDate.hms.hour = 20;
        allDate.hms.min = 15;
        allDate.hms.sec = 30;
        ds1302_port_init();
	ds1302_setTime(allDate);
}
