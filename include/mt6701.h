#ifndef _MT6701_H
#define _MT6701_H
#include <SimpleFOC.h>
#include "Arduino.h"
#include <Wire.h>

#define MT6701_SCL_PIN  17
#define MT6701_SDA_PIN  16

class MT6701: public Sensor {
public:
    MT6701();
    void init();
    float getSensorAngle();

    int readOneByte(int in_adr);
    word readTwoBytes(int in_adr_hi, int in_adr_lo);
    void writeOneByte(int adr_in, int dat_in);

private:
    byte _mt6701_address;
    byte angle_hi;
    byte angle_lo;
    float x_;
    float y_;
};

#endif