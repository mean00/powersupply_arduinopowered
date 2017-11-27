//

#include "power_screen.h"

//#define DEBUG_MEASURE

/**
 * Small function to turn a float to a string
 * It automatically scale i.e. 0.5 becomes 500 m
 * 
 */
void float2str(char *s,float f,const char *unit)
{
  float scale=1;
  if(f<1.2) // milli something
  {
    f=f*1000.;
     sprintf(s,"%04dm%s",(int)f,unit);
     return;
  }
  
  int zleft=(int)(f);
  if(zleft<10)
    sprintf(s," %1d",zleft);
  else
    sprintf(s,"%2d",zleft);   
  s[2]='.';
  s+=3;
    
  float g=f-(float)(zleft); 
  int right=(int)(g*1000.);
  right=(right+5)/10;
  sprintf(s,"%02d%s",right,unit);
}
 powerSupplyScreen::powerSupplyScreen() : u8g(12, 11, 9, 10, 8) // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
  {
     u8g.setColorIndex(1); // pixel on
     u8g.setFont(u8g_font_ncenB18); //u8g_font_ncenB24);
     u8g.setContrast(105);  //105
     printStatus("*Psu*");
     displayLimit=false;
  }
 
 void powerSupplyScreen::printStatus(const char *s)
 {
      u8g.firstPage();
     do
     {
         u8g.drawStr(1, 32,s);
         //::delay(2000);
     }
      while(u8g.nextPage());  
 }
 /**
  * 
  * @param voltage
  */
void powerSupplyScreen::setVoltage(unsigned int mvoltage) 
{
    float v=mvoltage;
    v=v/1000.;
    float2str(stV,v,"V");
}
/**
 * 
 * @param mA
 * @param maxA
 * @param connected
 */
void powerSupplyScreen::setCurrent(int mA,int maxA,bool connected) 
{     
    float a=mA;
    a=a/1000.;
    
    if(connected)
    {    
        float2str(stA,a,"A");
    }else
    {
        strcpy(stA,"-- Disc --");
    }
    float m=maxA;
    m=m/1000.;    
    float2str(stM,m,"A");
}
/**
 * 
 * @param mA
 * @param rawReading
 * @param limited
 */
void  powerSupplyScreen::setCurrentCalibration(int mA,int rawReading,bool limited)
{
    float a=mA;
    a=a/1000.;
    
    float2str(stA,a,"A");
    sprintf(stM,"%04d-%d",rawReading,limited);
}

/**
 * 
 */
void powerSupplyScreen::refresh()
{    
    u8g.firstPage();
    do
    {
      u8g.setFont(u8g_font_ncenB18); //u8g_font_ncenB24);
      u8g.drawStr(0, 18, stV);          
      u8g.setFont(u8g_font_ncenB12); //u8g_font_ncenB24);
      u8g.drawStr(5, 18+12+2, stA);    
      u8g.setFont(u8g_font_helvR08);
      if(displayLimit)
      {
          u8g.drawRBox(1,18+12+6, 
                       4*6,12,1);
          u8g.setColorIndex(0);
      }
      u8g.drawStr(1, 18+12+12+2+2, "Lim:");          
      u8g.setColorIndex(1);
          
      u8g.drawStr(24,18+12+12+2+2, stM);          
    }
    while(u8g.nextPage());
}

void  powerSupplyScreen::setLimitOn(bool limit)
{
    if(limit)
    {
        displayLimit=!displayLimit;
    }else
        displayLimit=false;
}

