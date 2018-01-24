
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
        D2 : Load On LED,  output
        D3 : Fan PWM command output
        D4 : Load switch , input
        D5 : Relay command, output
 
        D8/D12 LCD
  
        A1    : max current input
        A4/A5 : I2C
        
 
 */


#include <Wire.h>
#include "simpler_INA219.h"
#include "power_screen.h"
#include "Rotary.h"
#include "wav_irotary.h"
#include <Adafruit_MCP4725.h>


extern void mySetup();
extern void myRun();

void setup(void) 
{
    mySetup();
}

void loop(void) 
{
    myRun();
}
// EOF

