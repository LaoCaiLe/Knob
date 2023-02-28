#include "oled.h"
#include "oledfont.h"


TwoWire OLED_Wire = TwoWire(0);

OLED::OLED()
{
    _oled_address = 0x3c;
    write_data = 0x40;
    write_cmd = 0x00;
}  

void OLED::OLED_IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void OLED::OLED_IIC_Stop()
{
	OLED_SCLK_Set();
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void OLED::OLED_IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void OLED::OLED_Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void OLED::OLED_Write_IIC_Command(unsigned char IIC_Command)
{
		OLED_IIC_Start();
		OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(0x00);			//write command
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(IIC_Command); 
		OLED_IIC_Wait_Ack();	
		OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLED::OLED_Write_IIC_Data(unsigned char IIC_Data)
{
		OLED_IIC_Start();
		OLED_Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(0x40);			//write data
		OLED_IIC_Wait_Ack();	
		OLED_Write_IIC_Byte(IIC_Data);
		OLED_IIC_Wait_Ack();	
		OLED_IIC_Stop();
}
void OLED::OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    OLED_Wire.beginTransmission(_oled_address);
	if(cmd)
	{  
        OLED_Wire.write(write_cmd);    
	}
	else
	{
        OLED_Wire.write(write_data);
	}
    OLED_Wire.write(dat);
    OLED_Wire.endTransmission();

    // if(cmd)
    //     OLED_Write_IIC_Command(dat);
    // else
    //     OLED_Write_IIC_Data(dat);
}

void OLED::begin()
{
    OLED_Wire.begin();
    // pinMode(OLED_SCLK_PIN, OUTPUT);
    // pinMode(OLED_SDIN_PIN, OUTPUT);

    delay(10);
    OLED_WR_Byte(0xAE, OLED_CMD); //--display off
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
    OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
    OLED_WR_Byte(0x81, OLED_CMD); // contract control
    OLED_WR_Byte(0xFF, OLED_CMD); //--128
    OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
    OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
    OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
    OLED_WR_Byte(0x00, OLED_CMD); //

    OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
    OLED_WR_Byte(0x80, OLED_CMD); //

    OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
    OLED_WR_Byte(0x05, OLED_CMD); //

    OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
    OLED_WR_Byte(0xF1, OLED_CMD); //

    OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
    OLED_WR_Byte(0x12, OLED_CMD); //

    OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
    OLED_WR_Byte(0x30, OLED_CMD); //

    OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
    OLED_WR_Byte(0x14, OLED_CMD); //

    OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel

    Clear();
}
/********************************************
// fill_Picture
********************************************/
void OLED::fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}



//坐标设置

void OLED::Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED::OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED::OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED::Clear(void)  
{  
	byte i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED::ShowChar(byte x,byte y,byte chr,byte Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>Max_Column-1)
	{
		x=0;
		y=y+2;
	}

	Set_Pos(x,y);	
	for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);

	Set_Pos(x,y+1);
	for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);

}
//m^n函数
unsigned long oled_pow(byte m,byte n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED::ShowNumber(byte x,byte y,unsigned long num,byte len,byte size2)
{         	
	byte t,temp;
	byte enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//显示一个字符号串
void OLED::ShowString(byte x,byte y,byte *chr,byte Char_Size)
{
	while (*chr!='\0')
	{		
		ShowChar(x,y,*chr,Char_Size);
		x+=8;
		if(x>120)
		{
			x=0;
			y+=2;
		}
		chr++;
	}
}
//显示汉字
void OLED::ShowCHinese(byte x,byte y,byte no)
{      			    
	byte t,adder=0;
	Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
	}	
	Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
	}					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED::DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;

	for(y=y0;y<y1;y++)
	{
		Set_Pos(x0,y);
    	for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
void OLED::Float(unsigned char X,unsigned char Y,double real,unsigned char N) 
{
	unsigned char   i_Count=1;
	unsigned char   n[12]={0};
	long   j=1;  
	int    real_int=0;
	double decimal=0;
	unsigned int real_decimal=0;
	if(real<0)
	{
		real_int=(int)(-real);
	}
	else
	{
		real_int=(int)real;
   	}
	decimal=real-real_int;
   	real_decimal=decimal*1e4;
	while(real_int/10/j!=0)
	{
		j=j*10;i_Count++;  
	} 
	n[0]=(real_int/10000)%10; 
	n[1]=(real_int/1000)%10;
	n[2]=(real_int/100)%10;
	n[3]=(real_int/10)%10;
	n[4]=(real_int/1)%10;
	
	

	n[6]=(real_decimal/1000)%10;
	n[7]=(real_decimal/100)%10;
	n[8]=(real_decimal/10)%10;
	n[9]=real_decimal%10;
	n[6+N]='\0'; 
	for(j=0;j<10;j++) 
		n[j]=n[j]+16+32;
	if(real<0) 
	{		 
		i_Count+=1;
		n[5-i_Count]='-';
	}
   //n[5]='.';
    if(N == 0)
	{
		n[5]='\0 ';
	} 
	else
	{
		n[5]='.';
	}
	n[6+N]='\0';   
	ShowString(X,Y,&n[5-i_Count],12); 
}

void OLED::Float2(unsigned char X,unsigned char Y,double real,unsigned char N1,unsigned char N2) 
{
	unsigned char   i_Count=1;
	unsigned char   n[12]={0};
	long   j=1;  
	unsigned int   real_int=0;
	double decimal=0;
	unsigned int   real_decimal=0;
	X=X*8;
	real_int=(int)real;
	//Dis_Num(2,0,real_int,5);
	decimal=real-real_int;
	real_decimal=decimal*1e4;
	//Dis_Num(2,6,real_decimal,4);
	while(real_int/10/j!=0)
	{
		j=j*10;i_Count++;  
	} 
	n[0]=(real_int/10000)%10; 
	n[1]=(real_int/1000)%10;
	n[2]=(real_int/100)%10;
	n[3]=(real_int/10)%10;
	
	n[5]='.';
	n[6]=(real_decimal/1000)%10;
	n[7]=(real_decimal/100)%10;
	n[8]=(real_decimal/10)%10;
	n[9]=real_decimal%10;
	n[6+N2]='\0'; 
	for(j=0;j<10;j++) n[j]=n[j]+16+32;
	n[5]='.';
	n[6+N2]='\0';   
	ShowString(X,Y,&n[5-N1],12); 
}

void OLED::Num2(unsigned char x,unsigned char y, int number)
{
	unsigned char shi,ge;
	int num =number;
	if(num<0)
	{ 
		num=-num;
		shi=num%100/10;
    	ge=num%10;
		fuhao_write(x,y,13); 
		Num_write(x+1,y,shi);
		Num_write(x+2,y,ge); 
	} 
	else
	{
		shi=num%100/10;
		ge=num%10;
		fuhao_write(x,y,11);
		Num_write(x+1,y,shi);
		Num_write(x+2,y,ge); 
	}
        
}

void OLED::Num3(unsigned char x,unsigned char y,int number)
{
  unsigned char ge,shi,bai;
	int num =number;
	if(num<0)
	{
		num=-num;
		fuhao_write(x,y,13); //显示-号
		ge = num %10;
		shi = num/10%10;
		bai = num/100;
		Num_write(x+3,y,ge);
		Num_write(x+2,y,shi);
		Num_write(x+1,y,bai);
	}
	else
	{
      	fuhao_write(x,y,11);
        ge = num %10;
        shi = num/10 %10;
        bai = num/100;
        Num_write(x+3,y,ge);
        Num_write(x+2,y,shi);
        Num_write(x+1,y,bai);
  }
}

void OLED::Num4(unsigned char x,unsigned char y, int number)
{
	unsigned char qian,bai,shi,ge;
	int num =number;
	if(num<0)
	{
		num=-num;
	}
	qian=num/1000;
	bai=num%1000/100;
	shi=num%100/10;
	ge=num%10;

	Num_write(x,y,qian);
	Num_write(x+1,y,bai);
	Num_write(x+2,y,shi);
	Num_write(x+3,y,ge);
}

void OLED::Num_write(unsigned char x,unsigned char y,unsigned char asc) 
{
	int i=0;
	Set_Pos(x*6,y);
	for(i=0;i<6;i++)
	{
		OLED_WR_Byte(F6x8[asc+16][i],OLED_DATA);         
	}
}	
void OLED::fuhao_write(unsigned char x,unsigned char y,unsigned char asc) 
{

	int i=0;
    Set_Pos(x*6,y);
    for(i=0;i<6;i++)
    {
       OLED_WR_Byte(F6x8[asc][i],OLED_DATA);         
    }
 
}			

void OLED::Num5(unsigned char x,unsigned char y,unsigned int number)
{
	unsigned char wan,qian,bai,shi,ge;
	wan=number/10000;
	qian = number%10000/1000;
	bai=number%1000/100;
	shi=number%100/10;
	ge=number%10;
	Num_write(x,y,wan);
	Num_write(x+1,y,qian);
	Num_write(x+2,y,bai);
	Num_write(x+3,y,shi);
	Num_write(x+4,y,ge);
}

			    

