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
 * \fn CurrentToDac
 * \brief convert current command to dac value
 * Current in mA
 */
static int CurrentToDac(int current)
{
    int r;
    if(current<=300)
        r= ((current*10)/26);
    else
        r= ((current*10)/25);
    
    if(r>4095) r=4095;
    if(r<5) r=5;
    return r;
}


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
    _maxCurrent=500;
    dac->setVoltage(CurrentToDac(_maxCurrent),false);
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
  int nw=_maxCurrent;

  int scale=100;
  if(nw<450) scale=50;
  if(nw<100) scale=10;
  
  nw+=scale*_rotary->getCount();
  if(nw<10) 
      nw=10; 
  if(_maxCurrent!=nw)
  {
      int command=CurrentToDac(nw);
      dac->setVoltage(command,false);   
      _maxCurrent=nw;
  }
}
/**
 */
int rotaryMaxCurrentControl::getMaxCurrentMa()
{
    return _maxCurrent;
}