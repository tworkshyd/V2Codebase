#include "ctrl_display.h"

String saveFlag = "Save  ";
String cancelFlag = "Cancel";
int count = 0;
static int settingScreenIndex = 0;
// macros here
#define ROT_ENC_FOR_IER (_currItem == inex_rati.index)
#define ROT_ENC_FOR_PEEP (_currItem == peep_pres.index)
#define EDIT_MENU_TIMEOUT 7000
int oldValue;
int oldValue1;
int oldValue2;
int oldValue3;
int oldValue4;
extern bool machineOn;

extern bool refreshfullscreen_inhale;
extern bool refreshfullscreen_exhale;
extern unsigned long exhale_refresh_timeout;

byte filledDownArrowCustomChar[] = {
    B00100,
    B00100,
    B00100,
    B00100,
    B11111,
    B01110,
    B00100,
    B00000};

byte saveCustomChar2[] = {
    B00011,
    B10001,
    B01001,
    B00101,
    B00101,
    B01001,
    B10001,
    B00011};

// double arrow rotate
byte rotate2customChar[] = {
    B11110,
    B10010,
    B10111,
    B00010,
    B01000,
    B11101,
    B01001,
    B01111};

byte pressEditCustomChar[] = {
    B10001,
    B00000,
    B01110,
    B01110,
    B01110,
    B01110,
    B00000,
    B10001};

byte upTriaChar[] = {
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000};
byte dwnTriaChar[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100};
byte emDnChar[] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B10001,
    B01010,
    B00100};
byte emUpChar[] = {
    B00100,
    B01010,
    B10001,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000};

byte errorCustomChar[] = {
    B00000,
    B00000,
    B00000,
    B11101,
    B10001,
    B11001,
    B10000,
    B11101};
typedef enum
{
  DP_UP_TR = 0,
  DP_DW_TR,
  //  DP_EM_DN_TR,
  // DP_EM_UP_TR,
  DW_POT_AR,
  ROTATE_CHAR,
  EDIT_CHAR,
  SAVE_CHAR,
  ERR_CHAR,
  DP_FI

} DP_CH_T;

static char str_temp[6];
static char buffer[6];
static char buffer1[6];
static char buffer2[6];
static char row[30] = "";

static bool blink = true;
static unsigned long last_o2update = 0;

void drawSplashScreen(sensorManager &sM, displayManager &dM)
{

  int wait = 500;
  RT_Events_T eRTState = RT_NONE;

  VENT_DEBUG_FUNC_START();

  // encoderScanUnblocked();

  lcd.setCursor(0, 0);
  lcd.write(splashScreenTopBottomBuffer);
  lcd.setCursor(0, 1);
  lcd.write(splashScreenMiddleBuffer);
  lcd.setCursor(0, 2);
  lcd.write(splashScreenMiddleBuffer);
  lcd.setCursor(0, 3);
  lcd.write(splashScreenTopBottomBuffer);
  lcd.setCursor(5, 1);
  lcd.write("  Tworks  ");
  lcd.setCursor(5, 2);
  lcd.write("Ventilator");

  delay(1400);

  lcd.setCursor(18, 3);
  lcd.write("  ");
  lcd.setCursor(19, 2);
  lcd.write(" ");

  lcd.setCursor(19, 3);
  lcd.write((byte)0x4);

  while (wait > 0)
  {
    if (digitalRead(DISP_ENC_SW) == 0)
    {
      drawSplashScreenMenu = true;
      wait = 0;
    }
    delay(10);
    wait--;
  }

  VENT_DEBUG_FUNC_END();
  return;
}
void drawServiceLevelScreen(int screenIndex, RT_Events_T eRTState)
{
}
void drawDiagnosticScreen(RT_Events_T eRTState)
{

  if (millivolt_flag)
  {
    mcu0_enable_sensors_voltage(false);
    millivolt_flag = false;
  }

  if (pressure_flag)
  {
    mcu0_enable_sensors_pressure(false);
    pressure_flag = false;
  }

  lcd.setCursor(19, 0);
  lcd.write((byte)(0x3));

  int actualPotValue2 = analogRead(PMAX_PIN);
  float convVal2 = map(actualPotValue2, 0, POT_HIGH, 100, 0);
  actualPotValue2 = (constrain((int)convVal2, 0, 100) % 5);
  //int actualPotValue2 = ((int)convVal2) % 4;

  static char buffer[4];
  sprintf(buffer, "%d/%d", 2, 4);
  lcd.setCursor(0, 0);
  lcd.write(buffer);
  lcd.setCursor(4, 0);
  lcd.write(" Diagnostics   ");

  lcd.setCursor(13, 3);
  lcd.write((byte)(0x2));

  lcd.setCursor(19, 3);
  lcd.write((byte)(0x4));

  lcd.setCursor(8, 2);
  lcd.write("<");
  lcd.print(diagnosticFuncName[actualPotValue2]);
  lcd.write(">");

  delay(50);
  if (RT_BT_PRESS == eRTState)
  {
    (*diagnosticFunc_arr[actualPotValue2])();
    _refreshSplashEditScreen = false;
    lcd.clear();
  }
  return;
}
void drawOxygenCalibScreen(RT_Events_T eRTState, sensorManager sM)
{
	
  static char O2CalibBuffer[4] = {0,0,0,0};

  if (millivolt_flag)
  {
    mcu0_enable_sensors_voltage(false);
    millivolt_flag = false;
  }

  if (pressure_flag)
  {
    mcu0_enable_sensors_pressure(false);
    pressure_flag = false;
  }

  int actualPotValue1 = analogRead(RR_PIN);
  float convVal1 = map(actualPotValue1, 0, POT_HIGH, 100, 0);
  actualPotValue1 = (constrain((int)convVal1, 1, 100) % 3);

  int actualPotValue2 = analogRead(PMAX_PIN);
  float convVal2 = map(actualPotValue2, 0, POT_HIGH, 100, 0);
  actualPotValue2 = (constrain((int)convVal2, 1, 100) % 4);
  lcd.setCursor(19, 0);
  lcd.write((byte)(0x3));

  O2CalibBuffer[0]= 0;
  O2CalibBuffer[1]= 0;
  O2CalibBuffer[2]= 0;
  O2CalibBuffer[3]= 0;
  
  sprintf(O2CalibBuffer, "%d/%d", 1, 4);
  lcd.setCursor(0, 0);
  lcd.write(O2CalibBuffer);
  lcd.setCursor(3, 0);
  lcd.print(" O2 Calibration");

  lcd.setCursor(1, 2);
  lcd.write("<");
  lcd.print(subMenu1[actualPotValue1]);
  lcd.write(">");

  lcd.setCursor(8, 2);
  lcd.write("<");
  if (actualPotValue2 == 0)
  {
    lcd.print(" ");
    lcd.print(retrieveCalibParam(EEPROM_O2_CALIB_ADDR + (actualPotValue1 * 2)));
    lcd.print("mv  ");
  }
  else
  {
    lcd.print(oxySettings[actualPotValue2]);
  }
  lcd.write(">");

  lcd.setCursor(5, 3);
  lcd.write((byte)(0x2));
  lcd.setCursor(13, 3);
  lcd.write((byte)(0x2));
  lcd.setCursor(19, 3);
  lcd.write((byte)(0x4));

  if (eRTState == RT_BT_PRESS)
  {
    if (actualPotValue2 == 2)
    {
      float avgO2Value = 0 ;
      for (int i = 0; i < 10; i++)
      {
        sM.capture_and_read_data(SENSOR_O2);
        avgO2Value += sM.read_sensor_rawvoltage(SENSOR_O2);
      }
      storeCalibParam(EEPROM_O2_CALIB_ADDR + (actualPotValue1 * 2), (int)(avgO2Value / 10));
    }

    (*oxygenCalibFunc_arr[actualPotValue2])(actualPotValue1);
    _refreshSplashEditScreen = false;
    lcd.clear();
  }
  return;
}
void CurrentO2Value(int i)
{
}

void ResetO2(int i)
{
  if (i == 0)
  {
    storeCalibParam(EEPROM_O2_CALIB_ADDR + (i * 2), O2_0_FACTORY_VALUE);
  }
  else if (i == 1)
  {
    storeCalibParam(EEPROM_O2_CALIB_ADDR + i * 2, O2_22_FACTORY_VALUE);
  }
  else if (i == 2)
  {
    storeCalibParam(EEPROM_O2_CALIB_ADDR + i * 2, O2_100_FACTORY_VALUE);
  }
  else
  {
  }
  lcd.setCursor(3, 3);
  lcd.print("Reset Done");
  delay(1000);
}
void CalibrateO2(int i)
{
  lcd.setCursor(3, 3);
  lcd.print("Calibrate Done");
  delay(1000);
}

void CalibExit(int i)
{
  continueLoop = false;
  drawSplashScreenMenu = false;
}
void Adc()
{
  lcd.setCursor(3, 3);
  lcd.print("adc");
  delay(1000);
  return;
}
void Com()
{
  lcd.setCursor(3, 3);
  lcd.print("Com");
  delay(1000);
  return;
}
void Sensor()
{
  lcd.setCursor(3, 3);
  lcd.print("Sensors");
  delay(1000);
  return;
}
void Valves()
{
  lcd.setCursor(3, 3);
  lcd.print("Valves");
  delay(1000);
  // continueLoop = false;
  return;
}
void ExitDaignostic()
{
  continueLoop = false;
  drawSplashScreenMenu = false;
  return;
}

void displayManager::setDisplayParam(eDisplayPrm param, float value)
{
  VENT_DEBUG_FUNC_START();

  VENT_DEBUG_INFO("Param", value);

  switch (param)
  {
  case DISPLAY_TVE:
    m_display_tve = value;
    break;
  case DISPLAY_TVI:
    m_display_tvi = value;
    break;
  case DISPLAY_PEEP:
    m_display_peep = value;
    break;
  case DISPLAY_PLAT:
    m_display_plat = value;
    break;
  case DISPLAY_PIP:
    m_display_pip = value;
    break;
  }
  VENT_DEBUG_FUNC_END();
}

float displayManager::getDisplayParam(eDisplayPrm param)
{
  switch (param)
  {
  case DISPLAY_TVI:
    return m_display_tvi;
    break;
  case DISPLAY_TVE:
    return m_display_tve;
    break;
  case DISPLAY_PEEP:
    return m_display_peep;
    break;
  case DISPLAY_PLAT:
    return m_display_plat;
    break;
  case DISPLAY_PIP:
    return m_display_pip;
    break;
  default:
    return -1;
  }
}
void exitFactory()
{
  delay(1000);
  showAboutScreenSubMenu = true;
}
void displayManager::drawSensorValueMenu(RT_Events_T eRTState)
{
  if (millivolt_flag)
  {
    mcu0_enable_sensors_voltage(false);
    millivolt_flag = false;
  }
  if (pressure_flag == false)
  {
    mcu0_enable_sensors_pressure(true);
    pressure_flag = true;
  }
  _lastSubEditMenuTime = millis();
  bool bCalibrate = false;
  m_sM->enable_sensor(DP_A0 | O2 | DP_A1);

  switch (eRTState)
  {
  case RT_INC:
    //  _currentSaveFlag = false;
    break;
  case RT_DEC:
    // _currentSaveFlag = true;
    break;
  case RT_BT_PRESS:
    bCalibrate = true;
    // _currentSaveFlag = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 500) ||
      (eRTState != RT_NONE))
  {
    drawRuntimeTopBottomLines(3, 4, ROTATE_CHAR, EDIT_CHAR);
    lcd.setCursor(8, 1);
    lcd.print("  ");
    lcd.setCursor(8, 2);
    lcd.print("  ");
    lcd.setCursor(5, 0);
    lcd.print("Sensor cmH2o");
    lcd.setCursor(0, 1);
    lcd.print("GP1:");
    lcd.print(mcu0_read_pressure(SENSOR_PRESSURE_A0));
    Serial.print("GP1: ");
    Serial.print(mcu0_read_pressure(SENSOR_PRESSURE_A0));

    lcd.setCursor(10, 1);
    lcd.print("DP1:");
    lcd.print(m_sM->read_sensor_pressurevalues(SENSOR_DP_A0));
    lcd.setCursor(0, 2);
    lcd.print("GP2:");
    lcd.print(mcu0_read_pressure(SENSOR_PRESSURE_A1));
    Serial.print("\tGP2: ");
    Serial.println(mcu0_read_pressure(SENSOR_PRESSURE_A1));
    lcd.setCursor(10, 2);
    lcd.print("DP2:");
    lcd.print(m_sM->read_sensor_pressurevalues(SENSOR_DP_A1));
    lcd.setCursor(0, 3);
  }
  if (bCalibrate)
  {
    // if (_currentSaveFlag == 0)
    // {
    //   lcd.print(" .....cancelled");
    //   mcu0_enable_sensors_pressure(false);
    //   count = 0;
    //   m_sM->enable_sensor(0);
    //   clearDisplay();
    //   _refreshEditScreenDisplay = false;
    //   _refreshRunTimeDisplay = true;
    //   bCalibrate = false;
    //   _dpState = STATUS_MENU;
    // }
    // else
    //   {
    lcd.setCursor(0, 3);
    lcd.print(" calibrating....");
    serial2_rxdata= "";
    //rxdata_buff="";
    m_sM->enable_sensor(0);
    m_sM->start_calibration();
    m_sM->enable_sensor(0);
    mcu0_enable_sensors_pressure(false);
    mcu0_calibrate_sensor();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    //clearDisplay();
    _refreshEditScreenDisplay = false;
    _refreshRunTimeDisplay = true;
    bCalibrate = false;
    //m_sM->enable_sensor(0);
       delay(5000);
    lcd.setCursor(0, 3);
    lcd.print("   rebooting...."); 
    //drawSplashScreenMenu = false;
    //statusScreenIndex = 0;
    //setup();
    return;
     //wdt_reset();
    // showAboutScreenSubMenu = true;
    //  }
  }
  //}
}

void displayManager::drawSensorvoltageMenu(RT_Events_T eRTState)
{

  _lastSubEditMenuTime = millis();
  m_sM->enable_sensor(DP_A0 | O2 | DP_A1);
  if (millivolt_flag == false)
  {
    mcu0_enable_sensors_voltage(true);
    millivolt_flag = true;
  }
  if (pressure_flag)
  {
    mcu0_enable_sensors_pressure(false);
    pressure_flag = false;
  }

  switch (eRTState)
  {
  case RT_INC:
    //  m_sM->enable_sensor(0);
    // mcu0_enable_sensors_voltage(false);
    break;
  case RT_DEC:
    // m_sM->enable_sensor(0);
    // mcu0_enable_sensors_voltage(false);
    break;
  case RT_BT_PRESS:
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 500) ||
      (eRTState != RT_NONE))
  {
    lcd.setCursor(19, 0);
    lcd.write((byte)(0x3));

    static char buffer[4];
    sprintf(buffer, "%d/%d", 4, 4);
    lcd.setCursor(0, 0);
    lcd.write(buffer);

    // drawRuntimeTopBottomLines(4, 4, ROTATE_CHAR, EDIT_CHAR);
    lcd.setCursor(7, 1);
    lcd.print("   ");
    lcd.setCursor(7, 2);
    lcd.print("   ");
    lcd.setCursor(6, 0);
    lcd.print("Sensor mV");
    lcd.setCursor(0, 1);
    lcd.print("GP1:");
    lcd.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A0));
    Serial.print("mvGP1: ");
    Serial.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A0));

    lcd.setCursor(10, 1);
    lcd.print("DP1:");
    lcd.print(m_sM->read_sensor_rawvoltage(SENSOR_DP_A0));

    lcd.setCursor(0, 2);
    lcd.print("GP2:");
    lcd.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A1));
    Serial.print("\tmvGP2: ");
    Serial.println(mcu0_read_rawvoltage(SENSOR_PRESSURE_A1));

    lcd.setCursor(10, 2);
    lcd.print("DP2:");
    lcd.print(m_sM->read_sensor_rawvoltage(SENSOR_DP_A1));
    int o2value = m_sM->read_sensor_rawvoltage(SENSOR_O2);
    lcd.setCursor(7, 3);
    lcd.print("O2: ");
    lcd.print(o2value);
  }
}

void displayManager::drawUpdateO2_InputMenu(RT_Events_T eRTState)
{
  static bool tempO2LineSelect;
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    tempO2LineSelect = 1;
    break;
  case RT_DEC:
    tempO2LineSelect = 0;
    break;
  case RT_BT_PRESS:
    bSave = true;
    _bBack2EditMenu = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if (((millis() - _lastDisplayTime) > 300) ||
      (eRTState != RT_NONE))
  {
    _lastDisplayTime = millis();
    VENT_DEBUG_INFO("O2 Changer", 0);

    lcd.setCursor(0, 1);
    if (tempO2LineSelect == 0)
    {
      lcd.print("< O2 Cylinder >  ");
      lcd.setCursor(0, 2);
      lcd.print("  HospitalLine  ");
    }
    else
    {
      lcd.print("  O2 Cylinder    ");
      lcd.setCursor(0, 2);
      lcd.print("< HospitalLine > ");
    }
  }
  if (true == bSave)
  {
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" saved          ");

    _o2LineSelect = tempO2LineSelect;
    if (_o2LineSelect == CYLINDER)
    {
      //Ctrl_send_packet(OXY_SOLE_HOS_O2_OFF);
      //delay(10);
      Ctrl_send_packet(OXY_SOLE_CYL_ONN);
    }
    else //HOSP_LINE
    {
      //Ctrl_send_packet(OXY_SOLE_CYL_OFF);
      //delay(10);
      Ctrl_send_packet(OXY_SOLE_HOS_O2_ONN);
    }
    params[_currItem].value_curr_mem = _o2LineSelect;
    storeParam(params[_currItem]);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void displayManager::drawUpdatePEEPorIERMenu(RT_Events_T eRTState)
{
  int oldIER = params[inex_rati.index].value_curr_mem;
  static int incr = 0;
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    incr++;
    break;
  case RT_DEC:
    incr--;
    break;
  case RT_BT_PRESS:
    bSave = true;
    _bBack2EditMenu = true;
    if (ROT_ENC_FOR_IER)
      params[inex_rati.index].value_curr_mem = _newIER;
    if (ROT_ENC_FOR_PEEP)
      params[peep_pres.index].value_curr_mem = _newPeep;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 300) ||
      (incr != 0))
  {
    _lastDisplayTime = millis();
    if (ROT_ENC_FOR_IER)
    {
      _newIER = rectifyBoundaries(_newIER + incr, inex_rati.min_val, inex_rati.max_val);
      lcd.setCursor(VALUE1_DISPLAY_POS, 1);
      lcd.setCursor(3, 1);
      lcd.print("New IER  1:");
      lcd.print(_newIER);
      lcd.setCursor(3, 2);
      lcd.print("Old IER  1:");
      lcd.print(oldIER);
      params[inex_rati.index].value_new_pot = _newIER;
    }
    else if (ROT_ENC_FOR_PEEP)
    {
      _newPeep = rectifyBoundaries(_newPeep + incr * peep_pres.incr, peep_pres.min_val, peep_pres.max_val);
      params[peep_pres.index].value_new_pot = _newPeep;
      lcd.setCursor(VALUE1_DISPLAY_POS, 1);
      printPadded(_newPeep);
      lcd.setCursor(0, 3);
      lcd.print("set using PEEP valve");
    }
    incr = 0;
  }

  if (true == bSave)
  {
    params[_currItem].value_curr_mem = getCalibratedParamFromPot(params[_currItem]);
    storeParam(params[_currItem]);
    Serial.print(_currItem);
    Ctrl_send_packet(params[_currItem].parm_name, params[_currItem].value_curr_mem);
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" saved          ");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }
}

void displayManager::drawUpdateFiO2Menu(RT_Events_T eRTState)
{
  bool bSave = false;
  if (0 == _o2_en_flag)
  {
    unsigned int _o2_en_flag = sM.get_enable_sensors();
    if ((_o2_en_flag & O2) == 0)
    {
      _o2_en_flag = _o2_en_flag | O2;
      sM.enable_sensor(_o2_en_flag);
    }
  }
  switch (eRTState)
  {
  case RT_INC:
    _currentSaveFlag = false;
    break;
  case RT_DEC:
    _currentSaveFlag = true;
    break;
  case RT_BT_PRESS:
    _o2_en_flag = 0;
    _bBack2EditMenu = true;
    bSave = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 300) ||
      (eRTState != RT_NONE))
  {
    _lastDisplayTime = millis();
    lcd.setCursor(0, 0);
    lcd.print(" Live O2  : ");
    float data = 0.0;
    lcd.print((int)m_o2_sensor_data);
    lcd.print("%");
    Serial.print("reading from Pot :");
    params[_currItem].value_new_pot = analogRead(params[_currItem].readPortNum);
    Serial.println(params[_currItem].value_new_pot);
    lcd.setCursor(0, 1);
    lcd.print(" New      : ");
    lcd.print(" ");
    lcd.setCursor(11, 1);
    int actualValue = getCalibValue(params[_currItem].value_new_pot, _currItem);
    printPadded(actualValue);
    //lcd.setCursor(15, 1);
    lcd.print(params[_currItem].units);
    lcd.setCursor(0, 2);
    lcd.print(" Old      : ");
    lcd.setCursor(11, 2);
    printPadded(params[_currItem].value_curr_mem);
    //lcd.setCursor(15, 2);
    lcd.print(params[_currItem].units);
    lcd.setCursor(0, 3);
    if (true == _currentSaveFlag)
    {
      lcd.print(SAVE_FLAG_CHOSEN);
      lcd.print("   ");
      lcd.print(CANC_FLAG);
    }
    else
    {
      lcd.print(SAVE_FLAG);
      lcd.print("   ");
      lcd.print(CANC_FLAG_CHOSEN);
    }

    int diffValue = abs(oldValue - params[_currItem].value_new_pot);
    if (diffValue > 5)
    {
      /*Special case update for pot inputs*/
      _lastSubEditMenuTime = millis();
    }
    oldValue = params[_currItem].value_new_pot;
  }

  if (bSave)
  {
    if (_currentSaveFlag == 0)
    {
      lcd.print(" Edit cancelled.");
    }
    else
    {
      params[_currItem].value_curr_mem = getCalibratedParamFromPot(params[_currItem]);
      storeParam(params[_currItem]);
      //Ctrl_send_packet(params[_currItem].parm_name, params[_currItem].value_curr_mem);
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print(" saved.....");
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  if (true == _bBack2EditMenu)
  {
    lcd.setCursor(0, 3);
    lcd.print("   going back....");
    delay(100);
  }
}

void displayManager::drawUpdateOpModeMenu(RT_Events_T eRTState)
{
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    _oPModeSelect = SIMV;
    break;
  case RT_DEC:
    _oPModeSelect = CMV;
    break;
  case RT_BT_PRESS:
    bSave = true;
    _bBack2EditMenu = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if (((millis() - _lastDisplayTime) > 300) ||
      (eRTState != RT_NONE))
  {
    _lastDisplayTime = millis();
    lcd.setCursor(0, 1);
    if (_oPModeSelect == CMV)
    {
      lcd.print("    < CMV >     ");
      lcd.setCursor(0, 2);
      lcd.print("      SIMV       ");
    }
    else
    {
      lcd.print("      CMV        ");
      lcd.setCursor(0, 2);
      lcd.print("    < SIMV >     ");
    }
  }
  if (true == bSave)
  {
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" saved          ");

    params[_currItem].value_curr_mem = _oPModeSelect;
    storeParam(params[_currItem]);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void displayManager::drawDefaultAllItemUpdateMenu(RT_Events_T eRTState)
{

  drawRuntimeTopBottomLines(1, 4, ROTATE_CHAR, SAVE_CHAR);
  bool bSave = false;
  //cleanRow(4);
  switch (eRTState)
  {
  case RT_INC:
    _currentSaveFlag = false;
    _dpState = STATUS_MENU;
    break;
  case RT_DEC:
    _currentSaveFlag = true;
    break;
  case RT_BT_PRESS:
    //_bBack2EditMenu = true;
    bSave = true;
    _dpState = STATUS_MENU;
    // _refreshEditScreenDisplay = false;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  _lastEditDisplayTime = millis();

  //_refreshEditScreenDisplay  _lastDisplayTime = millis();
  if ((millis() - _lastDisplayTime > 30) ||
      (eRTState != RT_NONE))
  {
    _lastDisplayTime = millis();
    lcd.setCursor(0, 0);
    lcd.write(" TV   RR  Pmax IER");
    lcd.setCursor(0, 1);
    lcd.write(" ");
    printPadded(params[1].value_curr_mem);
    lcd.write(" ");
    printPadded(params[2].value_curr_mem);
    lcd.write("  ");
    printPadded(params[6].value_curr_mem);
    lcd.write("  ");
    lcd.print("1:");
    lcd.print(params[4].value_curr_mem);

    lcd.setCursor(0, 2);

    //TV
    lcd.write("<");
    params[1].value_new_pot = analogRead(params[1].readPortNum);
    int actualTVValue = getCalibValue(params[1].value_new_pot, 1);
    lcd.print(actualTVValue);
    lcd.write(">");

    //RR
    params[2].value_new_pot = analogRead(params[2].readPortNum);
    int actualRRValue = getCalibValue(params[2].value_new_pot, 2);
    printRRValue(actualRRValue);

    //PMAX

    params[6].value_new_pot = analogRead(params[6].readPortNum);
    int actualPMAXValue = getCalibValue(params[6].value_new_pot, 6);
    // lcd.print(actualPMAXValue);
    printEditValue(actualPMAXValue);

    //IER
    lcd.write("<");
    params[4].value_new_pot = analogRead(params[4].readPortNum);
    int actualIERValue = map(((params[4].value_new_pot / 100) % 3) + 1, 1, 3, 3, 1);
    // params[4].value_curr_mem=
    lcd.print("1:");
    lcd.print(actualIERValue);
    lcd.write(">");
    lcd.print(" ");

    int diffValue1 = abs(oldValue1 - params[1].value_new_pot);
    int diffValue2 = abs(oldValue2 - params[2].value_new_pot);
    int diffValue3 = abs(oldValue3 - params[6].value_new_pot);
    int diffValue4 = abs(oldValue4 - params[4].value_new_pot);

    if (diffValue1 > 5 || diffValue2 > 5 || diffValue3 > 5 || diffValue4 > 5)
    {
      /*Special case update for pot inputs*/
      _lastEditMenuTime = millis();
    }
    oldValue1 = params[1].value_new_pot;
    oldValue2 = params[2].value_new_pot;
    oldValue3 = params[6].value_new_pot;
    oldValue4 = params[4].value_new_pot;
    lcd.setCursor(1, 3);
    lcd.write((byte)DW_POT_AR);
    lcd.setCursor(6, 3);
    lcd.write((byte)(DW_POT_AR));
    lcd.setCursor(11, 3);
    lcd.write((byte)(DW_POT_AR));
    lcd.setCursor(16, 3);
    lcd.write((byte)(DW_POT_AR));

    if (bSave)
    {
      params[1].value_curr_mem = getCalibratedParamFromPot(params[1]);
      storeParam(params[1]);
      Ctrl_send_packet(params[1].parm_name, params[1].value_curr_mem);
      delay(100);
      params[2].value_curr_mem = getCalibratedParamFromPot(params[2]);
      storeParam(params[2]);
      Ctrl_send_packet(params[2].parm_name, params[2].value_curr_mem);
      delay(100);
      params[6].value_curr_mem = getCalibratedParamFromPot(params[6]);
      storeParam(params[6]);
      Ctrl_send_packet(params[6].parm_name, params[6].value_curr_mem);
      delay(100);
      params[4].value_curr_mem = actualIERValue; //getCalibratedParamFromPot(params[4]);
      storeParam(params[4]);
      Ctrl_send_packet(params[4].parm_name, params[4].value_curr_mem);
      
      lcd.setCursor(0, 3);
      lcd.print(" saved.....");
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      clearDisplay();
      _refreshEditScreenDisplay = false;
      _refreshRunTimeDisplay = true;
      bSave = false;
      _dpState = STATUS_MENU;
    }
  }
}

void displayManager::drawDefaultItemUpdateMenu(RT_Events_T eRTState)
{
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    _currentSaveFlag = false;
    break;
  case RT_DEC:
    _currentSaveFlag = true;
    break;
  case RT_BT_PRESS:
    _bBack2EditMenu = true;
    bSave = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 500) ||
      (eRTState != RT_NONE))
  {
    _lastDisplayTime = millis();
    lcd.setCursor(8, 0);
    lcd.print(params[_currItem].parm_name);
    Serial.print("reading from Pot :");
    params[_currItem].value_new_pot = analogRead(params[_currItem].readPortNum);
    Serial.println(params[_currItem].value_new_pot);
    lcd.setCursor(0, 1);
    lcd.print("New: ");
    lcd.print("   ");
    lcd.setCursor(8, 1);
    int actualValue = getCalibValue(params[_currItem].value_new_pot, _currItem);
    printPadded(actualValue);
    lcd.setCursor(15, 1);
    lcd.print(params[_currItem].units);
    lcd.setCursor(0, 2);
    lcd.print("Old: ");
    lcd.setCursor(8, 2);
    printPadded(params[_currItem].value_curr_mem);
    lcd.setCursor(15, 2);
    lcd.print(params[_currItem].units);
    lcd.setCursor(0, 3);
    if (true == _currentSaveFlag)
    {
      lcd.print(SAVE_FLAG_CHOSEN);
      lcd.print("    ");
      lcd.print(CANC_FLAG);
    }
    else
    {
      lcd.print(SAVE_FLAG);
      lcd.print("    ");
      lcd.print(CANC_FLAG_CHOSEN);
    }

    int diffValue = abs(oldValue - params[_currItem].value_new_pot);
    if (diffValue > 5)
    {
      /*Special case update for pot inputs*/
      _lastSubEditMenuTime = millis();
    }
    oldValue = params[_currItem].value_new_pot;
  }
  if (bSave)
  {
    if (_currentSaveFlag == 0)
    {
      lcd.print(" Edit cancelled.");
    }
    else
    {
      params[_currItem].value_curr_mem = getCalibratedParamFromPot(params[_currItem]);
      storeParam(params[_currItem]);
      Ctrl_send_packet(params[_currItem].parm_name, params[_currItem].value_curr_mem);
      lcd.setCursor(0, 3);
      lcd.print("                    ");
      lcd.setCursor(0, 3);
      lcd.print(" saved.....");
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void displayManager::drawEditMenu()
{
  RT_Events_T eRTState = RT_NONE;

  eRTState = encoderScanUnblocked();
  switch (eRTState)
  {
  case RT_INC:
    clearDisplay();
    settingScreenIndex = settingScreenIndex + 1;
    _lastEditMenuTime = millis();
    break;
  case RT_DEC:
    clearDisplay();
    settingScreenIndex = settingScreenIndex - 1;
    _lastEditMenuTime = millis();
    break;
  case RT_BT_PRESS:
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  switch (abs(settingScreenIndex % 4))
  {
  case 0:
    drawDefaultAllItemUpdateMenu(eRTState);
    break;
  case 1:
    drawSettingScreen1(eRTState);
    break;
  case 2:
    drawSettingScreen2(eRTState);
    break;
  case 3:
    drawSettingScreen3(eRTState);
    break;
  // case 4:
  //   drawSensorValueMenu(eRTState);
  //   break;
  // case 5:
  //   drawSensorvoltageMenu(eRTState);
  //   break;
  default:
    break;
  }
}
void displayManager::drawSettingScreen1(RT_Events_T eRTState)
{
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    break;
  case RT_DEC:
    break;
  case RT_BT_PRESS:
    bSave = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  drawRuntimeTopBottomLines(2, 4, ROTATE_CHAR, SAVE_CHAR);
  lcd.setCursor(5, 0);
  lcd.write("LCD Contrast");
  lcd.setCursor(1, 2);
  lcd.write("Value");
  lcd.write(" ");
  printPadded(params[13].value_curr_mem);
  lcd.write("%");
  lcd.write("  ");
  lcd.write("<");
  params[13].value_new_pot = analogRead(params[13].readPortNum);
  int actualFio2Value = getCalibValue(params[13].value_new_pot, 13);
  //printEditValue(actualFio2Value);
  lcd.print(actualFio2Value);
  lcd.print("%");
  lcd.write(">");
  lcd.print(" ");
  lcd.setCursor(14, 3);
  lcd.write((byte)DW_POT_AR);
  lcd.setCursor(19, 3);
  lcd.write((byte)SAVE_CHAR);

    int diffValue1 = abs(oldValue1 - params[13].value_new_pot);
       if (diffValue1 > 5 )
    {
      /*Special case update for pot inputs*/
      _lastEditMenuTime = millis();
    }
    oldValue1 = params[13].value_new_pot;

  //_lastEditMenuTime = millis();
  if (bSave)
  {
    params[13].value_curr_mem = getCalibratedParamFromPot(params[13]);
    storeParam(params[13]);
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" saved.....");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    clearDisplay();
    _refreshEditScreenDisplay = false;
    _refreshRunTimeDisplay = true;
    bSave = false;
    _dpState = STATUS_MENU;
  }

  return;
}
void displayManager::drawSettingScreen2(RT_Events_T eRTState)
{
  bool bSave = false;
  switch (eRTState)
  {
  case RT_INC:
    break;
  case RT_DEC:
    break;
  case RT_BT_PRESS:
    bSave = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  drawRuntimeTopBottomLines(3, 4, ROTATE_CHAR, SAVE_CHAR);
  lcd.setCursor(7, 0);
  lcd.write("ALARMS");
  lcd.setCursor(1, 2);
  lcd.write("FiO2");
  lcd.write("  ");
  printPadded(params[3].value_curr_mem);
  lcd.print("%");
  lcd.write("  ");
  lcd.write("<");
  params[3].value_new_pot = analogRead(params[3].readPortNum);
  int actualFio2Value = getCalibValue(params[3].value_new_pot, 3);
  lcd.print(actualFio2Value);
  lcd.print("%");
  lcd.write(">");
  lcd.write(" ");
  lcd.setCursor(14, 3);
  lcd.write((byte)DW_POT_AR);
  lcd.setCursor(19, 3);
  lcd.write((byte)SAVE_CHAR);
    int diffValue1 = abs(oldValue1 - params[3].value_new_pot);
       if (diffValue1 > 5 )
    {
      /*Special case update for pot inputs*/
      _lastEditMenuTime = millis();
    }
    oldValue1 = params[3].value_new_pot;

  if (bSave)
  {
    params[3].value_curr_mem = getCalibratedParamFromPot(params[3]);
    storeParam(params[3]);
    lcd.setCursor(0, 3);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print(" saved.....");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    clearDisplay();
    _refreshEditScreenDisplay = false;
    _refreshRunTimeDisplay = true;
    bSave = false;
    _dpState = STATUS_MENU;
  }
  return;
}
void displayManager::drawSettingScreen3(RT_Events_T eRTState)
{
  showAboutScreenSubMenu = false;
  drawRuntimeTopBottomLines(4, 4, ROTATE_CHAR, EDIT_CHAR);
  lcd.setCursor(7, 0);
  lcd.write("About");
  lcd.setCursor(1, 1);
  lcd.write("Device   : BMV");
  lcd.setCursor(1, 2);
  lcd.write("Serial No: TW0001");
  lcd.setCursor(1, 3);
  lcd.write("Version  : V2.3");

  if (eRTState == RT_BT_PRESS)
  {
    if (machineOn)
    {
      _lastEditMenuTime = millis();
      clearDisplay();
      lcd.setCursor(2, 1);
      lcd.print("Not Accessible");
      lcd.setCursor(2, 2);
      lcd.print("Stop Device ");
      lcd.setCursor(2, 3);
      lcd.print("for Access");
      delay(2000);
      clearDisplay();
    }
    else
    {
      aboutScreenIndex = 0;
      lcd.clear();
       while(!showAboutScreenSubMenu)
      {
       eRTState = encoderScanUnblocked();
        switch (eRTState)
        {
        case RT_INC:
          clearDisplay();
          aboutScreenIndex = aboutScreenIndex + 1;
          break;
        case RT_DEC:
          clearDisplay();
          aboutScreenIndex = aboutScreenIndex - 1;
          break;
        case RT_NONE:
          break;
        default:
          break;
        }
       // drawServiceMenuScreen1(eRTState);
        switch (abs(aboutScreenIndex % 1))
        {
        case 0:
          drawServiceMenuScreen1(eRTState);
          break;
        default:
          break;
        }
      }
      clearDisplay();
    }
  }
}

void displayManager::drawServiceMenuScreen1(RT_Events_T eRTState)
{
  if (millivolt_flag)
  {
    mcu0_enable_sensors_voltage(false);
    millivolt_flag = false;
  }

  if (pressure_flag)
  {
    mcu0_enable_sensors_pressure(false);
    pressure_flag = false;
  }

  if (showAboutScreenSubMenu == false)
  {
    while (digitalRead(DISP_ENC_SW) == 0)
    {

      delay(10);
    }
  }
  drawRuntimeTopBottomLines(1, 3, ROTATE_CHAR, SAVE_CHAR);

  params[6].value_new_pot = analogRead(params[6].readPortNum);
  int actualIERValue = getCalibValue((params[6].value_new_pot), 6);
  int index = (constrain(actualIERValue, 29, 99) % 4); //actualIERValue % 4;

  /// Factory Reset
  lcd.setCursor(4, 0);
  lcd.write("Factory Setting");

  lcd.setCursor(13, 3);
  lcd.write((byte)DW_POT_AR);

  lcd.setCursor(1, 2);
  lcd.write("<");
  lcd.write(factorySettings[index]);
  lcd.write(">");
  if (digitalRead(DISP_ENC_SW) == 0)
  {
    (*fun_ptr_arr[index])();
  }

  return;
}
void SetDefaultAllParam()
{
  params[1].value_curr_mem = TV_DEFAULT_VALUE;
  storeParam(params[1]);
  Ctrl_send_packet(params[1].parm_name, params[1].value_curr_mem);
  delay(100);
  params[2].value_curr_mem = RR_DEFAULT_VALUE;
  storeParam(params[2]);
  Ctrl_send_packet(params[2].parm_name, params[2].value_curr_mem);
  delay(100);
  params[6].value_curr_mem = PMAX_DEFAULT_VALUE;
  storeParam(params[6]);
  Ctrl_send_packet(params[6].parm_name, params[6].value_curr_mem);
  delay(100);
  params[4].value_curr_mem = IER_DEFAULT_VALUE;
  storeParam(params[4]);
  Ctrl_send_packet(params[4].parm_name, params[4].value_curr_mem);
  lcd.setCursor(2, 3);
  lcd.write("Reset Done");
  delay(2000);
  showAboutScreenSubMenu = true;
  //cleanRow(3);
  lcd.clear();
}
void SetBasicParam()
{
  params[1].value_curr_mem = TV_DEFAULT_VALUE;
  storeParam(params[1]);
  Ctrl_send_packet(params[1].parm_name, params[1].value_curr_mem);
  delay(100);
  params[2].value_curr_mem = RR_DEFAULT_VALUE;
  storeParam(params[2]);
  Ctrl_send_packet(params[2].parm_name, params[2].value_curr_mem);
  delay(100);
  params[6].value_curr_mem = PMAX_DEFAULT_VALUE;
  storeParam(params[6]);
  Ctrl_send_packet(params[6].parm_name, params[6].value_curr_mem);
  delay(100);
  params[4].value_curr_mem = IER_DEFAULT_VALUE;
  storeParam(params[4]);
  Ctrl_send_packet(params[4].parm_name, params[4].value_curr_mem);

  lcd.setCursor(2, 3);
  lcd.write("Reset Done");
  delay(2000);
  showAboutScreenSubMenu = true;
  //cleanRow(3);
  lcd.clear();
}
void SetDefaultCalibration()
{
  lcd.setCursor(2, 3);
  lcd.write("Reset Done");
  delay(2000);
  showAboutScreenSubMenu = true;
  //cleanRow(3);
  lcd.clear();
}

/*utility to show the states in string*/
String displayManager::dpStatusString(STATE State)
{
  switch (State)
  {
  case STATUS_MENU:
    return "STATUS_MENU";
  case STATUS_MENU_TO_EDIT_MENU:
    return "STATUS_MENU_TO_EDIT_MENU";
  case EDIT_MENU:
    return "EDIT_MENU";
  case EDIT_MENU_TO_SUB_EDIT_MENU:
    return "EDIT_MENU_TO_SUB_EDIT_MENU";
  case SUB_EDIT_MENU:
    return "SUB_EDIT_MENU";
  case SUB_EDIT_MENU_TO_EDIT_MENU:
    return "SUB_EDIT_MENU_TO_EDIT_MENU";
  case EDIT_MENU_TO_STATUS_MENU:
    return "EDIT_MENU_TO_STATUS_MENU";
  default:
    return "InCorrect State";
  }
}
void displayManager::moveUpEdit()
{
  _editSeletIndicator++;
  /*
     check wrap around of select indicator
  */
  if ((_editSeletIndicator >= MAX_EDIT_MENU_ITEMS) ||
      (_editSeletIndicator > LCD_HEIGHT_CHAR - 1))
  {
    _editSeletIndicator = min(LCD_HEIGHT_CHAR - 1, MAX_EDIT_MENU_ITEMS);
    if (_editSeletIndicator == LCD_HEIGHT_CHAR - 1)
    {
      _editScrollIndex++;
    }
    if ((_editScrollIndex + _editSeletIndicator) >= MAX_EDIT_MENU_ITEMS)
    {
      _editScrollIndex = MAX_EDIT_MENU_ITEMS - _editSeletIndicator - 1;
    }
  }
}

void displayManager::moveDownEdit()
{
  /*
     check wrap around of select indicator
  */
  if (_editSeletIndicator == 0)
  {
    if (_editScrollIndex > 0)
    {
      _editScrollIndex--;
    }
  }
  else
  {
    _editSeletIndicator--;
  }
}

/*
   The getCalibValue is for calibrating any integer to a parameter.
   The parameter is input with the help of an index
*/
int displayManager::getCalibValue(int potValue, int paramIndex)
{
  float convVal = map(potValue, 0, POT_HIGH, params[paramIndex].max_val, params[paramIndex].min_val);
  return ((int)(convVal / params[paramIndex].incr) + 1) * params[paramIndex].incr;
}

/*
   The below method is for a specific parameter
*/
int displayManager::getCalibratedParamFromPot(ctrl_parameter_t param)
{
  if (param.readPortNum == DISP_ENC_CLK)
  {
    return param.value_new_pot;
  }
  float convVal = map(param.value_new_pot, 0, POT_HIGH, param.max_val, param.min_val);
  return ((int)(convVal / param.incr) + 1) * param.incr;
}

int displayManager::rectifyBoundaries(int value, int minimum, int maximum)
{
  if (value < minimum)
  {
    return maximum;
  }
  if (value > maximum)
  {
    return minimum;
  }
  return value;
}

void displayManager::editModeEncoderInput(void)
{
  RT_Events_T eRTState = RT_NONE;
  _bEditItemSelected = false;
  _bRefreshEditScreen = false;
  eRTState = encoderScanUnblocked();
  switch (eRTState)
  {
  case RT_INC:
    VENT_DEBUG_INFO("RT_INC", 0);
    moveUpEdit();
    break;
  case RT_DEC:
    VENT_DEBUG_INFO("RT_DEC", 0);
    moveDownEdit();
    break;
  case RT_BT_PRESS:
    VENT_DEBUG_INFO("RT_BT_PRESS", 0);
    _bEditItemSelected = true;
    break;
  case RT_NONE:
    break;
  }
  if (eRTState != RT_NONE)
  {
    _bRefreshEditScreen = true;
  }
}

void displayManager::handleItemUpdate()
{
  RT_Events_T eRTState;
  eRTState = encoderScanUnblocked();
  if (eRTState != RT_NONE)
  {
    _lastSubEditMenuTime = millis();
  }
  switch (_currItem)
  {
  case (E_TV):
    // drawDefaultItemUpdateMenu(eRTState);
    drawDefaultAllItemUpdateMenu(eRTState);
    break;
  case (E_BPM):
    drawDefaultItemUpdateMenu(eRTState);
    break;
  case (E_FiO2):
    drawUpdateFiO2Menu(eRTState);
    break;
  case (E_IER):
    drawUpdatePEEPorIERMenu(eRTState);
    break;
  case (E_PEEP):
    drawUpdatePEEPorIERMenu(eRTState);
    break;
  case (E_PIP):
    drawDefaultItemUpdateMenu(eRTState);
    break;
  case (E_O2_INPUT):
    drawUpdateO2_InputMenu(eRTState);
    break;
  case (E_OP_MODE):
    drawUpdateOpModeMenu(eRTState);
    break;
  case (SHOW_VOL):
    drawSensorvoltageMenu(eRTState);
    break;
  case (SHOW_PRESSURE):
    drawSensorValueMenu(eRTState);
    break;
  default:
  {
    VENT_DEBUG_INFO("Unhandled Element", _currItem);
    Serial.println();
  }
  break;
  }
}

void displayManager::getItemIndx(void)
{

#if SERIAL_PRINTS
  VENT_DEBUG_INFO("Indicator", _editSeletIndicator);
  VENT_DEBUG_INFO("ScrollIndex", _editScrollIndex);
#endif

  if (params[_editSeletIndicator + _editScrollIndex].index != (_editSeletIndicator + _editScrollIndex))
  {
    VENT_DEBUG_ERROR("Invalid Edit", 0);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("  Edit Element Error ");
  }
  else
  {
    VENT_DEBUG_ERROR("Editing....", 0);
    Serial.println(params[_editSeletIndicator + _editScrollIndex].parm_name);
    _currItem = _editSeletIndicator + _editScrollIndex;
  }
}

void displayManager::stateMachine(void)
{
  switch (_dpState)
  {
  case STATUS_MENU_TO_EDIT_MENU:
  {
    _bRefreshEditScreen = true;
    _bEditItemSelected = false;
    _lastEditMenuTime = millis();
    _dpState = EDIT_MENU;
    //  drawEditMenu();
  }
  break;
  case EDIT_MENU:
  {
    if ((millis() - _lastEditMenuTime) > EDIT_MENU_TIMEOUT)
    {
      VENT_DEBUG_ERROR("Edit Menu Timeout", 0);
      _dpState = EDIT_MENU_TO_STATUS_MENU;
      return;
    }
    editModeEncoderInput();
    if (true == _bEditItemSelected)
    {
      getItemIndx();
      if (E_EXIT == _currItem)
        _dpState = EDIT_MENU_TO_STATUS_MENU;
      else
        _dpState = EDIT_MENU_TO_SUB_EDIT_MENU;
    }
    else
    {
      //No element selected for edit
      //display screen has to be updated if scroll bar is moved up or down
      if (true == _bRefreshEditScreen)
      {
        _lastEditMenuTime = millis();
        //    drawEditMenu();
      }
    }
    //element selected for edit
  }
  break;
    // case EDIT_MENU_TO_SUB_EDIT_MENU:
    // {
    //   lcd.clear();
    //   _bBack2EditMenu = false;
    //   _currentSaveFlag = false;
    //   _dpState = SUB_EDIT_MENU;
    //   _lastSubEditMenuTime = millis();
    // }
    // break;
    // // case SUB_EDIT_MENU:
    // {
    //   handleItemUpdate();
    //   if (true == _bBack2EditMenu)
    //   {
    //     _dpState = SUB_EDIT_MENU_TO_EDIT_MENU;
    //     _bBack2EditMenu = false;
    //   }
    //   if ((millis() - _lastSubEditMenuTime) > EDIT_MENU_TIMEOUT)
    //   {
    //     VENT_DEBUG_ERROR("SubEdit Menu Timeout", 0);
    //     _dpState = EDIT_MENU_TO_STATUS_MENU;
    //     return;
    //   }
    // }
    // break;
    // case SUB_EDIT_MENU_TO_EDIT_MENU:
    // {
    //   _bEditItemSelected = false;
    //   _currentSaveFlag = false;
    //   _lastEditMenuTime = millis();
    //   _bRefreshEditScreen = true;
    //   _dpState = EDIT_MENU;
    //   drawEditMenu();
    // }
    //  break;
  default:
  {
    VENT_DEBUG_ERROR("Unhandled State", _dpState);
  }
  }
}

void displayManager::drawRuntimeScreen1(void)
{
  blink = true;

  row[0] = '\0';
  drawRuntimeTopBottomLines(1, 3, ROTATE_CHAR, EDIT_CHAR);
  lcd.setCursor(0, 1);
  lcd.write("  TV    TVi   TVe ");
  lcd.setCursor(0, 2);
  sprintf(row, "  %3d  ", params[E_TV].value_curr_mem);
  lcd.print(row);
  if (tviErr > 0)
  {
    errorStatus = true;
    // digitalWrite(BUZZER_PIN, blink);
    lcd.write(DP_UP_TR);
  }
  else if (tviErr < 0)
  {
    errorStatus = true;
    // digitalWrite(BUZZER_PIN, blink);
    lcd.write(DP_DW_TR);
  }
  else
  {
    // digitalWrite(BUZZER_PIN, LOW);
    lcd.print(" ");
    errorStatus = false;
  }
  sprintf(row, "%3d  ", (int)m_display_tvi);
  lcd.print(row);
  if (tveErr > 0)
  {
    errorStatus = true;
    //digitalWrite(BUZZER_PIN, blink);
    lcd.write(DP_UP_TR);
  }
  else if (tveErr < 0)
  {
    errorStatus = true;
    // digitalWrite(BUZZER_PIN, blink);
    lcd.write(DP_DW_TR);
  }
  else
  {
    // digitalWrite(BUZZER_PIN, LOW);
    lcd.print(" ");
    errorStatus = false;
  }
  sprintf(row, "%3d", (int)m_display_tve);
  lcd.print(row);
  lcd.setCursor(4, 3);
  lcd.write("units : ml");

  return;
}

void displayManager::drawRuntimeScreen3(float *sensor_data)
{
  blink = true;

  row[0] = '\0';
  drawRuntimeTopBottomLines(3, 3, ROTATE_CHAR, EDIT_CHAR);
  lcd.setCursor(0, 1);
  lcd.write("  IER   RR    FiO2 ");
  lcd.setCursor(0, 2);
  sprintf(row, "  1:%1d  %2dbpm  ", params[E_IER].value_curr_mem, params[E_BPM].value_curr_mem);
  lcd.print(row);
  if ((int)sensor_data[SENSOR_O2] > 1.1 * params[E_FiO2].value_curr_mem)
  {
    errorStatus = true;
    lcd.write(DP_UP_TR);
  }
  else if ((int)sensor_data[SENSOR_O2] < 0.9 * params[E_FiO2].value_curr_mem)
  {
    errorStatus = true;
    lcd.write(DP_DW_TR);
  }
  else
  {
    lcd.print(" ");
    errorStatus = false;
  }
  sprintf(row, "%d%%  ", (int)sensor_data[SENSOR_O2]);
  lcd.print(row);
  return;
}

void displayManager::drawRuntimeScreen2(void)
{
  row[0] = '\0';
  dtostrf(m_display_pip, 4, 1, str_temp);
  sprintf(buffer, "%s", str_temp);
  dtostrf(m_display_plat, 4, 1, str_temp);
  sprintf(buffer1, "%s", str_temp);
  dtostrf(m_display_peep, 4, 1, str_temp);
  sprintf(buffer2, "%s", str_temp);
  drawRuntimeTopBottomLines(2, 3, ROTATE_CHAR, EDIT_CHAR);
    lcd.setCursor(0, 4);
    lcd.print("                ");
 // lcd.write("  PIP   Plat  PEEP ");
  lcd.setCursor(0, 1);
  lcd.write("  PIP   Plat  PEEP ");
  lcd.setCursor(0, 2);
  sprintf(row, "  %s  %s  %s ", buffer, buffer1, buffer2);
  lcd.print(row);
  lcd.setCursor(2, 3);
  lcd.write("   units : cmH2o");
  return;
}

void displayManager::clearDisplay(void)
{
  lcd.setCursor(0, 0);
  lcd.write(clearScreenBuffer);
  lcd.write(clearScreenBuffer);
  lcd.write(clearScreenBuffer);
  lcd.write(clearScreenBuffer);
  return;
}

void displayManager::drawRuntimeTopBottomLines(int currentPage, int totalPages, int topRight, int bottomLeft)
{
  static char buffer[4];
  lcd.setCursor(19, 0);
  lcd.write((byte)topRight);

  if (totalPages == 4)
  {
    if (currentPage != 4)
    {
      lcd.setCursor(19, 3);
      lcd.write((byte)bottomLeft);
      if (currentPage != 1)
      {
        sprintf(buffer, "%d/%d", currentPage, totalPages);
        lcd.setCursor(0, 0);
        lcd.write(buffer);
      }
    }
    else
    {
      sprintf(buffer, "%d/%d", currentPage, totalPages);
      lcd.setCursor(0, 0);
      lcd.write(buffer);
      lcd.setCursor(19, 3);
      lcd.write((byte)bottomLeft);
    }
  }
  else
  {
    lcd.setCursor(19, 3);
    lcd.write((byte)bottomLeft);
    sprintf(buffer, "%d/%d", currentPage, totalPages);
    lcd.setCursor(0, 0);
    lcd.write(buffer);
  }
  return;
}
void displayManager::displayStatusScreen(float *sensor_data, int statusScreenIndex)
{
  switch (statusScreenIndex)
  {
  case 0:
    drawRuntimeScreen1();
    break;
  case 1:
    drawRuntimeScreen2();
    break;
  case 2:
    drawRuntimeScreen3(sensor_data);
    break;
  default:
    break;
  }
  blink = false;
}

// void displayManager::displayRunTime(float *sensor_data)
// {

//   if ((true == _refreshRunTimeDisplay) || (true == refreshfullscreen_inhale) || (true == refreshfullscreen_exhale && (millis() > exhale_refresh_timeout)))
//   {

//     blink = true;

//     row[0] = '\0';
//     if (true == _refreshRunTimeDisplay)
//     {
//       lcd.clear();
//       lcd.setCursor(0, 0);
//       _refreshRunTimeDisplay = false;
//     }
//     {
//       // row0 start
//       row[0] = '\0';
//       sprintf(row, "TV  %3d RR %2d IE 1:%1d", params[E_TV].value_curr_mem, params[E_BPM].value_curr_mem, params[E_IER].value_curr_mem);
//       lcd.setCursor(0, 0);
//       lcd.print(row);
//     }
//     {
//       row[0] = '\0';
//       dtostrf(m_display_pip, 4, 1, str_temp);
//       sprintf(buffer, "%s", str_temp);
//       sprintf(row, "%3d ", (int)m_display_tvi);
//       lcd.setCursor(0, 1);
//       lcd.print("TVi");
//       if (tviErr > 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_UP_TR);
//         else
//           lcd.write(DP_EM_UP_TR);
//       }
//       else if (tviErr < 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_DW_TR);
//         else
//           lcd.write(DP_EM_DN_TR);
//       }
//       else
//       {
//         digitalWrite(BUZZER_PIN, LOW);
//         lcd.print(" ");
//       }
//       lcd.print(row);
//       lcd.setCursor(8, 1);
//       lcd.print("PIP ");
//       if (pipErr > 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_UP_TR);
//         else
//           lcd.write(DP_EM_UP_TR);
//       }
//       else if (pipErr < 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_DW_TR);
//         else
//           lcd.write(DP_EM_DN_TR);
//       }
//       else
//       {
//         digitalWrite(BUZZER_PIN, LOW);
//         lcd.print(" ");
//       }
//       sprintf(row, "%s", buffer);
//       lcd.print(row);
//     }
//     //3rd row
//     {
//       row[0] = '\0';
//       dtostrf(m_display_plat, 4, 1, str_temp);
//       sprintf(buffer, "%s", str_temp);
//       sprintf(row, " %3d Plat %s", (int)m_display_tve, buffer);
//       lcd.setCursor(0, 2);
//       lcd.print("TVe");
//       lcd.print(row);
//       lcd.setCursor(18, 2);
//       // display CMV or SIMV
//       if (CMV == params[E_OP_MODE].value_curr_mem)
//       {
//         lcd.write("CV");
//       }
//       else if (SIMV == params[E_OP_MODE].value_curr_mem)
//       {
//         lcd.write("SV");
//       }
//       else
//       {
//         lcd.write("**");
//       }
//     }
//     {
//       row[0] = '\0';
//       dtostrf(m_display_peep, 4, 1, str_temp);
//       sprintf(buffer, "%s", str_temp);
//       sprintf(row, "%2d%% ", (int)sensor_data[SENSOR_O2]);
//       lcd.setCursor(0, 3);
//       lcd.write(DP_FI);
//       lcd.print("O2");
//       if ((int)sensor_data[SENSOR_O2] > 1.1 * params[E_FiO2].value_curr_mem)
//       {
//         if (blink)
//           lcd.write(DP_UP_TR);
//         else
//           lcd.write(DP_EM_UP_TR);
//       }
//       else if ((int)sensor_data[SENSOR_O2] < 0.9 * params[E_FiO2].value_curr_mem)
//       {
//         if (blink)
//           lcd.write(DP_DW_TR);
//         else
//           lcd.write(DP_EM_DN_TR);
//       }
//       else
//       {
//         lcd.print(" ");
//       }
//       lcd.print(row);

//       lcd.setCursor(8, 3);
//       sprintf(row, "%s", buffer);
//       lcd.print("PEEP");
//       if (peepErr > 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_UP_TR);
//         else
//           lcd.write(DP_EM_UP_TR);
//       }
//       else if (peepErr < 0)
//       {
//         digitalWrite(BUZZER_PIN, blink);
//         if (blink)
//           lcd.write(DP_DW_TR);
//         else
//           lcd.write(DP_EM_DN_TR);
//       }
//       else
//       {
//         digitalWrite(BUZZER_PIN, LOW);
//         lcd.print(" ");
//       }
//       lcd.print(row);
//     }
//     lcd.setCursor(19, 3);
//     if (true == machineOn)
//     {
//       if (blink)
//         lcd.print("R");
//       else
//         lcd.print(" ");
//     }
//     else
//     {
//       lcd.print("S");
//     }
//     refreshfullscreen_inhale = false;
//     refreshfullscreen_exhale = false;
//   }
//   else if (last_o2update < millis())
//   {
//     last_o2update = millis() + 500;
//     row[0] = '\0';
//     sprintf(row, "%2d%% ", (int)sensor_data[SENSOR_O2]);
//     lcd.setCursor(3, 3);
//     if ((int)sensor_data[SENSOR_O2] > 1.1 * params[E_FiO2].value_curr_mem)
//     {
//       if (blink)
//         lcd.write(DP_UP_TR);
//       else
//         lcd.write(DP_EM_UP_TR);
//     }
//     else if ((int)sensor_data[SENSOR_O2] < 0.9 * params[E_FiO2].value_curr_mem)
//     {
//       if (blink)
//         lcd.write(DP_DW_TR);
//       else
//         lcd.write(DP_EM_DN_TR);
//     }
//     else
//     {
//       lcd.print(" ");
//     }
//     lcd.print(row);
//     blink = false;
//     digitalWrite(BUZZER_PIN, LOW);
//   }
// }

void displayManager::displayManagerSetup()
{
  // set to edit menu state and trigger state machine
  _dpState = STATUS_MENU_TO_EDIT_MENU;
  volatile STATE dpStateTemp;
  do
  {
    dpStateTemp = _dpState;
    stateMachine();
    if (dpStateTemp != _dpState)
    {
      Serial.print("(setup)dpState Change from : ");
      Serial.print(dpStatusString(dpStateTemp));
      Serial.print("-> ");
      Serial.println(dpStatusString(_dpState));
    }
  } while (EDIT_MENU_TO_STATUS_MENU != _dpState);
  // at the end of the setup move to show status menu
  _dpState = STATUS_MENU;
  _refreshRunTimeDisplay = true;
  _editSeletIndicator = 0;
  _editScrollIndex = 0;
}

void displayManager::displayManagerloop(float *sensor_data, sensorManager &sM)
{
  RT_Events_T eRTState = RT_NONE;
  
  m_sM = &sM;
  volatile STATE dpStateTemp = _dpState;
  if (drawSplashScreenMenu)
  {

    // delay(500);

    // lcd.clear();
    // RT_Events_T eRTState = RT_NONE;
    // wait = 0;
    // while (continueLoop)
    {
      eRTState = encoderScanUnblocked();

      switch (eRTState)
      {
      case RT_DEC:
        serviceLevelIndex = serviceLevelIndex + 1;
        _refreshSplashEditScreen = true;
        lcd.clear();
        break;
      case RT_INC:
        serviceLevelIndex = serviceLevelIndex - 1;
        _refreshSplashEditScreen = true;
        lcd.clear();
        break;

      case RT_NONE:
        //serviceLevelIndex = serviceLevelIndex+0;
        _refreshSplashEditScreen = true;
        break;
      default:
        break;
      }
      switch (abs(serviceLevelIndex % 4))
      {
      case 0:
        drawOxygenCalibScreen(eRTState, sM);
        break;
      case 1:
        drawDiagnosticScreen(eRTState);
        break;
      case 2:
        drawSensorValueMenu(eRTState);
        break;
      case 3:
        drawSensorvoltageMenu(eRTState);
        break;
      default: //drawOxygenCalibScreen(eRTState,sM);
        break;
      }
      delay(50);
    }
  }

  else if (STATUS_MENU == _dpState)
  {
    eRTState = encoderScanUnblocked();
    switch (eRTState)
    {
    case RT_INC:
      clearDisplay();
      statusScreenIndex = statusScreenIndex + 1;
      _refreshRunTimeDisplay = true;
      break;
    case RT_DEC:
      clearDisplay();
      statusScreenIndex = statusScreenIndex - 1;
      _refreshRunTimeDisplay = true;
      break;
    case RT_BT_PRESS:
      _dpState = EDIT_MENU;
      clearDisplay();
      settingScreenIndex = 0;
      _lastEditMenuTime = millis();
      break;
    case RT_NONE:
      break;
    default:
      break;
    }
    if ((true == _refreshRunTimeDisplay) || (true == refreshfullscreen_inhale) || (true == refreshfullscreen_exhale && (millis() > exhale_refresh_timeout))) //
    {
      if (machineOn && errorStatus)
      {
        lcd.setCursor(0, 3);
        lcd.write(byte(ERR_CHAR));
      }
      displayStatusScreen(sensor_data, abs(statusScreenIndex % 3));
      _refreshRunTimeDisplay = false;
      refreshfullscreen_inhale = false;
      refreshfullscreen_exhale = false;
    }
  }
  if ((eRTState == RT_BT_PRESS) || (STATUS_MENU != _dpState))
  {

    m_o2_sensor_data = sensor_data[SENSOR_O2];
    if ((millis() - _lastEditMenuTime) > EDIT_MENU_TIMEOUT)
    {
      _dpState = STATUS_MENU;
      clearDisplay();
      _refreshRunTimeDisplay = true;
    }
    else
    {
      drawEditMenu();
    }
  }
  // else
  // {
  //  // clearDisplay();
  //   drawEditMenu();
  // }

  // if ((eRTState == RT_BT_PRESS) || (STATUS_MENU != _dpState))
  // {
  //   m_o2_sensor_data = sensor_data[SENSOR_O2];
  //   if ((dpStateTemp != _dpState) || (eRTState == RT_BT_PRESS))
  //   {
  //     Serial.print("before SM: dpState Changed from : ");
  //     Serial.print(dpStatusString(dpStateTemp));
  //     Serial.print("-> ");
  //     Serial.println(dpStatusString(_dpState));
  //   }
  //   if (STATUS_MENU == _dpState)
  //   {
  //     _dpState = STATUS_MENU_TO_EDIT_MENU;
  //   }
  //   stateMachine();
  //   if (EDIT_MENU_TO_STATUS_MENU == _dpState)
  //   {
  //     _dpState = STATUS_MENU;
  //     _editSeletIndicator = 0;
  //     _editScrollIndex = 0;
  //     _refreshRunTimeDisplay = true;
  //   }

  //   if ((dpStateTemp != _dpState) || (eRTState == RT_BT_PRESS))
  //   {
  //     VENT_DEBUG_INFO("State Change from", dpStatusString(dpStateTemp));
  //     VENT_DEBUG_INFO("State Change to", _dpState);
  //   }
  // }
}

void displayManager::errorDisplay(ErrorDef_T errorState)
{
  switch (errorState)
  {
  case (ERR_OXY):
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("       ERROR");
    lcd.setCursor(0, 1);
    lcd.print(OXY_FAILURE_MSG1);
    lcd.setCursor(0, 2);
    lcd.print(OXY_FAILURE_MSG2);
  }
  break;
  case (ERR_PATIENT_DISCONN):
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("       ERROR");
    lcd.setCursor(0, 1);
    lcd.print(PATIENT_DISCONN_MSG1);
    lcd.setCursor(0, 2);
    lcd.print(PATIENT_DISCONN_MSG2);
  }
  break;
  case (ERR_PEEP):
    break;
  case (ERR_BVM):
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.write("!!!   Error   !!! ");
    lcd.setCursor(0, 2);
    lcd.print(BVM_FAILURE_MSG1);
    lcd.setCursor(0, 3);
    lcd.print(BVM_FAILURE_MSG2);
    delay(5);
    lcd.clear();
  }
  break;
  case (ERR_TV):
    break;
  default:
    break;
  }
  digitalWrite(BUZZER_PIN, blink);
  bvmFailure = false;
}
