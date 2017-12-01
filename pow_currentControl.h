
#pragma once


class MaxCurrentControl
{
public:
                            MaxCurrentControl(int pin)
                            {
                              _pin=pin;
                              _maxCurrent=200;
                            }
virtual void                 run()=0;
virtual int                  getMaxCurrentMa()=0;

protected:
        int  _pin;
        int  _maxCurrent;


};

MaxCurrentControl       *potCurrentControl_instantiate(int pin);
MaxCurrentControl       *rotaryCurrentControl_instantiate(int pin);
