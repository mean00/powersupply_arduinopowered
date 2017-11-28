
#pragma once
/**
 */
enum WavDirection
{
  WavNone=0,
  WavLeft,
  WavRight
};
/**
 */
class WavRotary
{
public:
                     WavRotary(int pinA,int pinB );
        WavDirection getSense();
protected:
        Rotary       _rotary;
};
