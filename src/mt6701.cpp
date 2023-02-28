#include "mt6701.h"

TwoWire mt6701_wire = TwoWire(1);
static const float ALPHA = 0.4;

MT6701::MT6701()
{
    _mt6701_address = 0x06;
    angle_hi = 0x03;
    angle_lo = 0x04;
}

void MT6701::init()
{
    mt6701_wire.begin(MT6701_SDA_PIN, MT6701_SCL_PIN);

}

float MT6701::getSensorAngle()
{
    int ret_value;
    ret_value = readTwoBytes(angle_hi, angle_lo);

    float new_angle = (float)ret_value * 2 * PI / 16384;
    float new_x = cosf(new_angle);
    float new_y = sinf(new_angle);
    x_ = new_x * ALPHA + x_ * (1-ALPHA);
    y_ = new_y * ALPHA + y_ * (1-ALPHA);

    float rad = -atan2f(y_, x_);
    if (rad < 0) {
        rad += 2*PI;
    }
    return rad;
}

int MT6701::readOneByte(int in_adr)
{
    int retVal = -1;
    mt6701_wire.beginTransmission(_mt6701_address);
    mt6701_wire.write(in_adr);
    mt6701_wire.endTransmission();
    mt6701_wire.requestFrom(_mt6701_address, 1);
    while(mt6701_wire.available() == 0);
    retVal = mt6701_wire.read();  
    return retVal;
}
word MT6701::readTwoBytes(int in_adr_hi, int in_adr_lo)
{
    word retVal = -1;

    /* Read Low Byte */
    mt6701_wire.beginTransmission(_mt6701_address);
    mt6701_wire.write(in_adr_lo);
    mt6701_wire.endTransmission();
    mt6701_wire.requestFrom(_mt6701_address, 1);
    while(mt6701_wire.available() == 0);
    int low = mt6701_wire.read();

    /* Read High Byte */  
    mt6701_wire.beginTransmission(_mt6701_address);
    mt6701_wire.write(in_adr_hi);
    mt6701_wire.endTransmission();
    mt6701_wire.requestFrom(_mt6701_address, 1);

    while(mt6701_wire.available() == 0);

    word high = mt6701_wire.read();

    high = high << 8;
    retVal = (high | low)>>2;

    return retVal;
}

void MT6701::writeOneByte(int adr_in, int dat_in)
{    
  mt6701_wire.beginTransmission(_mt6701_address);
  mt6701_wire.write(adr_in); 
  mt6701_wire.write(dat_in);
  mt6701_wire.endTransmission();
}