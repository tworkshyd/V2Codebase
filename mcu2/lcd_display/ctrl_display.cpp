#include "../BoardDefines.h"

#include "ctrl_display.h"
#include "../sensors/o2_sensor.h"

String saveFlag = "Save  ";
String cancelFlag = "Cancel";
int count = 0;
static int settingScreenIndex = 0;
// macros here
#define ROT_ENC_FOR_IER (_currItem == inex_rati.index)
#define ROT_ENC_FOR_PEEP (_currItem == peep_pres.index)
#define EDIT_MENU_TIMEOUT 7000

#define BOOTUP_SETUP_SCREENS 3
#define MAIN_SCREENS 4
#define ABOUT_SCREENS 1

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
  DP_FI,
  NULL_CHAR

} DP_CH_T;

static char str_temp[6];
static char buffer[6];
static char buffer1[6];
static char buffer2[6];
static char row[30] = "";

static bool blink = true;
static unsigned long last_o2update = 0;


void initRowBuffer(){
	for ( int i = 0 ; i < 30 ; i++ ){
		row[i] = ' ' ;
	}
}

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

void displayManager::drawDiagnosticScreen(RT_Events_T eRTState)
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
  drawRuntimeTopBottomLines(4, BOOTUP_SETUP_SCREENS, ROTATE_CHAR, EDIT_CHAR);
  
  int actualPotValue2 = analogRead(PMAX_PIN);
  float convVal2 = map(actualPotValue2, 0, POT_HIGH, 100, 0);
  actualPotValue2 = (constrain((int)convVal2, 0, 100) % 5);
  //int actualPotValue2 = ((int)convVal2) % 4;

  lcd.setCursor(4, 0);
  lcd.write(" Diagnostics   ");

  lcd.setCursor(13, 3);
  lcd.write((byte)(0x2));

  lcd.setCursor(8, 2);
  lcd.write("<");
  lcd.print(diagnosticFuncName[actualPotValue2]);
  lcd.write(">");

  if (RT_BT_PRESS == eRTState)
  {
    (*diagnosticFunc_arr[actualPotValue2])();
    _refreshSplashEditScreen = false;
    lcd.clear();
  }
  return;
}
void displayManager::drawOxygenCalibScreen(RT_Events_T eRTState, sensorManager sM)
{
  drawRuntimeTopBottomLines(1, BOOTUP_SETUP_SCREENS, ROTATE_CHAR, EDIT_CHAR);
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

  lcd.setCursor(3, 0);
  lcd.print(" O2 Calibration");

  lcd.setCursor(1, 2);
  lcd.write("<");
  lcd.print(subMenu1[actualPotValue1]);
  lcd.write(">");

  // clear to draw the next value
  lcd.setCursor(8, 2);
  lcd.write("          ");
  
  lcd.setCursor(8, 2);
  lcd.write("<");
  if (actualPotValue2 == 0)
  {
    lcd.print(" ");
	long int calibValue = 0 ;
	calibValue = retrieve_sensor_data_long( EEPROM_O2_CALIB_ADDR + (actualPotValue1 * sizeof(long int) ) );
	lcd.print( calibValue / O2_CALIBRATION_VOLTAGE_ACCURACY);
    //lcd.print(retrieveCalibParam(EEPROM_O2_CALIB_ADDR + (actualPotValue1 * 2)));

    lcd.print("mv  ");
  }
  else
  {
    lcd.print(oxySettings[actualPotValue2]);
  }
  lcd.setCursor(18, 2);
  lcd.write(">");

  lcd.setCursor(5, 3);
  lcd.write((byte)(0x2));
  lcd.setCursor(13, 3);
  lcd.write((byte)(0x2));

  if (eRTState == RT_BT_PRESS)
  {
    if (actualPotValue2 == 2)
    {
      float avgO2Value = 0;
	  long int savedSample = 0 ;

	  for (int i = 0; i < 10; i++)
      {
        sM.capture_and_read_data(SENSOR_O2);
        avgO2Value += sM.read_sensor_rawvoltage(SENSOR_O2);
      }
	  
	  savedSample = (avgO2Value / 10.0) * 100.0  ;
	  VENT_DEBUG_INFO("O2 Calib Value Save : ", savedSample ) ;
	  
      store_sensor_data_long(EEPROM_O2_CALIB_ADDR + (actualPotValue1 * sizeof (long int) ), savedSample );
      sM.init(O2);
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

  o2_sensor::reset_calibration_data(i);

  lcd.setCursor(2, 3);
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
  return;
}
void ExitDaignostic()
{
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
    break;
  case RT_DEC:
    break;
  case RT_BT_PRESS:
    bCalibrate = true;
    break;
  case RT_NONE:
    break;
  default:
    break;
  }
  if ((millis() - _lastDisplayTime > 500) ||
      (eRTState != RT_NONE))
  {
    drawRuntimeTopBottomLines(2, BOOTUP_SETUP_SCREENS, ROTATE_CHAR, EDIT_CHAR);
    lcd.setCursor(8, 1);
    lcd.print("  ");
    lcd.setCursor(8, 2);
    lcd.print("  ");
    lcd.setCursor(5, 0);
    lcd.print("Sensor cmH2o");
    lcd.setCursor(0, 1);
    lcd.print("GP1:");
    lcd.print(mcu0_read_pressure(SENSOR_PRESSURE_A0));
    DebugPort.print("GP1: ");
    DebugPort.print(mcu0_read_pressure(SENSOR_PRESSURE_A0));

    lcd.setCursor(10, 1);
    lcd.print("DP1:");
    lcd.print(m_sM->read_sensor_pressurevalues(SENSOR_DP_A0));
    lcd.setCursor(0, 2);
    lcd.print("GP2:");
    lcd.print(mcu0_read_pressure(SENSOR_PRESSURE_A1));
    DebugPort.print("\tGP2: ");
    DebugPort.println(mcu0_read_pressure(SENSOR_PRESSURE_A1));
    lcd.setCursor(10, 2);
    lcd.print("DP2:");
    lcd.print(m_sM->read_sensor_pressurevalues(SENSOR_DP_A1));
    lcd.setCursor(0, 3);
  }
  if (bCalibrate)
  {
    lcd.setCursor(0, 3);
    lcd.print(" Calibrating...");
    serial2_rxdata = "";
    //rxdata_buff="";
    m_sM->enable_sensor(0);
    m_sM->start_calibration();
    m_sM->enable_sensor(0);
    mcu0_enable_sensors_pressure(false);
    mcu0_calibrate_sensor();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    _refreshEditScreenDisplay = false;
    _refreshRunTimeDisplay = true;
    bCalibrate = false;
    delay(5000);
	pressure_flag = false;
#if 0
	drawMessageScreen(" " , 1,2, "Restart The Device");

	while ( 1 ){
		delay(50);
	}
#endif
	
    return;
  }
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

    drawRuntimeTopBottomLines(3, BOOTUP_SETUP_SCREENS, ROTATE_CHAR, EDIT_CHAR);
    lcd.setCursor(7, 1);
    lcd.print("   ");
    lcd.setCursor(7, 2);
    lcd.print("   ");
    lcd.setCursor(6, 0);
    lcd.print("Sensor mV");
    lcd.setCursor(0, 1);
    lcd.print("GP1:");
    lcd.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A0));
    DebugPort.print("mvGP1: ");
    DebugPort.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A0));

    lcd.setCursor(10, 1);
    lcd.print("DP1:");
    lcd.print(m_sM->read_sensor_rawvoltage(SENSOR_DP_A0));

    lcd.setCursor(0, 2);
    lcd.print("GP2:");
    lcd.print(mcu0_read_rawvoltage(SENSOR_PRESSURE_A1));
    DebugPort.print("\tmvGP2: ");
    DebugPort.println(mcu0_read_rawvoltage(SENSOR_PRESSURE_A1));

    lcd.setCursor(10, 2);
    lcd.print("DP2:");
    lcd.print(m_sM->read_sensor_rawvoltage(SENSOR_DP_A1));
    int o2value = m_sM->read_sensor_rawvoltage(SENSOR_O2);
    lcd.setCursor(7, 3);
    lcd.print("O2: ");
    lcd.print(o2value);
  }
}

void displayManager::editMenuHandler(RT_Events_T eRTState)
{
  drawRuntimeTopBottomLines(0, MAIN_SCREENS, ROTATE_CHAR, SAVE_CHAR);
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
    bSave = true;
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
      lcd.print(" Saved.....");
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
  switch (abs(settingScreenIndex % MAIN_SCREENS))
  {
  case 0:
    editMenuHandler(eRTState);
    break;
  case 1:
    lcdSettingScreen(eRTState);
    break;
  case 2:
    fio2SettingScreen(eRTState);
    break;
  case 3:
    aboutScreen(eRTState);
    break;
  default:
    break;
  }
}
void displayManager::lcdSettingScreen(RT_Events_T eRTState)
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
  drawRuntimeTopBottomLines(2, MAIN_SCREENS, ROTATE_CHAR, SAVE_CHAR);
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
  if (diffValue1 > 5)
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
    lcd.print(" Saved.....");
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
void displayManager::fio2SettingScreen(RT_Events_T eRTState)
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
  drawRuntimeTopBottomLines(3, MAIN_SCREENS, ROTATE_CHAR, SAVE_CHAR);
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
  if (diffValue1 > 5)
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
    lcd.print(" Saved.....");
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
void displayManager::aboutScreen(RT_Events_T eRTState)
{
  showAboutScreenSubMenu = false;
  drawRuntimeTopBottomLines(4, MAIN_SCREENS, ROTATE_CHAR, EDIT_CHAR);
  lcd.setCursor(7, 0);
  lcd.write("About");
  lcd.setCursor(1, 1);
  lcd.write("Device   : BMV");
  lcd.setCursor(1, 2);
  lcd.write("Serial No: TW0002");
  lcd.setCursor(1, 3);

  lcd.write("Version  : V3.02");

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
      while (!showAboutScreenSubMenu)
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
  drawRuntimeTopBottomLines(1, ABOUT_SCREENS, ROTATE_CHAR, SAVE_CHAR);

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
  	
	lcd.setCursor(13, 3);
	lcd.print("    ");
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

  /// Reset The Oxygen Sensor Calibration Default Values 
  ResetO2(0);ResetO2(1);ResetO2(2);
  sM.init(O2);

  lcd.setCursor(2, 3);
  lcd.write("Reset Done");
  delay(2000);
  showAboutScreenSubMenu = true;

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
  
  /// Reset The Oxygen Sensor Calibration Default Values 
  ResetO2(0);ResetO2(1);ResetO2(2);
  sM.init(O2);

  lcd.setCursor(2, 3);
  lcd.write("Reset Done");
  delay(2000);
  showAboutScreenSubMenu = true;
  //cleanRow(3);
  lcd.clear();
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

void displayManager::tidalVolumeStatusScreen(void)
{
  int errorIn_TV = 0;
  blink = true;

  /// reset the buffer with spaces
  initRowBuffer();

  row[0] = '\0';

  drawRuntimeTopBottomLines(1, 3, ROTATE_CHAR, EDIT_CHAR);
  lcd.setCursor(0, 1);
  lcd.write("  TV    TVi   TVe ");
  lcd.setCursor(0, 2);

  if( machineOn == false ) {
  	
	  /// Check validity and set to the lower/upper bounds
	  if ( params[E_TV].value_curr_mem < params[E_TV].min_val ) {
	  	params[E_TV].value_curr_mem = params[E_TV].min_val;
		errorIn_TV = 1;
	  } else if ( params[E_TV].value_curr_mem > params[E_TV].max_val ) {
	  	params[E_TV].value_curr_mem = params[E_TV].max_val;
	    errorIn_TV = 1;
	  }
 }
  
  sprintf(row, "  %3d  ", params[E_TV].value_curr_mem);
  lcd.print(row);

  /// Check for TVi Error and set alarm
  if (tviErr > 0)
  {
    errorStatus = true;
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, blink);
#endif
    lcd.write(DP_UP_TR);
  }
  else if (tviErr < 0)
  {
    errorStatus = true;
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, blink);
#endif
    lcd.write(DP_DW_TR);
  }
  else
  {
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, LOW);
#endif
    lcd.print(" ");
    errorStatus = false;
  }

  if( machineOn == false ) {

	  /// Check validity and set to the lower/upper bounds
	  if ( m_display_tvi < params[E_TV].min_val ) {
		m_display_tvi = params[E_TV].min_val;
	    errorIn_TV = 1;
	  } else if ( m_display_tvi > params[E_TV].max_val ) {
	  	m_display_tvi = params[E_TV].max_val;
	    errorIn_TV = 1;	
	  }
  }
   
  sprintf(row, "%3d  ", (int)m_display_tvi);
  lcd.print(row);
  if (tveErr > 0)
  {
    errorStatus = true;
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, blink);
#endif
    lcd.write(DP_UP_TR);
  }
  else if (tveErr < 0)
  {
    errorStatus = true;
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, blink);
#endif
    lcd.write(DP_DW_TR);
  }
  else
  {
#if ENABLE_BUZZER
    digitalWrite(BUZZER_PIN, LOW);
#endif
    lcd.print(" ");
    errorStatus = false;
  }

  if( machineOn == false ) {

	  /// Check validity and set to the lower/upper bounds
	  if ( m_display_tve < params[E_TV].min_val ) {
		m_display_tve = params[E_TV].min_val;
	    errorIn_TV = 1;	
	  } else if ( m_display_tve > params[E_TV].max_val ) {
	  	m_display_tve = params[E_TV].max_val;
	    errorIn_TV = 1;	
	  }
  }

  sprintf(row, "%3d   ", (int)m_display_tve);
  lcd.print(row);
  lcd.setCursor(4, 3);
  lcd.write("units : ml");

  if( errorIn_TV == 1 ){
  	VENT_DEBUG_ERROR( "TV Value Error" , errorIn_TV ) ;
  }

  return;
}

void displayManager::ierRrOxygenStatusScreen(float *sensor_data)
{
  blink = true;

  initRowBuffer();
  
  row[0] = '\0';
  
  drawRuntimeTopBottomLines(3, 3, ROTATE_CHAR, EDIT_CHAR);
  
  lcd.setCursor(0, 1);
  lcd.write("  IER   RR    FiO2 ");


  lcd.setCursor(0, 2);
  
  if( machineOn == false ) {
	  /// Check validity and set to the lower/upper bounds
	  if ( params[E_IER].value_curr_mem < params[E_IER].min_val ) {
		params[E_IER].value_curr_mem = params[E_IER].min_val;
	  } else if ( params[E_IER].value_curr_mem > params[E_IER].max_val ) {
	  	params[E_IER].value_curr_mem = params[E_IER].max_val;
	  }

	  /// Check validity and set to the lower/upper bounds
	  if ( params[E_BPM].value_curr_mem < params[E_BPM].min_val ) {
		params[E_IER].value_curr_mem = params[E_IER].min_val;
	  } else if ( params[E_BPM].value_curr_mem > params[E_BPM].max_val ) {
	  	params[E_BPM].value_curr_mem = params[E_BPM].max_val;
	  }

	  if ( params[E_FiO2].value_curr_mem < params[E_FiO2].min_val ) {
		params[E_FiO2].value_curr_mem = params[E_FiO2].min_val;
	  } else if ( params[E_FiO2].value_curr_mem > params[E_FiO2].max_val ) {
	  	params[E_FiO2].value_curr_mem = params[E_FiO2].max_val;
	  }
  }
  
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

void displayManager::pressureSensorsStatusScreen(void)
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
  lcd.write("  units : cmH2o ");
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

  if( topRight != NULL_CHAR ) {
	  lcd.setCursor(19, 0);
	  lcd.write((byte)topRight);
  }
  
  if ( bottomLeft != NULL_CHAR ) {

	lcd.setCursor(19, 3);
    lcd.write((byte)bottomLeft);

  }

  if( currentPage != 0 ) {

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
    tidalVolumeStatusScreen();
    break;
  case 1:
    pressureSensorsStatusScreen();
    break;
  case 2:
    ierRrOxygenStatusScreen(sensor_data);
    break;
  default:
    break;
  }
  blink = false;
}

void displayManager::displayManagerloop(float *sensor_data, sensorManager &sM)
{
  RT_Events_T eRTState = RT_NONE;
  
  m_sM = &sM;
  volatile STATE dpStateTemp = _dpState;
  if (drawSplashScreenMenu) {
    
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

        _refreshSplashEditScreen = true;
        break;
      default:
        break;
      }
      switch (abs(serviceLevelIndex % BOOTUP_SETUP_SCREENS))
      {
      case 0:
        drawOxygenCalibScreen(eRTState, sM);
        break;
      case 1:
        drawSensorValueMenu(eRTState);
        break;
      case 2:
        drawSensorvoltageMenu(eRTState);
        break;
      case 3:
        drawDiagnosticScreen(eRTState);
        break;

    default:
      break;
    }
    
  }  else if (STATUS_MENU == _dpState)  {
  
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
}

void displayManager::errorDisplay(ErrorDef_T errorState)
{
  switch (errorState)
  {
  case (ERR_OXY):
  {
	drawMessageScreen(OXY_FAILURE_MSG1,1,2,OXY_FAILURE_MSG2 );
	delay(100);
    clearDisplay();
  }
  break;
  case (ERR_PATIENT_DISCONN):
  {
  
	drawMessageScreen(PATIENT_DISCONN_MSG1,1,2,PATIENT_DISCONN_MSG1 );
    clearDisplay();
  }
  break;
  case (ERR_PEEP):
    break;
  case (ERR_BVM):
  {
    drawMessageScreen( BVM_FAILURE_MSG1,5,2,BVM_FAILURE_MSG2 );
    clearDisplay();
  }
  break;
  case (ERR_TV):
    break;
  default:
    break;
  }

#ifdef ENABLE_BUZZER
  digitalWrite(BUZZER_PIN, blink);
#endif

  bvmFailure = false;
}

void displayManager::drawMessageScreen(const char* pHeading, int xPos, int yPos, const char* pMessage) {

	/// Clear the Screen 
	clearDisplay();

	/// Draw Heading 
	if( pHeading ) {
		lcd.setCursor (2,0);
		lcd.print(pHeading);
	}
	
	/// Draw the Message 
	if ( pMessage ) {
		lcd.setCursor(xPos,yPos);
		lcd.print(pMessage);
	}

    delay(100);
	
	return;
}

