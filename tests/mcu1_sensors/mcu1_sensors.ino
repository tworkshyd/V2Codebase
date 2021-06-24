
#include "BoardDefines.h"

void setup() {
  // put your setup code here, to run once:
 DebugPort.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc1 = analogRead(1);//Inhale
  int adc2 = analogRead(0);//Exhale
  //adc3 = ads.readADC_SingleEnded(3);
  DebugPort.print("AIN0_GP1: "); DebugPort.println(adc1);
  DebugPort.print("AIN1_GP2: "); DebugPort.println(adc2);
  delay(1000);
  
  DebugPort.print("Press the Inhalation and exhalation pipe to see the variations in the Values read through adc (channel 1,0)");
}
