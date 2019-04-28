#include "cds1302.h"
 
void delay()
{
    int i;
    for(i=0;i<10;i++)
       ;
}
void ds1302_port_init(void)
{
        GPIO_Init(DS1302_PORT_CLK,DS1302_CLK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//RTC_CLK
        GPIO_Init(DS1302_PORT_RST,DS1302_RST_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//        
        GPIO_Init(DS1302_PORT,DS1302_IO_PIN, GPIO_MODE_IN_FL_NO_IT);      //RTC_DATA   
        
        GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN); 
        GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN); 
        
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
     if(GPIO_ReadInputDataBit(DS1302_PORT,DS1302_IO_PIN))
     {
          temp|=0x80;	
     }
   // delay_10us(5);
     GPIO_WriteHigh(DS1302_PORT_CLK,DS1302_CLK_PIN); 
    
     
  } 
  return temp;
} 
 
void ds1302_write( unsigned char address,unsigned char dat )     
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
/****************************************************************************/
unsigned char ds1302_read( unsigned char address )
{
 	unsigned char ret;
        write_IO();
 	GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);
 	GPIO_WriteLow(DS1302_PORT_CLK,DS1302_CLK_PIN);
 	GPIO_WriteHigh(DS1302_PORT_RST,DS1302_RST_PIN);
        // delay_10us(1);
 	ds1302_write_byte(address|0x01);  //读标志：地址最后一位为1
        read_IO();
        ret = ds1302_read_byte();
         //delay_10us(1);
 	GPIO_WriteLow(DS1302_PORT_RST,DS1302_RST_PIN);
	return (ret);		
}	
 
unsigned char ds1302_check(void) 
{ 
    unsigned char ret;
    ds1302_write(DS1302_CONTROL_REG,0x80); 
    ret = ds1302_read(DS1302_CONTROL_REG);
    if(ret==0x80)
       return 1; 
    return 0; 
}
 
void ds1302_read_time(DS1302_TIME* time) 
{ 
   time->year=ds1302_read(DS1302_YEAR_REG); //年 
    time->month=ds1302_read(DS1302_MONTH_REG);//月 
   time->day=ds1302_read(DS1302_DATE_REG); //日 
    time->week=ds1302_read(DS1302_DAY_REG); //周 
    time->hour=ds1302_read(DS1302_HR_REG); //时 
    time->minute=ds1302_read(DS1302_MIN_REG); //分 
    time->second=ds1302_read(DS1302_SEC_REG); //秒 
} 
void ds1302_write_time(DS1302_TIME* time) 
{ 
    ds1302_write(DS1302_CONTROL_REG,0x00); //关闭写保护 
    ds1302_write(DS1302_SEC_REG,0x80); //暂停 
    ds1302_write(DS1302_CHARGER_REG,0xa9); //涓流充电 
 
    //ds1302_write(DS1302_SECOND_ADDR, 0x00);           // 启动振荡器
   // ds1302_write(DS1302_YEAR_REG,time->year); //年 
   // ds1302_write(DS1302_MONTH_REG,time->month); //月 
   // ds1302_write(DS1302_DATE_REG,time->day); //日 
   // ds1302_write(DS1302_DAY_REG,time->week); //周 
    ds1302_write(DS1302_HR_REG,time->hour); //时 
    ds1302_write(DS1302_MIN_REG,time->minute); //分 
    ds1302_write(DS1302_SEC_REG,0x00); //秒 
    ds1302_write(DS1302_CONTROL_REG,0x80); //打开写保护 
}
 
void ds1302_write_ram(unsigned char ram_num,unsigned char dat) 
{ 
    ds1302_write(DS1302_CONTROL_REG,0x00); //关闭写保护 
    ds1302_write((DS1302_RAM_REG|(ram_num<<2)),dat);
    ds1302_write(DS1302_CONTROL_REG,0x80);
}
 
unsigned char  ds1302_read_ram(unsigned char ram_num) 
{ 
    unsigned char ret;
    ret = ds1302_read((DS1302_RAM_REG|(ram_num<<2)));
    return ret;
}
