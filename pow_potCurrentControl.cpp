/**
 Here the max control current is driven by a simple analog pot between GND and VCC
 and connected to pin
 
 */
#include "Arduino.h"
#include <Wire.h>
#include "pow_currentControl.h"
/**
 */
class potMaxCurrentControl : public MaxCurrentControl
{
public:
                            potMaxCurrentControl(int pin) : MaxCurrentControl(pin)
                            {
                               pinMode(_pin, INPUT);        // max current value pin, output
                            }
       void                 run();
       int                  getMaxCurrentMa();
       
protected:
        int             _pin;
        
};
/**
 */
static int evaluatedMaxAmp(int measure)
{
    int v=measure*10-40;
    if(v<0) v=0;
    if(v>5000) v=5000;
    return v;
}
/**
 */
MaxCurrentControl       *potCurrentControl_instantiate(int pin)
{
        return new potMaxCurrentControl(pin);
}
/**
 */
void potMaxCurrentControl::run()
{
    
}
/**
 */
int potMaxCurrentControl::getMaxCurrentMa()
{
    // Read value
    float f=analogRead(_pin);
    f=f*5000./1024.;
    
    return evaluatedMaxAmp((int)f);
}
// EOF