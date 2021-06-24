/*
  DS1307 RTC (Real-Time-Clock) Example

  Uno       A4 (SDA), A5 (SCL)
  Mega      20 (SDA), 21 (SCL)
  Leonardo   2 (SDA),  3 (SCL)
 */

#include <jm_Wire.h>
#include <DS1307.h>

#include "BoardDefines.h"

//#include <DS1307RTC.h>
DS1307 rtc;


void setup()
{
  /*init Serial port*/
  DebugPort.begin(115200);
  while(!Serial); /*wait for serial port to connect - needed for Leonardo only*/

  /*init RTC*/
  DebugPort.println("Init RTC...");

  /*only set the date+time one time*/
  rtc.set(0, 0, 15, 14, 10, 2020); /*08:00:00 24.12.2014 //sec, min, hour, day, month, year*/

  /*stop/pause RTC*/
  // rtc.stop();

  /*start RTC*/
  rtc.start();
}


void loop()
{
  uint8_t sec, min, hour, day, month;
  uint16_t year;

  /*get time from RTC*/
  rtc.get(&sec, &min, &hour, &day, &month, &year);

  /*serial output*/
  DebugPort.print("\nTime: ");
  DebugPort.print(hour, DEC);
  DebugPort.print(":");
  DebugPort.print(min, DEC);
  DebugPort.print(":");
  DebugPort.print(sec, DEC);

  DebugPort.print("\nDate: ");
  DebugPort.print(day, DEC);
  DebugPort.print(".");
  DebugPort.print(month, DEC);
  DebugPort.print(".");
  DebugPort.print(year, DEC);

  /*wait a second*/
  delay(1000);
}
