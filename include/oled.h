#ifndef OLED_H
#define OLED_H

#include "Arduino.h"
#include <Wire.h>

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  	

#define OLED_SCLK_PIN 22
#define OLED_SDIN_PIN 21

#define OLED_SCLK_Set() digitalWrite(OLED_SCLK_PIN, HIGH)
#define OLED_SDIN_Set() digitalWrite(OLED_SDIN_PIN, HIGH)
#define OLED_SDIN_Clr() digitalWrite(OLED_SDIN_PIN, LOW)
#define OLED_SCLK_Clr() digitalWrite(OLED_SCLK_PIN, LOW)
 		     
#define OLED_CMD  1	//写命令
#define OLED_DATA 0	//写数据


// #define byte unsigned char
// #define u32 unsigned int

//OLED控制用函数
class OLED
{
  public:
    OLED();
    void begin();
    void OLED_WR_Byte(unsigned dat,unsigned cmd);  
    void OLED_Display_On(void);
    void OLED_Display_Off(void);					   		    
    
    void Clear(void);
    void DrawPoint(byte x,byte y,byte t);
    void Fill(byte x1,byte y1,byte x2,byte y2,byte dot);
    void ShowChar(byte x,byte y,byte chr,byte Char_Size);
    void ShowNumber(byte x,byte y,unsigned long num,byte len,byte size);
    void ShowString(byte x,byte y, byte *p,byte Char_Size);	 
    void Set_Pos(unsigned char x, unsigned char y);
    void ShowCHinese(byte x,byte y,byte no);
    void DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
    void fill_picture(unsigned char fill_Data);
    void Picture(void);

    void fuhao_write(unsigned char x,unsigned char y,unsigned char asc);
    void Num_write(unsigned char x,unsigned char y,unsigned char asc) ;
    void Float(unsigned char Y,unsigned char X,double real,unsigned char N);
    void Float2(unsigned char Y,unsigned char X,double real,unsigned char N1,unsigned char N2);
    void Num2(unsigned char x,unsigned char y, int number);
    void Num3(unsigned char x,unsigned char y,int number); 
    void Num4(unsigned char x,unsigned char y, int number);
    void Num5(unsigned char x,unsigned char y,unsigned int number);

    void OLED_IIC_Start(void);
    void OLED_IIC_Stop(void);
    void OLED_Write_IIC_Command(unsigned char IIC_Command);
    void OLED_Write_IIC_Data(unsigned char IIC_Data);
    void OLED_Write_IIC_Byte(unsigned char IIC_Byte);
    void OLED_IIC_Wait_Ack(void);

    byte _oled_address;
    byte write_data;
    byte write_cmd;
};

#endif