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
  const char *ext=" ";

  if(f<1.1) // milli something
  {
    f=f*1000.;
     sprintf(s,"%04dm%s",(int)f,unit);
     return;
  }
  
  int zleft=(int)(f);
  float g=f-(float)(zleft);
  int right=(int)(g*10.);
  if(zleft<9)
    sprintf(s," %1d",zleft);
  else
    sprintf(s,"%2d",zleft);   
  s[2]='.';
  s+=3;
  sprintf(s,"%01d",right);
  s+=1;
  sprintf(s,"%s%s",ext,unit);
  
}
 powerSupplyScreen::powerSupplyScreen() : u8g(12, 11, 9, 10, 8) // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
  {
     u8g.setColorIndex(1); // pixel on
     u8g.setFont(u8g_font_ncenB18); //u8g_font_ncenB24);
     u8g.setContrast(105);  //105
     printStatus("*Psu*");
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
 
void powerSupplyScreen::displayFull(int  mV, int mA, int maxA,int measure,bool connected)
{
    float v=mV;
    v=v/1000.;
    char stV[20];
    float2str(stV,v,"V");

    char stA[20];
    float a=mA;
    a=a/1000.;
    
    if(connected)
    {    
        float2str(stA,a,"A");
    }else
    {
        strcpy(stA,"-- Disc --");
    }

    char stM[20];
    float m=maxA;
    m=m/1000.;
    
    float2str(stM,m,"A");

    
    char stS[20];
    sprintf(stS,"%04d",measure);
    
    u8g.firstPage();
    do
    {
      u8g.setFont(u8g_font_ncenB18); //u8g_font_ncenB24);
      u8g.drawStr(1, 18, stV);          
      u8g.setFont(u8g_font_ncenB12); //u8g_font_ncenB24);
      u8g.drawStr(5, 18+12+2, stA);    
      //u8g.setFont(u8g_font_ncenB10);
      //u8g.setFont(u8g_font_6x10);
      u8g.setFont(u8g_font_helvR08);
#ifdef DEBUG_MEASURE
      u8g.drawStr(1, 18+12+12+2+2, stS);          
#else
      u8g.drawStr(1, 18+12+12+2+2, "Lim:");          
#endif
      u8g.drawStr(24,18+12+12+2+2, stM);          
    }
    while(u8g.nextPage());
}
