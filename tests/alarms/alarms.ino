#include <LiquidCrystal.h>
#define DISPLAY_1_PIN       13   //  LCD D4 pin to digital pin 13
#define DISPLAY_2_PIN       12   // LCD D5 pin to digital pin 12
#define DISPLAY_3_PIN       11   // LCD D6 pin to digital pin 11
#define DISPLAY_4_PIN       10   // LCD D6 pin to digital pin 10
#define DISPLAY_RS_PIN       43   // LCD RS pin to digital pin 42
#define DISPLAY_EN_PIN       42  // LCD RS pin to digital pin 43


#define O2_CYN_SWITCH 41
#define BUZZER_PIN    22
#define OXY_FAILURE_MSG1 "OXYGEN FAILURE"
#define OXY_FAILURE_MSG2 "Check Oxygen Supply"
static bool blink = true;
LiquidCrystal lcd(DISPLAY_RS_PIN, DISPLAY_EN_PIN, DISPLAY_4_PIN, DISPLAY_3_PIN, DISPLAY_2_PIN, DISPLAY_1_PIN);

void setup() {
   Serial.begin(115200);
   // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
    pinMode(O2_CYN_SWITCH, INPUT_PULLUP);
     pinMode(BUZZER_PIN, OUTPUT);
    checkAlarms();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkAlarms();
}
void checkAlarms() 
{

  int oxySupply = digitalRead(O2_CYN_SWITCH);
  if (oxySupply == LOW ) 
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("       ERROR");
        lcd.setCursor(0, 1);
        lcd.print( OXY_FAILURE_MSG1);
        lcd.setCursor(0, 2);
        lcd.print( OXY_FAILURE_MSG2);
        digitalWrite(BUZZER_PIN, blink);
        Serial.print("Oxygen cylinder Supply Interrupted");
    }else{
         Serial.print("Oxygen cylinder Supply working properly !!!");
      
    }
    delay(5000);
}
