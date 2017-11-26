
#include "U8glib.h"
/**
 * 
 */
class powerSupplyScreen
{
public:
        powerSupplyScreen() ;
  void  displayFull(unsigned int  mV, int mA, int maxA,int measure,bool connected);
  void  printStatus(const char *s);



protected:
    U8GLIB_PCD8544 u8g;


};
