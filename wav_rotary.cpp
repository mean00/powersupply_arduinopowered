
#include "Rotary.h"
#include "wav_rotary.h"



/**
 */
 WavRotary::WavRotary(int pinA,int pinB ) : _rotary(pinA,pinB)
 {
     
 }
 /**
  */
 WavDirection WavRotary::getSense()
 {
     switch(_rotary.process())
     {
        case DIR_CW:   return WavLeft;break;
        case DIR_CCW:  return WavRight;break;
        default:
                        return WavNone;break;
     }
 }

 // EOF
