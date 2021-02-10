#include "service_mode.h"
//#include "ctrl_display.cpp"
/*
void drawSplashScreen(sensorManager &sM,displayManager &dM)
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
      wait=0;
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

  //   if (millivolt_flag)
  // {
  //   mcu0_enable_sensors_voltage(false);
  //   millivolt_flag = false;
  // }

  // if (pressure_flag)
  // {
  //   mcu0_enable_sensors_pressure(false);
  //   pressure_flag = false;
  // }

  lcd.setCursor(19, 0);
  lcd.write((byte)(0x3));

  int actualPotValue2 = analogRead(PMAX_PIN);
  float convVal2 = map(actualPotValue2, 0, POT_HIGH, 100, 0);
  actualPotValue2 = (constrain((int)convVal2, 0, 100) % 5);
  //int actualPotValue2 = ((int)convVal2) % 4;

  static char buffer[4];
  sprintf(buffer, "%d/%d", 1, 1);
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

  // if (millivolt_flag)
  // {
  //   mcu0_enable_sensors_voltage(false);
  //   millivolt_flag = false;
  // }

  // if (pressure_flag)
  // {
  //   mcu0_enable_sensors_pressure(false);
  //   pressure_flag = false;
  // }

  static char buffer[4];
  lcd.setCursor(19, 0);
  lcd.write((byte)(0x3));
  int actualPotValue1 = analogRead(RR_PIN);
  float convVal1 = map(actualPotValue1, 0, POT_HIGH, 100, 0);
  actualPotValue1 = (constrain((int)convVal1, 1, 100) % 3);

  int actualPotValue2 = analogRead(PMAX_PIN);
  float convVal2 = map(actualPotValue2, 0, POT_HIGH, 100, 0);
  actualPotValue2 = (constrain((int)convVal2, 1, 100) % 4);

  sprintf(buffer, "%d/%d", 1, 2);
  lcd.setCursor(0, 0);
  lcd.write(buffer);
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
      float avgO2Value;
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
  return;
}
*/