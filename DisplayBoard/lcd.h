
/**************************************************************************/
/*!
    @file     lcd.h

    @brief     This Module contains all the configurations and Display details 

    @author   Tworks
    @{

*/
/**************************************************************************/

#pragma once
#include "BoardDefines.hh"

#include <LiquidCrystal.h>
#include <string.h>

#define LCD_LENGTH_CHAR 20    /*!< Length of the LCD in terms of Char*/
#define LCD_HEIGHT_CHAR 4     /*!< Height of the LCD in terms of Char*/
#define NAME1_DISPLAY_POS 0   /*!< position where name1 to be displayed*/
#define VALUE1_DISPLAY_POS 5  /*!< position where value1 to be displayed*/
#define NAME2_DISPLAY_POS 11  /*!< position where name2 to be displayed*/
#define VALUE2_DISPLAY_POS 14 /*!< position where value2 to be displayed*/
#define PAR_SEP_CHAR ' '      /*!< space assigned for parameter seperation*/
#define POWER_FAILURE_MSG1 "POWER FAILURE"
#define POWER_FAILURE_MSG2 "Check Power Supply"
#define PATIENT_DISCONN_MSG1 "PATIENT DISCONNECTED"
#define PATIENT_DISCONN_MSG2 "Check Breath Circuit"
#define PEEP_MSG1 "PEEP NOT ACHIEVED"
#define PEEP_MSG2 "Check Breath Circuit"
#define AIR_SUPPLY_MSG1 "AIR SUPPLY FAILURE"
#define AIR_SUPPLY_MSG2 "Check Breath Circuit"
#define TV_MSG1 "TV NOT ACHIEVED"
#define TV_MSG2 "Check Pressure Limit"
#define TV_MSG3 "BVM, Breath Circuit"
#define OXY_FAILURE_MSG1 "OXYGEN FAILURE"
#define OXY_FAILURE_MSG2 "Check Oxygen Supply"
#define BVM_FAILURE_MSG1 "   BVM FAILURE"
#define BVM_FAILURE_MSG2 "Check BVM"


char paddedValue[10];
LiquidCrystal lcd(DISPLAY_RS_PIN, DISPLAY_EN_PIN, DISPLAY_4_PIN, DISPLAY_3_PIN, DISPLAY_2_PIN, DISPLAY_1_PIN);


/**
 * @enum   ControlStatesDef_T
 * @brief   Following are the Control States during which Sensor data and user inputs are compared 
 * to generate specific commands required for that state.
 */
typedef enum
{
  NO_ERR = 0,
  ERR_OXY,
  ERR_BVM,
  ERR_TV,
  ERR_PEEP,
  ERR_PATIENT_DISCONN,
} ErrorDef_T;





void printPadded(int unpaddedNumber)
{
  if (unpaddedNumber < 10)
  {
    lcd.print("  ");
  }
  else if (unpaddedNumber < 100)
  {
    lcd.print(" ");
  }
  lcd.print(unpaddedNumber);
}
void printEditValue(int unpaddedNumber)
{
  if (unpaddedNumber < 10)
  {
    lcd.print("<");
    lcd.print(" ");
    lcd.print(unpaddedNumber);
    lcd.print(" ");
    lcd.print(">");
  }
  else if (unpaddedNumber < 100)
  {
    lcd.print("<");
    lcd.print(" ");
    lcd.print(unpaddedNumber);
    lcd.print(">");
  }
 else if (unpaddedNumber == 100)
  {
    lcd.print("<");
    lcd.print(unpaddedNumber);
    lcd.print(">");
  }
  
}
void printRRValue(int unpaddedNumber)
{
  if (unpaddedNumber < 10)
  {
    lcd.print("<");
    lcd.print(" ");
    lcd.print(unpaddedNumber);
    lcd.print(">");
  }
  else if (unpaddedNumber < 100)
  {
    lcd.print("<");
    lcd.print(unpaddedNumber);
    lcd.print(">");
  }
}


//String emptyString="aaaaaaaaaaaaaaaaaaaaaaa";
/**@}*/
