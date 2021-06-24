#include <jm_Wire.h>
#include <Adafruit_ADS1015.h>
#include "BoardDefines.h"

// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

//Adafruit_ADS1115 ads(0x48);
//Adafruit_ADS1115 ads1(0x49);

Adafruit_ADS1115 ads(0x4A);
Adafruit_ADS1115 ads1(0x4B);


void setup(void) 
{
  DebugPort.begin(115200);
  DebugPort.println("Hello!");
  Wire.begin();
  DebugPort.println("Getting single-ended readings from AIN0..3");
  DebugPort.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
  ads1.begin();
}

void loop(void) 
{
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(0);
  
  DebugPort.println();
  DebugPort.print("AIN0_DP1: "); DebugPort.println(adc0);
  DebugPort.print("AIN1_DP2: "); DebugPort.println(adc1);
  DebugPort.print("AIN2_O2: "); DebugPort.println(adc2);
  DebugPort.println(" ");
  
  delay(1000);
  DebugPort.print("Press the Inhalation and exhalation pipe to see the variations in the Values read through adc (channel 1,0)");
}
