
#include "./memory/memory.cpp"
#define MEMORY_READ_TIME 1
#define MEMORY_WRITE_TIME 1
#include <jm_Wire.h>
int writeValue = 1;

int addr_write_to_mem = EEPROM_ADDR;
void setup() {
 Serial.begin(115200);
 Serial.print("EEPROM TEST:");
 uint8_t eepStatus = hbad_mem.begin(hbad_mem.twiClock400kHz);   
  if (eepStatus) {
    Serial.print(F("extEEPROM.begin() failed, status = "));
    Serial.println(eepStatus);
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
 
  Serial.print("WriteValue:");
   Serial.println(writeValue); 
    
#if MEMORY_WRITE_TIME
 Serial.print("Time taken by write operation in EEPROM :");
  Serial.println((millis()-memWriteTime));
#endif  
  delay(1000);

    #if MEMORY_READ_TIME
  unsigned long memReadTime = millis();
#endif   

   retrivedValue = retrieveParam(addr_write_to_mem);
   Serial.print("Retrieved :");
   Serial.println(retrivedValue);
   
   #if MEMORY_WRITE_TIME
 Serial.print("Time taken by read operation in EEPROM :");
  Serial.println((millis()-memReadTime));
#endif  
 if(writeValue == retrivedValue){
  Serial.println("written and Retrived values are Same");
 }else{
  Serial.println("written and Retrived values are not Same");
 }
 writeValue++;
}
