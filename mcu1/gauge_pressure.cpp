#include "Arduino.h"
#include "gauge_pressure.h"
const unsigned int MAX_SAMPLE_COUNT = 4;
const unsigned int MAX_COUNT_FOR_CALIBRATION = 100;
//const unsigned int MAX_SAMPLE_COUNT = 1;


static float ADC_ApplyAvgFilter(int *SampleBuf, int SampleCount) {
  float SumValue = 0.0;
  float SensorVolts = 0.0;
  float ADCAnalogValues[SampleCount] ;
  for (int i = 0; i < SampleCount; i++) {
    ADCAnalogValues[i] = SampleBuf[i] ;
    SumValue += ADCAnalogValues[i];
  }
  SensorVolts = SumValue / SampleCount;
  return (SensorVolts);
}

float ADC_ReadVolageATMega(int channel)
{
  int ADCSampleBuff[MAX_SAMPLE_COUNT];
  float SensorVolts = 0.0;

  for (unsigned int i = 0; i < MAX_SAMPLE_COUNT; i++) {
    ADCSampleBuff[i] = analogRead(channel);
  }
  SensorVolts = ADC_ApplyAvgFilter(ADCSampleBuff, MAX_SAMPLE_COUNT);
  // DebugPort.print("Analog Voltage");
  // DebugPort.println(SensorVolts);
  return SensorVolts;

}

/*
  Function to return the pressure from the sensor
  vout = vs(0.09P + 0.04) + 5%VFSS
  P = (vout - (0.005*VFSS) - (Vs*0.04))/(VS * 0.09)
*/
float get_pressure_MPX5010(int channel, float *raw_voltage) {
  float pressure = 0.0;
  float vout = 0.0;
  int err = 0;
  vout = ADC_ReadVolageATMega(channel) * 0.0048828125;
  //  if (channel == 0) {
  //     iRaw = vout;
  //  }
  //  if (channel == 1) {
  //    eRaw = vout;
  //  }


  pressure = ((vout - (MPX5010_ACCURACY) - (MPX5010_VS * 0.04)) / (MPX5010_VS * 0.09));
  // Error correction on the pressure, based on the H2O calibration
  pressure = ((pressure - 0.07) / 0.09075);

  //  DebugPort.print("channel :");
  //  DebugPort.print(channel);
  //  DebugPort.print(", pressure :");
  //  DebugPort.println(pressure);
  //  DebugPort.print(", raw_voltage :");
  //  DebugPort.println(vout * 1000);

  if (raw_voltage != NULL)
    *raw_voltage = vout * 1000;

  return pressure;
}

static float cal_offset_PRESSURE_A0 = 0.0;
static float cal_offset_PRESSURE_A1 = 0.0;

static void calculate_zero_offset(sensor_e s_idx) {
  float ADCSampleBuff[MAX_COUNT_FOR_CALIBRATION] = {0};
  int channel = 0;

  if ( SENSOR_PRESSURE_A0 == s_idx) {
    channel = 0; // change these values as per BOM
    cal_offset_PRESSURE_A0 = 0.0;
    for (unsigned int i = 0; i < MAX_COUNT_FOR_CALIBRATION; i++) {
      ADCSampleBuff[i] = get_pressure_MPX5010(channel, NULL);
      cal_offset_PRESSURE_A0 += ADCSampleBuff[i];
    }
    cal_offset_PRESSURE_A0 = cal_offset_PRESSURE_A0/ MAX_COUNT_FOR_CALIBRATION;
  }
  else if ( SENSOR_PRESSURE_A1 == s_idx)
  {
    channel = 1; // change these values as per BOM
    cal_offset_PRESSURE_A1=0.0;
    for (unsigned int i = 0; i < MAX_COUNT_FOR_CALIBRATION; i++) {
      ADCSampleBuff[i] = get_pressure_MPX5010(channel, NULL);
      //cal_offset_PRESSURE_A1 = ADCSampleBuff[i] / MAX_COUNT_FOR_CALIBRATION;
      cal_offset_PRESSURE_A1 += ADCSampleBuff[i];
    }
    cal_offset_PRESSURE_A1 = cal_offset_PRESSURE_A1/ MAX_COUNT_FOR_CALIBRATION;
  }
  DebugPort.println(cal_offset_PRESSURE_A0 * 100000);
  DebugPort.println(cal_offset_PRESSURE_A1 * 100000);
}

/**************************************************************
  public funcions
***************************************************************/

/*call this function when pressure sensor needs to be calibrated*/
void calibrate_MPX5010() {
  calculate_zero_offset(SENSOR_PRESSURE_A0);
  calculate_zero_offset(SENSOR_PRESSURE_A1);
}


/*apply the already stored zero cal offset read from eeprom via MCU2*/
void apply_zerocal_offset_MPX5010(sensor_e s_idx, float offset) {
  if ( SENSOR_PRESSURE_A0 == s_idx) {
    cal_offset_PRESSURE_A0 = offset;
  }
  if ( SENSOR_PRESSURE_A1 == s_idx) {
    cal_offset_PRESSURE_A1 = offset;
  }
}

/*get the calculated zero cal offset to supply to MCU2 to write into eeprom*/
float get_zerocal_offset_MPX5010(sensor_e s_idx) {
  if ( SENSOR_PRESSURE_A0 == s_idx) {
    return cal_offset_PRESSURE_A0;
  }
  if ( SENSOR_PRESSURE_A1 == s_idx) {
    return cal_offset_PRESSURE_A1;
  }
  return 0.0;
}

/*function returns pressure with zero calibration applied */
/*places the raw voltage value if raw_voltage supplied is not NULL*/
float get_calibrated_pressure_MPX5010(sensor_e s_idx, float *raw_voltage)
{
  int channel = 0;
  float cal_offset = 0.0;
  float pressure = 0.0;

  if ( SENSOR_PRESSURE_A0 == s_idx) {
    channel = 0; // change these values as per BOM
    cal_offset = cal_offset_PRESSURE_A0;
  }
  else if ( SENSOR_PRESSURE_A1 == s_idx)
  {
    channel = 1; // change these values as per BOM
    cal_offset = cal_offset_PRESSURE_A1;
  }

  pressure = get_pressure_MPX5010(channel, raw_voltage) - cal_offset;

  //  DebugPort.print("s_idx :");
  //  DebugPort.print(s_idx);
  //  DebugPort.print(", cal_offset :");
  //  DebugPort.print(cal_offset);
  //  DebugPort.print(", cal_pressure :");
  //  DebugPort.println(pressure);

  return pressure;
}
