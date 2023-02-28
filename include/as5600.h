#ifndef _AS5600_H
#define _AS5600_H

#include <Arduino.h>
#include "Wire.h"

#define AS5600_SCL_PIN 17//33
#define AS5600_SDA_PIN 16//032
class AS_5600
{
  public:
    
    AS_5600(void);
    int getAddress();
    void begin();

    word setMaxAngle(word newMaxAngle = -1);
    word getMaxAngle();
    
    word setStartPosition(word startAngle = -1);
    word getStartPosition();
    
    word setEndPosition(word endAngle = -1);
    word getEndPosition();
    
    word getRawAngle();
    word getScaledAngle();
    
    int  detectMagnet();
    int  getMagnetStrength();
    int  getAgc();
    word getMagnitude();
    
    int  getBurnCount();
    int  burnAngle();
    int  burnMaxAngleAndConfig();

    float convertScaledAngleToDegrees();
    float convertRawAngleToDegrees();

private:
  
    int _ams5600_Address;
      
    word _rawStartAngle;
    word _zPosition;
    word _rawEndAngle;
    word _mPosition;
    word _maxAngle;
    
    /* Registers */
    int _zmco;
    int _zpos_hi;    /*zpos[11:8] high nibble  START POSITION */
    int _zpos_lo;    /*zpos[7:0] */
    int _mpos_hi;    /*mpos[11:8] high nibble  STOP POSITION */
    int _mpos_lo;    /*mpos[7:0] */
    int _mang_hi;    /*mang[11:8] high nibble  MAXIMUM ANGLE */
    int _mang_lo;    /*mang[7:0] */
    int _conf_hi;    
    int _conf_lo;
    int _raw_ang_hi;
    int _raw_ang_lo;
    int _ang_hi;
    int _ang_lo;
    int _stat;
    int _agc;
    int _mag_hi;
    int _mag_lo;
    int _burn;
    
    int readOneByte(int in_adr);
    word readTwoBytes(int in_adr_hi, int in_adr_lo);
    void writeOneByte(int adr_in, int dat_in);
   
};


#endif 

