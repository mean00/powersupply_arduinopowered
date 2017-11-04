
/**
 * Simple power supply monitored by arduino
 * - We have an INA219 monitoring voltage and current
 * - Basic relay (with protection diode) disconnecting output when active
 * - Nokia 5110 screen printing out voltage and amps
 * 
 * Gettings the current limit is a bit tricky as it is done in anolog world through a resistor
 */


 /**
  *   maxA (mA)    Value
  *    
  *    2000         610  3.2
  *    1500         430
  *    1000         266
  *    750          160
  *    500          107
  *    300          60
  *    200          28
  *    100          6
  *    60           0
  * 
  * 
  */
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "power_screen.h"

#if 0
#define MARK(x) screen->printStatus("." #x ".")
#else
#define MARK(x) {}
#endif


powerSupplyScreen *screen;
Adafruit_INA219 sensor219; // Declare and instance of INA219

float currentBias=300./1000.; // to correct current bias

bool connected=false; // is the relay disconnecting voltage ? false => connected

int relayPin = 5;      // D6 : L
int buttonPin = 4;   // choose the input pin (for a pushbutton)
int buttonLedPin = 2;   // Led in on/off button (the above button)
int maxAmpPin=A1;     // A1 is the max amp pin voltage driven
int bounce=0;
#define ANTIBOUNCE 2
#define NEXT_CYCLE() delay(250)
//#define VERBOSE 1
//#define TESTMODE

#define MAX_EVAL_POINTS 10
const int extrapol[MAX_EVAL_POINTS][2]=
{
  {100,2},
  {250,40},
  {500,100},
  {750,163},
  {1000,256},
  {1250,350},
  {1500,450},
  {2000,630},
  {2300,760},
  {3400,1024}  
};
/**
 * 
 */
int evaluatedMaxAmp(int measure)
{
  if(measure<6) return 100;
  if(measure>1024) return 3500;

  for(int i=0;i<MAX_EVAL_POINTS-1;i++)
  {
    if(measure>=extrapol[i][1] && measure<extrapol[i+1][1])
    {
            float scale=measure-extrapol[i][1];
            scale=scale/(float)(extrapol[i+1][1]-extrapol[i][1]);

            float r=extrapol[i+1][0]-extrapol[i][0];
            r=r*scale;
            r+=extrapol[i][0];
            return (int)(r);
    }
  }
  return 3500;
}

/**
 * 
 * 
 * 
 */
void setup(void) 
{

  pinMode(relayPin, OUTPUT);  // declare relay as output
  pinMode(buttonPin, INPUT_PULLUP);    // declare pushbutton as input
  pinMode(buttonLedPin,OUTPUT); // declare button led as ouput 
  digitalWrite(buttonLedPin,0);
 // D3 is PWM for fan
  pinMode(3, OUTPUT);  // D3
  TCCR2A = _BV(COM2A1)| _BV(WGM21) | _BV(WGM20)| _BV(COM2B1) ;
  //TCCR2B = _BV(CS22);
  //OCR2A = 180;
  OCR2B = 120;

  Serial.begin(9600);   
  Serial.print("Start\n"); 
  
#ifdef  TESTMODE  
#else
  sensor219.begin();
#endif
  setRelayState(false);
  Serial.print("Setting up screen\n");
  screen=new powerSupplyScreen;
  Serial.print("Screen Setup\n");
  Serial.print("Setup done\n");
  delay(150);
  screen->printStatus(".1.");
  
}
/**
 * drive relay
 * If state is on; the relay disconnects the output
 * if state if off, output is connected to power supply
 */
void setRelayState(bool state)
{
  if(state)
    digitalWrite(relayPin, HIGH);
  else
    digitalWrite(relayPin, LOW);
}
/**
 *    Check for button press
      a high enough value of NEXT_CYLE should get ride of the bumps on the button (> ~ 20 ms)
 */
bool buttonPressed()
{
  if(bounce)
  {
    bounce--;
    return false;
  }
  int r=digitalRead(buttonPin);
  if(!r)
  {
     Serial.print("Press\n");
     bounce=ANTIBOUNCE;
     return true;
  }
  return false;
}
/**
 * 
 */
void loop(void) 
{
  float busVoltage = 0;
  float current = 0; // Measure in milli amps
  float power = 0;
  MARK(3);

#ifdef TESTMODE
  int maxAmp=analogRead(maxAmpPin);
  char amp[16];
  sprintf(amp,"READ : %04d",maxAmp);
//  u8g.drawStr(3,48,amp);
   Serial.print(amp);

#else

  busVoltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA()+currentBias;
  power = busVoltage * (current/1000); // Calculate the Power

  float currentInMa=current+currentBias;
  MARK(4);

  
  int maxMeasure=analogRead(maxAmpPin);
  int maxAmp=evaluatedMaxAmp(maxMeasure);

  if(busVoltage>30.) // cannot read
  {
      screen->printStatus("Error");
      NEXT_CYCLE();       
      return;
  }
  if(!connected)
  {
//    strcpy(stA,"-- DISC --");
  }
  MARK(5);
  
  
#if 1 //def VERBOSE
  Serial.print(busVoltage);
  Serial.print("-----------------\n");
#endif  
  screen->displayFull(busVoltage*1000,currentInMa,maxAmp,maxMeasure,connected);

#endif
  if(buttonPressed()) // button pressed
  {
    connected=!connected;
    setRelayState(connected); // when relay is high, the output is disconnected    
  }
  
  NEXT_CYCLE();   
  MARK(6);
}
// EOF

