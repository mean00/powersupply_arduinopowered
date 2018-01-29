/**
 This is the version where the max current is managed by a rotarty encoder
 * and the target is output by the arduino to the CC board
 * Unforunately we lack an accurate DAC for the moment, we need an external one
 * 10 bits is enough
 */
#include "Arduino.h"
#include "Rotary.h"
#include "wav_irotary.h"
#include <Wire.h>
#include "pow_currentControl.h"
#include <Adafruit_MCP4725.h>

/**
 
 */

class rotaryMaxCurrentControl : public MaxCurrentControl
{
public:
                            rotaryMaxCurrentControl(int pin,int rotA, int rotB,int dacAdr);
       void                 run();
       int                  getMaxCurrentMa();       
       
protected:
        int _pin;
        WavRotary           *_rotary;        // rotary encoder
        Adafruit_MCP4725    *dac;
};
/**
 */
rotaryMaxCurrentControl::rotaryMaxCurrentControl(int pin, int rotA,int rotB,int dacAdr) : MaxCurrentControl(pin)
{
    _rotary=new WavRotary(rotA,rotB);
    dac=new Adafruit_MCP4725;
    dac->begin(dacAdr); //0x60);
    dac->setVoltage(100,false);
}


/**
 */
MaxCurrentControl    *rotaryCurrentControl_instantiate(int pin,int rotA,int rotB,int dacAdr)
{
    return new rotaryMaxCurrentControl(pin,rotA,rotB,dacAdr);
}
/**
 */
void rotaryMaxCurrentControl::run()
{
  int nw;

  nw+=50*_rotary->getCount();
  if(nw<50) 
      nw=50;
  if(nw>4095) 
      nw=4095;
  if(_maxCurrent!=nw)
  {
      dac->setVoltage(nw,false);    
  }
}
/**
 */
int rotaryMaxCurrentControl::getMaxCurrentMa()
{
    return _maxCurrent;
}