#include "lcd.h"

#define DEG2RAD 0.0174532925




//屏幕更新
void Screen::SreenUpdate(bool state, float zone ,int step)
{
  int weizhi = 60 /PI * 180.0 + 0.5;
  static int last_weizhi = -1000 ,last_fn_state = 0;
  
  char buf[10];

  if(state)   ArcPro(20 , zone); //画弧
  
  Pointer(20); //画指针
  
  if(step != -100) weizhi = step; //等于-100则显示角度 //不等于则显示级数STEP

  //更新位置
  // if(last_weizhi != weizhi || last_weizhi == -1000 )
  {
    tft.drawCentreString("           ",120,80,7);
    // if(fn_state == 4)
    // {
    //   if( weizhi > 28)  tft.drawCentreString("ON",120,80,4);
    //   else              tft.drawCentreString("OFF",120,80,4);
    // }
    // else
    {
      tft.drawCentreString("a2",120,80,7);
    }
    last_weizhi = weizhi; 
  } 
  // else if( last_fn_state != fn_state)
  // {
  //   last_weizhi = -1000;
  //   last_fn_state = fn_state;
  // }
}



//电机当前角度/弧度 
void Screen::Pointer( float angle )
{
  static int last_x = -100, last_y = 0;
  angle = -angle - 0.5*PI + 0;
  int x = 110 * cos(angle) + 120.5;
  int y = 110 * sin(angle) + 120.5;
  if(last_x != -100)//第一次则跳过清圆
  {
    if(last_x == x && last_y == y)//位置与上次一样 退出
    return;
    tft.fillCircle(last_x,last_y,3,TFT_BLACK);//清圆
  }
  tft.fillCircle(x,y,3,TFT_MAGENTA);
  last_x = x;
  last_y = y;
}  


//bug：某种情况下会有弧线的小颗粒残留
//电机当前角度/弧度  区域/弧度
void Screen::ArcPro(float weizhi, float total)
{
  static int zero = 0;
  static int last_x = zero;
  int x = (weizhi - 0)/ total * 120.0 + 0.5;
  if(last_x != x )
  {
    if( x > last_x ) //逆时针滑动
    {
      if(last_x >= zero)//画白弧 l_x 到 x
      {
        if(x > 120)//超过一圈的范围了
        {
          if(last_x > 120)//全都超过一圈
          {
            int error = x -last_x;
            fillArc(120, 120, -x * 3 , error , 100, 100, 4, TFT_RED);//画黄弧 从x 到 l_x
          }
          else//由白圈超到黄圈
          {
            int error = 120 - last_x;
            fillArc(120, 120, -120 * 3 , error , 100, 100, 4, TFT_RED);//画白弧 从zero 到 l_x
            error = x - 120;
            fillArc(120, 120, -x * 3 , error , 100, 100, 4, TFT_RED);//画黄弧 从x 到 zero
          }
        }
        else
        {
          int error = x - last_x;   //顺时针从 x 到 l_x 画弧
          fillArc(120, 120, -x * 3 , error, 100, 100, 4, TFT_WHITE);
        }
      }
      else if( x <= zero) //画黑弧 l_x 到 x
      {
        int error = x - last_x;   //顺时针从 x 到 l_x 画弧
        fillArc(120, 120, -x*3 , error , 100, 100, 6, TFT_BLACK);
      }
      else //0位置在x和l_x中间
      {
        int error = zero - last_x;  
        fillArc(120, 120, -zero * 3 , error , 100, 100, 6, TFT_BLACK);//黑弧 l_x 到 0 位置 
        error = x - zero;   
        fillArc(120, 120 , -x*3 , error , 100, 100, 4, TFT_WHITE);//白弧 0   到 x 位置
      }
    }

    //顺时针滑动 last_x > x
    else 
    {
      if(x >= zero)//画黑弧 x 到 l_x
      {
        if(last_x > 120)
        {
          if(x > 120) //一圈多了，回转 ，白
          {
            int error = last_x - x;   //顺时针从 x 到 l_x 画弧
            //fillArc(120, 120, -last_x * 3 , error, 90, 90, 6, TFT_BLACK);
            fillArc(120, 120, -last_x * 3 , error, 100, 100, 4, TFT_WHITE);
          }
          else//zero 在 中间
          {
            int error = - 120 + last_x;
            fillArc(120, 120, -last_x * 3 , error , 100, 100, 4, TFT_WHITE);//画白弧 从zero 到 l_x
            error =  - x + 120;
            fillArc(120, 120, -120 * 3 , error , 100, 100, 6, TFT_BLACK);//画黑弧 从x 到 zero
          }
        }
        else
        {
          int error = last_x - x;   //顺时针从 l_x 到 x 画弧
          fillArc(120, 120, -last_x * 3 , error, 100, 100, 6, TFT_BLACK);
        }
      }
      else if( last_x <= zero) //画红弧 x 到 l_x
      {
        int error = last_x - x;   //顺时针从 l_x 到 x 画弧
        fillArc(120, 120, -last_x * 3 , error , 100, 100, 4, TFT_RED);
      }
      else //0位置在x和l_x中间
      {
        int error = last_x - zero;   //顺时针从 l_x 到 x 画弧
        fillArc(120, 120, -last_x*3 , error , 100, 100, 6, TFT_BLACK);//黑弧 l_x 到 0 位置
        error = zero - x; 
        fillArc(120, 120, -zero*3 , error, 100, 100, 4, TFT_RED);//红弧 0   到 x 位置
      }
    }
    //Serial.printf("x = %d , last_x = %d \n",x,last_x);
    last_x = x;
  }
}

void Screen::fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

  byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}

