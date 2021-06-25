#include "BoardDefines.h"

static const String commands[] =
{ "$VMST0000&",        /**< stop stepper motor*/
  "$VMST0001&",        /**< start stepper motor*/
  "$VMO20100&",        /**< oxygen solenoid cylinder line off*/
  "$VMO20101&",       /**< oxygen solenoid cylinder line on*/
  "$VMO20200&",       /**< oxygen solenoid hospital line off*/
  "$VMO20201&",       /**< oxygen solenoid hospital line on*/
  "$VMSV0100&",       /**< inhalation  solenoid off*/
  "$VMSV0101&",       /**< inhalation  solenoid on*/
  "$VMSV0200&",       /**< exhalation  solenoid off*/
  "$VMSV0201&",       /**< exhalation  solenoid on*/
  "$VMSV0300&",       /**< peak pressure relief off*/
  "$VMSV0301&",       /**< peak pressure relief on*/
  "$VMP1xxxx&",       /**< set tidal volume */
  "$VMP2xxxx&",       /**< set BPM */
  "$VMP3xxxx&",       /**< set peak pressure*/
  "$VMP4xxxx&",       /**< set Fio2*/   
  "$VMP5xxxx&",       /**< set inhalation exhalation ratio */
  "$VMPP0000&",       /**< slave entering parameter edit */
  "$VMPP1111&",       /**< slave completed parameter edit */
  "$VMIN0000&",       /**< initialize master*/
  "$VMIN0001&",       /**< initialize stepper module*/
  "$VMIN0002&",       /**< initialize stepper module*/
  "$VMIN0003&"        /**< initialize breath detection*/
};
void setup() {
  DebugPort.begin(115200);       // initialize UART with baud rate of 9600 bps
}

void loop() {
  if(DebugPort.available()) {
    char data_rcvd = DebugPort.read();   // read one byte from serial buffer and save to data_rcvd

    if(data_rcvd == '1') 
    {
  DebugPort.write('0'); 
  delay(1000); 
  
    }
    if(data_rcvd == '0') {
        DebugPort.write('1');
        delay(1000);
    }
  }
    
 DebugPort.write('1');
 for(int i=0; i<=22;i++){
  Serial3.print(commands[i]);
 }
                      
}
