

// include the library code:
#include <LiquidCrystal.h>
#define DISPLAY_1_PIN       13   //  LCD D4 pin to digital pin 13
#define DISPLAY_2_PIN       12   // LCD D5 pin to digital pin 12
#define DISPLAY_3_PIN       11   // LCD D6 pin to digital pin 11
#define DISPLAY_4_PIN       10   // LCD D6 pin to digital pin 10
#define DISPLAY_RS_PIN       42   // LCD RS pin to digital pin 42
#define DISPLAY_EN_PIN       43  // LCD RS pin to digital pin 43


#define DISPLAY_PROCESSING_TIME_TESTING 1
#define ROW1_PROCESSING_TIME_TESTING 1
#define ROW2_PROCESSING_TIME_TESTING 1
#define ROW3_PROCESSING_TIME_TESTING 1
#define ROW0_PROCESSING_TIME_TESTING 1


LiquidCrystal lcd(DISPLAY_RS_PIN, DISPLAY_EN_PIN, DISPLAY_4_PIN, DISPLAY_3_PIN, DISPLAY_2_PIN, DISPLAY_1_PIN);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  
}

void loop() {
 lcdWriteTest();
 lcdPrintTest();
 lcdRowPrint();
 lcdRowWrite();
 setCursorTime();  
}

void lcdRowPrint(){
  #if ROW0_PROCESSING_TIME_TESTING
  unsigned long dstarttime = millis();
#endif 
          lcd.setCursor(0, 0);
           lcd.print("TV  350 RR 19 IE 1:1"); //row0
#if ROW0_PROCESSING_TIME_TESTING
 Serial.print("display row time  in milli seconds using lcdprint:");
  Serial.println((millis()-dstarttime));
#endif  
           
}

void lcdRowWrite(){
  #if ROW1_PROCESSING_TIME_TESTING
  unsigned long dstarttime = millis();
#endif 
          lcd.setCursor(0, 1);
           lcd.write("TV  350 RR 19 IE 1:1"); //row1
#if ROW1_PROCESSING_TIME_TESTING
 Serial.print("display row time  in milli seconds using lcdwrite:");
  Serial.println((millis()-dstarttime));
#endif  
           
}
void setCursorTime(){
  unsigned long dstarttime = micros();
  lcd.setCursor(0, 2);
 Serial.print("Time taken to set Cursor");
  Serial.println((micros()-dstarttime)); 
   lcd.write("TV  350 RR 19 IE 1:1"); //row1
}

void lcdWriteTest(){
  #if DISPLAY_PROCESSING_TIME_TESTING
  unsigned long dstarttime = millis();
#endif
      lcd.clear(); 
        {

          lcd.setCursor(0, 0);
           lcd.write("TV  350 RR 19 IE 1:1"); //row0
        }   
        { //row1

        lcd.setCursor(0, 1);
        lcd.write("TVi 350 ");
        lcd.setCursor(8,1);
        lcd.write("PIP ");
        lcd.write(" ");
        lcd.write("23.7");

        }
        
        {
          //row2
         lcd.setCursor(0, 2); 
         lcd.write("TVe 350");
         lcd.write(" Plat 23.7"); 
        }
         
        {   
          //row3
          lcd.setCursor(0, 3);
          lcd.write("O2");
          lcd.setCursor(8, 3);
          lcd.write("PEEP");
            lcd.write(" 23.7");
             lcd.setCursor(19,3); 
            lcd.write("R");
        }
        
#if DISPLAY_PROCESSING_TIME_TESTING
 Serial.print("total display time  in milli seconds using lcdwrite:");
  Serial.println((millis()-dstarttime));
#endif  
}
void lcdPrintTest(){
  #if DISPLAY_PROCESSING_TIME_TESTING
  unsigned long dstarttime = millis();
#endif
 unsigned long screenClrtime = millis();
      lcd.clear(); 
 Serial.print("Time taken to clear screen :");
  Serial.println((millis()-screenClrtime));       
        {

          lcd.setCursor(0, 0);
           lcd.print("TV  350 RR 19 IE 1:1"); //row0
        }   
        { //row1

        lcd.setCursor(0, 1);
        lcd.print("TVi 350 ");
        lcd.setCursor(8,1);
        lcd.print("PIP ");
        lcd.print(" ");
        lcd.print("23.7");

        }
        
        {
          //row2
         lcd.setCursor(0, 2); 
         lcd.print("TVe 350");
         lcd.print(" Plat 23.7"); 
        }
         
        {   
          //row3
          lcd.setCursor(0, 3);
          lcd.print("O2");
          lcd.print("21");
          lcd.setCursor(8, 3);
          lcd.print("PEEP");
            lcd.print(" 23.7");
             lcd.setCursor(19,3); 
            lcd.print("R");
        }
        
#if DISPLAY_PROCESSING_TIME_TESTING
 Serial.print("total display time  in milli seconds using lcdprint:");
  Serial.println((millis()-dstarttime));
#endif  
}
