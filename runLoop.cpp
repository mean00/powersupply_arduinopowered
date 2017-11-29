
/**
 * Simple power supply monitored by arduino
 * - We have an INA219 monitoring voltage and current
 * - Basic relay (with protection diode) disconnecting output when active
 * - Nokia 5110 screen printing out voltage and amps
 *
 * Gettings the current limit is a bit tricky as it is done in anolog world through a resistor
 */
/**
    Pin Out :
        D2 : LoadOn LED,  output
        D3 : Fan PWM command output
        D4 : Load switch , input
        D5 : Relay command, output

        D6/D7 : Rotary encoder

        D8/D12 LCD

        A1    : max current set (maxCurrent=value*10-40,output)
        A2    : Current limiting mode (input,active low)
        A4/A5 : I2C


 */


#include <Wire.h>
#include "simpler_INA219.h"
#include "power_screen.h"

//#define TESTMODE


#define SHUNT_VALUE 20 // 20 mOhm
#define SIGN  1. //-1. // in case you inverted vin+ and vin*

//#define VERBOSE 1
//#define TESTMODE

#if 0
#define MARK(x) screen->printStatus("." #x ".")
#else
#define MARK(x) {}
#endif

static void setRelayState(bool state);



powerSupplyScreen *screen;
simpler_INA219 *voltageSensor; // Declare and instance of INA219 : High side voltage
simpler_INA219 *currentSensor; // Declare and instance of INA219 : Low side current

bool connected=false; // is the relay disconnecting voltage ? false => connected

int ccModePin    = A2;
int maxAmpPin    = A1;     // A1 is the max amp pin voltage driven
int relayPin     = 5;      // D6 : L
int buttonPin    = 4;   // choose the input pin (for a pushbutton)
int buttonLedPin = 2;   // Led in on/off button (the above button)

int bounce=0;
#define ANTIBOUNCE 2
#define NEXT_CYCLE() delay(250)

/**
 *
 */
int evaluatedMaxAmp(int measure)
{
    int v=10*measure-40;
    if(v<0) v=0;
    return v;
}

/**
 *
 *
 *
 */
void mySetup(void)
{
  pinMode(ccModePin, INPUT);         // max current value pin, input
  pinMode(ccModePin, INPUT_PULLUP);  // cc mode pin, active low
  pinMode(relayPin,  OUTPUT);         // declare relay as output
  pinMode(buttonPin, INPUT_PULLUP);  // declare pushbutton as input
  pinMode(buttonLedPin,OUTPUT);      // declare button led as ouput
  digitalWrite(buttonLedPin,0);
 // D3 is PWM for fan
  pinMode(3, OUTPUT);  // D3
  TCCR2A = _BV(COM2A1)| _BV(WGM21) | _BV(WGM20)| _BV(COM2B1) ;
  //TCCR2B = _BV(CS22);
  //OCR2A = 180;
  OCR2B = 120;

  Serial.begin(9600);
  Serial.print("Start\n");

  Serial.print("Setting up screen\n");
  screen=new powerSupplyScreen;
  Serial.print("Screen Setup\n");
  Serial.print("Setup done\n");

  screen->printStatus(2,"Low side");
  currentSensor=new simpler_INA219(0x40,SHUNT_VALUE);   // 22 mOhm low side current sensor
  currentSensor->setMultiSampling(2); // average over 4 samples
  currentSensor->begin();
  screen->printStatus(3,"High side");
  voltageSensor=new simpler_INA219 (0x44,100); // we use that one only for high side voltage
  voltageSensor->begin();
  setRelayState(false);
  delay(150);
  screen->printStatus(4,"..");

}
/**
 * drive relay
 * If state is on; the relay disconnects the output
 * if state if off, output is connected to power supply
 */
void setRelayState(bool state)
{
  if(state)
  {
    // Auto zero the low side ina
    currentSensor->autoZero();
    digitalWrite(buttonLedPin,HIGH);
    digitalWrite(relayPin, HIGH);
  }
  else
  {
    digitalWrite(buttonLedPin,LOW);
    digitalWrite(relayPin, LOW);
  }
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
void myRun(void)
{
  float busVoltage = 0, busVoltageLowSide=0;
  float current = 0; // Measure in milli amps


  busVoltage = voltageSensor->getBusVoltage_V();
  busVoltageLowSide=currentSensor->getBusVoltage_V();
  current = currentSensor->getCurrent_mA();


  float currentInMa=SIGN*current; // it is inverted (?)
  if(currentInMa<0)  // clamp noise
      currentInMa=0;


  int maxMeasure=analogRead(maxAmpPin);
  int maxAmp=evaluatedMaxAmp(maxMeasure);

  bool err=false;
#ifndef TESTMODE
  if(busVoltage>30.) // cannot read
  {
      Serial.print("Voltage overflow\n");
      screen->printStatus(1,"Err HS");
      err=true;
  }
   if(busVoltageLowSide>30.) // cannot read
  {
      Serial.print("Low side HS\n");
      screen->printStatus(2,"Err LS");
      err=true;
  }
  if(err)
  {
      NEXT_CYCLE();
      return;
  }
#endif

  MARK(5);

  bool ccmode=false;
  int  cpin=analogRead(ccModePin);

  if(cpin<400) ccmode=true;

  busVoltage=busVoltage-(currentInMa*SHUNT_VALUE)/1000000.; // compensate for voltage drop on the shunt

  screen->setVoltage(busVoltage*1000);

#ifdef TESTMODE
  screen->setCurrentCalibration(currentInMa,maxMeasure,ccmode);
#else
  screen->setCurrent(currentInMa,maxAmp,connected);
  screen->setLimitOn(ccmode);
#endif

  screen->refresh();
  NEXT_CYCLE();
  MARK(6);

  if(buttonPressed()) // button pressed
  {
    connected=!connected;
    setRelayState(connected); // when relay is high, the output is disconnected
  }
}
// EOF
