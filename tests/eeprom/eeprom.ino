#include "BoardDefines.h"

#include "./memory/memory.cpp"
#define MEMORY_READ_TIME 1
#define MEMORY_WRITE_TIME 1
#include <jm_Wire.h>
int writeValue = 1;

int addr_write_to_mem = EEPROM_ADDR;
void setup() {
 DebugPort.begin(115200);
 DebugPort.print("EEPROM TEST:");
 uint8_t eepStatus = hbad_mem.begin(hbad_mem.twiClock400kHz);   
  if (eepStatus) {
    DebugPort.print(F("extEEPROM.begin() failed, status = "));
    DebugPort.println(eepStatus);
    while (1);
  }

}

void loop() {
   delay(1000);
 int retrivedValue;
    #if MEMORY_WRITE_TIME
  unsigned long memWriteTime = millis();
#endif

 storeParam(EEPROM_ADDR,writeValue);
 
  DebugPort.print("WriteValue:");
   DebugPort.println(writeValue); 
    
#if MEMORY_WRITE_TIME
 DebugPort.print("Time taken by write operation in EEPROM :");
  DebugPort.println((millis()-memWriteTime));
#endif  
  delay(1000);

    #if MEMORY_READ_TIME
  unsigned long memReadTime = millis();
#endif   

   retrivedValue = retrieveParam(addr_write_to_mem);
   DebugPort.print("Retrieved :");
   DebugPort.println(retrivedValue);
   
   #if MEMORY_WRITE_TIME
 DebugPort.print("Time taken by read operation in EEPROM :");
  DebugPort.println((millis()-memReadTime));
#endif  
 if(writeValue == retrivedValue){
  DebugPort.println("written and Retrived values are Same");
 }else{
  DebugPort.println("written and Retrived values are not Same");
 }
 writeValue++;
}
