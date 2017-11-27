
#include "U8glib.h"
/**
 * 
 */
class powerSupplyScreen
{
public:
        powerSupplyScreen() ;  
  void  printStatus(const char *s);
  void  setVoltage(unsigned int voltage) ;
  void  setCurrent(int mA,int maxA,bool connected) ;
  void  setCurrentCalibration(int mA,int rawReading,bool limited) ;
  void  setLimitOn(bool limit);
  void  refresh();

protected:
    U8GLIB_PCD8544 u8g;
    char           stV[20]; // voltage
    char           stA[20]; // current
    char           stM[20]; // max current
    bool           displayLimit; // true if current limiting is on

};
