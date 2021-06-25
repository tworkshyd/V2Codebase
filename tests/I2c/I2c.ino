#include <jm_Wire.h>

#include "BoardDefines.h"
 
 
void setup()
{
  Wire.begin();
 
  DebugPort.begin(115200);
  while (!Serial);             // Leonardo: wait for serial monitor
  DebugPort.println("\nI2C Scanner");
}
 
 
void loop()
{
  byte error, address;
  int nDevices;
 
  DebugPort.println("Scanning...");
 
  nDevices = 0;
  for(address = 0; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      DebugPort.print("I2C device found at address 0x");
      if (address<16)
        DebugPort.print("0");
      DebugPort.print(address,HEX);
      DebugPort.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      DebugPort.print("Unknown error at address 0x");
      if (address<16)
        DebugPort.print("0");
      DebugPort.println(address,HEX);
    } else {
		DebugPort.print( "Error  :   ");
		DebugPort.println(error);
	}   
  }
  if (nDevices == 0)
    DebugPort.println("No I2C devices found\n");
  else
    DebugPort.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
