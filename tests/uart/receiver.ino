#include <LiquidCrystal.h>
#define DISPLAY_1_PIN       13   // Changed
#define DISPLAY_2_PIN       12   // Changed
#define DISPLAY_3_PIN       11   // Changed
#define DISPLAY_4_PIN       10   // Changed
#define DISPLAY_RS_PIN       42   // Changed
#define DISPLAY_EN_PIN       43 
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
int comcnt;
LiquidCrystal lcd(DISPLAY_RS_PIN, DISPLAY_EN_PIN, DISPLAY_4_PIN, DISPLAY_3_PIN, DISPLAY_2_PIN, DISPLAY_1_PIN);
void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);// initialize UART with baud rate of 9600 bps
}

void loop() {
  if(Serial.available()) {
    char data_rcvd = Serial.read();
    lcd.clear();   
    if(data_rcvd == '1') 
    {
     
     lcd.setCursor(0, 0);
    lcd.print(data_rcvd);
     delay(1000);
     Serial.write('0');  
  
    }
    if(data_rcvd == '0') {
     lcd.setCursor(0, 1);
     lcd.print(data_rcvd);
      delay(1000);
      Serial.write('1');    
    }
  }
}

String rxdata_buff;
void serialEvent3() 
{

  while (Serial3.available()>0) 
  {
    char inChar = (char)Serial3.read();
    lcd.clear();
    if (inChar == '$') 
    {
      comcnt = 1;
      rxdata_buff = "";
    }
    if  (comcnt >= 1) 
    {
      rxdata_buff += inChar;
      comcnt = comcnt + 1;
      if (inChar == '&') 
      {
        if (comcnt >= 10) 
        {
          lcd.setCursor(0, 0);
          lcd.print(rxdata_buff);
          
        }
      }
    }
  }

}
