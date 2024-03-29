#include "BoardDefines.h"
#include <EEPROM.h>
#include <jm_Wire.h>
bool machineOn = false;
#include "pin_new_v3.h"
#include "./lcd_display/ctrl_display.h"
#include "./memory/memory.cpp"
#include "./sensors/sensormanager.h"
#include "./sensors/sensormanager.cpp"
#include "./state_control/statecontrol.h"
#include "./state_control/statecontrol.cpp"
#include "./encoder/encoder.c"

#include "./lcd_display/ctrl_display.cpp"
#include <avr/wdt.h>
#include "debug.h" 

String rxdata;
int comcnt;

bool gCntrlSerialEventRecvd = false;

bool powerSupplyFailure = false;
bool gasSuppluFailure = false;
bool mechFailSafeValve = false;

const uint8_t wdog_timer = WDTO_8S;

displayManager dM;

ErrorDef_T gErrorState = NO_ERR;

void setup()
{
  int err = 0;

#if 0

  WDT_Clear();
  err = WDT_Cookie_Check();
  if (err == -1)
  {
    VENT_DEBUG_ERROR("WDOG Cookie check failed", err);
  }

   WDT_Set(wdog_timer);
  VENT_DEBUG_ERROR("WDOG Timer enabled for value", wdog_timer);
  
#endif

  initCtrlStateControl();
  
  DebugPort.begin(115200);

  lcd.begin(LCD_LENGTH_CHAR, LCD_HEIGHT_CHAR);
  VENT_DEBUG_ERROR("Initialization Started", 0);
  pinMode(DISPLAY_BACK_LED_PIN, OUTPUT);

  digitalWrite(DISPLAY_BACK_LED_PIN, HIGH);
  lcd.createChar(DP_UP_TR, upTriaChar);
  lcd.createChar(DP_DW_TR, dwnTriaChar);
  lcd.createChar(DW_POT_AR, filledDownArrowCustomChar);
  lcd.createChar(ROTATE_CHAR, rotate2customChar);
  lcd.createChar(EDIT_CHAR, pressEditCustomChar);
  lcd.createChar(SAVE_CHAR, saveCustomChar2);
  lcd.createChar(ERR_CHAR, errorCustomChar);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
  pinMode(LED_5_PIN, OUTPUT);
  pinMode(LED_6_PIN, OUTPUT);

  digitalWrite(LED_1_PIN, HIGH);

  pinMode(O2_CYN_SWITCH, INPUT_PULLUP);
  pinMode(RESET_SWITCH, INPUT_PULLUP);
  pinMode(DISP_ENC_CLK, INPUT_PULLUP);
  pinMode(DISP_ENC_DT, INPUT_PULLUP);
  pinMode(DISP_ENC_SW, INPUT_PULLUP);
  pinMode(ADS115_INT_PIN, INPUT_PULLUP);
  pinMode(ADS115_INT_PIN_1, INPUT_PULLUP);

  digitalWrite(LED_2_PIN, HIGH);
  
  VENT_DEBUG_ERROR("LCD Module Init Done", 0);

  Wire.setClock(4000000L);
  Wire.begin();
  VENT_DEBUG_ERROR("Wire Init Done", 0);

  hbad_mem.begin();
  VENT_DEBUG_ERROR("Ext EEPROM Init Done", 0);

  Serial3.begin(115200);
  VENT_DEBUG_ERROR("Serial3 Init Done", 0);


  getAllParamsFromMem();
  VENT_DEBUG_ERROR("Parameter read from EEPROM Done", 0);

  /// indicate the sensors to be inited
  int flags = ( O2 | DP_A0 |DP_A1 ) ; 
  err = sM.init( (sensor_flags_e)flags );
  //err = sM.init(DP_A0);
  
  VENT_DEBUG_ERROR("Sensors Init Done", 0);
  if (err < 1)

  {
    VENT_DEBUG_ERROR("Sensors Init *Failed*", 0);
  }

  digitalWrite(LED_3_PIN, HIGH);

  delay(1000);

  attachInterrupt(digitalPinToInterrupt(DISP_ENC_SW), isr_processSwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(DISP_ENC_CLK), isrEncoderClk, RISING);
  attachInterrupt(digitalPinToInterrupt(DISP_ENC_DT), isrEncoderDt, RISING);
  VENT_DEBUG_ERROR("Enable Rotator Button Interrupts Done", 0);
  
  drawSplashScreen(sM,dM);
  VENT_DEBUG_ERROR("Initial Screen Setup Done ", 0);

  checkAlarms();
  VENT_DEBUG_ERROR("Check Power Done ", 0);

  sendDefaultParams();
  VENT_DEBUG_ERROR("Param Set Default - Done ", 0);

  VENT_DEBUG_ERROR("Initialization Complete ", 0);
  dM.clearDisplay();

  digitalWrite(LED_4_PIN, HIGH);
  
} 

void sendDefaultParams()
{
  //VENT_DEBUG_FUNC_START();
  mcu0_enable_sensors_pressure(false);
  delay(100);
  mcu0_enable_sensors_voltage(false);
  delay(100);
  Ctrl_send_packet(tidl_volu.parm_name, params[E_TV].value_curr_mem);
  delay(100);
  Ctrl_send_packet(resp_rate.parm_name, params[E_BPM].value_curr_mem);
  delay(100);
  Ctrl_send_packet(inex_rati.parm_name, params[E_IER].value_curr_mem);
  delay(100);
  Ctrl_send_packet(peak_press.parm_name, params[E_PIP].value_curr_mem);
  delay(100);
  send_calvalue(SENSOR_PRESSURE_A0);
  delay(100);
  send_calvalue(SENSOR_PRESSURE_A1);
  delay(100);
  sM.enable_sensor(O2);
  //VENT_DEBUG_FUNC_END();
}

void checkAlarms()
{
  VENT_DEBUG_FUNC_START();
  gErrorState = NO_ERR;

  int oxySupply = digitalRead(O2_CYN_SWITCH);
 
  if (breathCount > 2)
  {
    if (machineOn == true && oxySupply == LOW)
    {

#if ENABLE_O2_SUPPLY    
      gErrorState = ERR_OXY;
#endif

    }
    if (bvmFailure)
    {
      gErrorState = ERR_BVM;
    }
    if (NO_ERR != gErrorState)
    {
      dM.errorDisplay(gErrorState);
      VENT_DEBUG_ERROR("Incorrect State", gErrorState);
    }
  }
  VENT_DEBUG_FUNC_END();
}


float data_sensors[MAX_SENSORS] = {0};

static unsigned long endtime = 0;

/* Project Main loop */

void loop()
{
  int index = 2;
  int err = 0;
#if PRINT_PROCESSING_TIME
  unsigned long starttime = millis();
  DebugPort.print("L:strt_ts ");
  DebugPort.println(starttime);
#endif
  checkAlarms();
 // wdt_reset();
  VENT_DEBUG_FUNC_START();

  for (; index < MAX_SENSORS; index++)
  {
    data_sensors[index] = 0.0;
    data_sensors[index] = sM.capture_and_read_data((sensor_e)index);
  }
  
#if PRINT_PROCESSING_TIME
 // DebugPort.print("sensor module processing time:");
 // DebugPort.println((millis() - starttime));
 // unsigned long dstarttime = millis();
  
#endif
  //VENT_DEBUG_ERROR("Error State: ", gErrorState);
  if (NO_ERR == gErrorState)
  {
    dM.displayManagerloop(&data_sensors[0], sM);
  }
  else
  {
    dM.errorDisplay(gErrorState);
    gErrorState = NO_ERR;
  }
#if PRINT_PROCESSING_TIME
 // DebugPort.print("display module processing time:");
 // unsigned long ctrlsm_starttime = millis();
 //  DebugPort.println((ctrlsm_starttime - dstarttime));
#endif
  if (gCntrlSerialEventRecvd == true)
  {
    
    gCntrlSerialEventRecvd = false;
    Ctrl_ProcessRxData(dM);
  }

  Ctrl_StateMachine_Manager(&data_sensors[0], sM, dM);
#if PRINT_PROCESSING_TIME
 // DebugPort.print("Ctrl_StateMachine_Manager processing time:");
 // DebugPort.println(millis() - ctrlsm_starttime);
#endif
  if (digitalRead(RESET_SWITCH) == LOW)
  {
    //reset switch.
    if (machineOn == true)
    {
      //machineOn = false;
      Serial3.print(commands[STPR_STP]);
      Ctrl_Stop();
      breathCount = 0;
      gErrorState = NO_ERR;
    }
    else if (machineOn == false)
    {
      //machineOn = true;
      Ctrl_Start();
      breathCount++;
    }
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }

 // wdt_reset(); //Reset watchdog timer in case there is no failure in the loop
               // VENT_DEBUG_ERROR("End of main process loop ", 0);
#if PRINT_PROCESSING_TIME
  DebugPort.print("loop:");
  endtime = millis();
  DebugPort.println((millis() - starttime));
  DebugPort.print("L:stp_ts ");
  DebugPort.println(endtime);
#endif
  VENT_DEBUG_FUNC_END();
}






String rxdata_buff;
#if 1
void serialEvent3()
{
  VENT_DEBUG_FUNC_START();
  while (Serial3.available())
  {
    char inChar = (char)Serial3.read();
    
    if (inChar == '$')
    {
      comcnt = 1;
      rxdata_buff = "";
    }
    if (comcnt >= 1)
    {
      rxdata_buff += inChar;
      comcnt = comcnt + 1;
      if (inChar == '&')
      {
        if (comcnt >= 10)
        {
          //DebugPort.print("Packet Received:");
          //DebugPort.println(rxdata_buff);
          Ctrl_store_received_packet(rxdata_buff);
          gCntrlSerialEventRecvd = true;
          VENT_DEBUG_INFO("Received Packet", rxdata_buff);
        }
      }
    }
  }
  VENT_DEBUG_FUNC_END();
}
#endif

/*************************************** WDOG Functionality *********************************************/

void WDT_Clear(void)
{
  VENT_DEBUG_FUNC_START();
  noInterrupts();

  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1 << WDRF);

  WDTCSR = (1 << WDCE);
  WDTCSR = 0;

  interrupts();
  VENT_DEBUG_FUNC_END();
}

void WDT_Set(const uint8_t val)
{
  VENT_DEBUG_FUNC_START();
  noInterrupts();

  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1 << WDRF);

  /*WDOG Timer and Reset enable*/
  wdt_enable(val);

  /* Interrupt Enable */
  WDTCSR |= ((1 << WDCE) | (1 << WDIE));

  interrupts();
  VENT_DEBUG_FUNC_END();
}

int WDT_Cookie_Check(void)
{
  VENT_DEBUG_FUNC_START();
  char cookie = 'w';
  int result = 0;

  result = eeprom_ext_rw(EEPROM_WDT_DATA, &cookie, sizeof(char), EEPROM_READ); //Read the WDOG Cookie
  if (result != 0)
  {
    VENT_DEBUG_ERROR("Read of EEPROM failed in Watchdog", result);
    result = -1;
    goto wdog_err;
  }

  if (cookie == 'w')
  {
    VENT_DEBUG_ERROR("Reset due to WDOG - Cookie Detected", result);
    cookie = 'r';
    result = eeprom_ext_rw(EEPROM_WDT_DATA, &cookie, sizeof(char), EEPROM_WRITE); //write "r" respresenting watchdog cookie is cleared
    if (result != 0)
    {
      VENT_DEBUG_ERROR("Write of EEPROM failed in Watchdog", result);
      result = -1;
      goto wdog_err;
    }

    result = eeprom_ext_rw(EEPROM_WDT_DATA + 1, &cookie, sizeof(char), EEPROM_READ); //read the state of the machine when the machine was reset by watchdog timer
    if (result == 0)
    {
      if (cookie == 'r') //machine running when the watchdog triggered
      {
        machineOn == true;
      }
      else if (cookie == 's') //machine was in stop state when the watchdog triggered
      {
        machineOn = false;
      }
      else
      {
        VENT_DEBUG_ERROR("Invalid cookie present for the state in the EEPROM WDT: ", -1);
      }
    }
    else
    {
      VENT_DEBUG_ERROR("error reading the state from the EEPROM ", -1);
    }
    /* Update the State to Run the Machine */
    //TODO
  }
  else
  {
    VENT_DEBUG_ERROR("Regular Boot without WDT reset", result);
  }

wdog_err:
  return result;
}

//interrupt service routine for the watchdog timer
ISR(WDT_vect)
{
  VENT_DEBUG_FUNC_START();
  char data = 'w';
  int result = 0;
  result = eeprom_ext_rw(EEPROM_WDT_DATA, &data, sizeof(char), EEPROM_WRITE); //write "w" respresenting watchdog reset to EEPROM at the address EEPROM_WDT_DATA

  if (result == 0)
  {
    VENT_DEBUG_INFO("Write to EEPROM successful by the watchdog", result);
  }
  else
  {
    VENT_DEBUG_ERROR("Write to EEPROM failed in Watchdog", result);
  }
  //writing the state of the machine in the EEPROM
  if (machineOn == true)
  {
    data = 'r'; // "r" respresents run state when the watchdog got triggered
  }
  else
  {
    data = 's'; //"s" respresents stop state when the watchdog was triggered
  }
  result = eeprom_ext_rw(EEPROM_WDT_DATA + 1, &data, sizeof(char), EEPROM_WRITE); //write the state of the machine so that the machine can resume from the original state

  if (result == 0)
  {
    VENT_DEBUG_INFO("Writing state to EEPROM successful by the watchdog", result);
  }
  else
  {
    VENT_DEBUG_ERROR("Writing state to EEPROM failed in Watchdog", result);
  }

  VENT_DEBUG_FUNC_END();
}
