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
/**
 */


/**
 *
 */
static int evaluatedMaxAmp(int measure)
{
    int v=4+measure/10;
    if(v<0) v=0;
    if(v>1000) v=1000;
    return v;
}

class rotaryMaxCurrentControl : public MaxCurrentControl
{
public:
                            rotaryMaxCurrentControl(int pin) : MaxCurrentControl(pin)
                            {
                              pinMode(_pin, OUTPUT);  
                              _rotary=new WavRotary(6,7);
                              
                            }
       void                 run();
       int                  getMaxCurrentMa();       
       
protected:
        int _pin;
        WavRotary           *_rotary;        // rotary encoder
};
/**
 */
MaxCurrentControl    *rotaryCurrentControl_instantiate(int pin)
{
    return new rotaryMaxCurrentControl(pin);
}
/**
 */
void rotaryMaxCurrentControl::run()
{
  _maxCurrent+=50*_rotary->getCount();
  if(_maxCurrent<50) 
      _maxCurrent=50;
  if(_maxCurrent>5000) 
      _maxCurrent=5000;

  analogWrite(_pin,evaluatedMaxAmp(_maxCurrent)); // this does not work, it is a 8 bit pwm DAC
}
/**
 */
int rotaryMaxCurrentControl::getMaxCurrentMa()
{
    return _maxCurrent;
}