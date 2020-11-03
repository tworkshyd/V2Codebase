/**************************************************************************/
/*!
@file     pressure_sensor.cpp

@brief 	  Pressure sensor module

@author   Tworks

@defgroup VentilatorModule

Module to read and measure pressure/flow rate 
from the pressure sensors
@{
*/
/**************************************************************************/

#include "pressure_sensor.h"
#include "ads1115_utils.h"

/*
* Macros to enable the sensor functionalities
*/
#define SENSOR_DISPLAY_REFRESH_TIME 5

/*
* Sensor specific configurations
*/
//#ifdef MPX5010
#define MPX5010_VS              (5.0)
#define MPX5010_VFSS            (0.0)
#define MPX5010_ACCURACY        (0.05 * MPX5010_VFSS)
#define MPX5010_ZERO_READING    2000 // 0.25 volts corresponds to 0.25/ADS115_MULTIPLIER = 2000 
#define MPX5010_ERROR_THRESHOLD 20
//#endif

//#ifdef MPXV7002DP
#define MPXV7002DP_VS           (5.0)
#define MPXV7002DP_VFSS         (4.0)
#define MPXV7002DP_ACCURACY     (0.06250)
#define MPX7002DP_ZERO_READING  22000 // for Vs 5.0 -> 2.75v(@zero pressure)  = 22000 * ADS115_MULTIPLIER 
#define MPX7002DP_ERROR_THRESHOLD 20
//#endif

/*
* Pressure sensors configurations
*/
#define SPYRO_KSYSTEM         110// Ksystem assumed for spyro
#define SPYRO_KSYSTEM_DP0       89// Ksystem assumed for spyro
#define SPYRO_KSYSTEM_DP1       100// Ksystem assumed for spyro

#define FLOWRATE_MIN_THRESHOLD  7
#define CALIBRATION_COUNT       20

String sensorId2String(sensor_e type) {
  switch (type) {
  case SENSOR_PRESSURE_A0:
    return "SENSOR_PRESSURE_A0";
  case SENSOR_PRESSURE_A1:
    return "SENSOR_PRESSURE_A1";
  case SENSOR_DP_A0:
    return "SENSOR_DP_A0  ";
  case SENSOR_DP_A1:
    return "SENSOR_DP_A1  ";      
  case SENSOR_O2:
    return "SENSOR_O2  "; 
  default:
    return "WRONG SENSOR TYPE";
  }        
}

/*
* Initialization routine to setup the sensors
* Calibrate the sensors for errors
*/
int pressure_sensor::init() 
{
  VENT_DEBUG_FUNC_START();
  
  delay(20); //delay of 20ms for sensors to come up and respond
  
  // Initialize the data
  this->m_data.current_data.pressure = 0.0;
  this->m_data.previous_data.pressure = 0.0;
  
  //int needs 2 byes , so index multiplied by 2
  this->m_calibrationinpressure = retrieve_sensor_data_long(EEPROM_CALIBRATION_STORE_ADDR + (m_sensor_id * sizeof(long int)));
  this->m_calibrationinpressure /= SENSOR_DATA_PRECISION;
  
  // EEPROM may be first time reading with 255 or -1 
  if ( 0 == this->m_calibrationinpressure) 
    this->m_calibrationinpressure = 0;

#if DEBUG_PRESSURE_SENSOR
    VENT_DEBUG_INFO("Init ID", m_sensor_id);
    VENT_DEBUG_INFO("Int Pin", m_ads->m_intPin);
    VENT_DEBUG_INFO("I2C Address", m_ads->m_i2cAddress);
    VENT_DEBUG_INFO("ADC Channel", m_adc_channel);
    VENT_DEBUG_INFO("DP", m_dp);
    VENT_DEBUG_INFO("m_calibrationinpressure*SENSOR_DATA_PRECISION", this->m_calibrationinpressure * SENSOR_DATA_PRECISION);
    Serial.print("init :sensorType");
    Serial.println(sensorId2String(m_sensor_id));
    Serial.println(EEPROM_CALIBRATION_STORE_ADDR + m_sensor_id * sizeof(long int), HEX);
    Serial.println(this->m_calibrationinpressure * SENSOR_DATA_PRECISION, HEX);
#endif
  VENT_DEBUG_FUNC_END();
  return 0;
}

/*
* Function to be called to read sensor data and return
* to caller
*/
float pressure_sensor::capture_and_read(void) {
  VENT_DEBUG_FUNC_START();

  if(m_error != 0) return m_error;

  this->m_data.current_data.flowvolume += get_spyro_volume_MPX7002DP();

  return this->m_data.current_data.flowvolume;
}

/*
* Function to reset the local data structures
*/
void pressure_sensor::reset_sensor_data(void)
{
  VENT_DEBUG_FUNC_START();
  _prev_samplecollection_ts = 0;
  
  if(m_dp == 1) 
  {
      this->m_data.current_data.flowvolume = 0;
  } 
  VENT_DEBUG_FUNC_END();
}

int pressure_sensor::sensor_zero_calibration() 
{
  int err = 0;
  float vout = 0.0;
  float pressure = 0.0;
  
  VENT_DEBUG_FUNC_START();
  
  for(int index = 0; index < CALIBRATION_COUNT; index++) 
  {
      err = ADS1115_ReadVoltageOverI2C(m_ads, m_adc_channel, &vout);
	  if(ERROR_I2C_TIMEOUT == err) 
	  {
      VENT_DEBUG_ERROR("Sensor read I2C timeout failure:", m_sensor_id);
      this->set_error(ERROR_SENSOR_READ);
      return -1;
    } else {
      this->set_error(SUCCESS);
    }

  if(m_dp)
      pressure += get_pressure_MPXV7002DP(vout);
   }

  m_calibrationinpressure = pressure/CALIBRATION_COUNT;
  
  VENT_DEBUG_ERROR("sensorType", sensorId2String(m_sensor_id));
  VENT_DEBUG_ERROR("Correction in Pressure by", m_calibrationinpressure);

  long int store_param = (long int)(m_calibrationinpressure * SENSOR_DATA_PRECISION);
  //eeprom needs 2 bytes , so *2 is added
  store_sensor_data_long(EEPROM_CALIBRATION_STORE_ADDR + (m_sensor_id * sizeof(store_param)), store_param);
  VENT_DEBUG_INFO("Store Param", store_param);

//#if DEBUG_PRESSURE_SENSOR
  Serial.print("store :sensorType");
  Serial.println(sensorId2String(m_sensor_id));
  Serial.println(EEPROM_CALIBRATION_STORE_ADDR+m_sensor_id*4, HEX);
  Serial.println(this->m_calibrationinpressure*SENSOR_DATA_PRECISION, HEX);
//#endif
  VENT_DEBUG_FUNC_END();
  return 0;
}

/*
* Function to get the flow rate of spyro
*/
float pressure_sensor::get_spyro_volume_MPX7002DP() {
  float vout = 0.0;
  float pressure = 0.0;
  float flowrate = 0.0, accflow = 0.0;
  int err = 0;
  unsigned long present_ts = 0;
  unsigned long accumlated_time = 0;
  VENT_DEBUG_FUNC_START();
  
  if ( 0 == _prev_samplecollection_ts) {
    _prev_samplecollection_ts = millis();
  } else {
  err = ADS1115_ReadVoltageOverI2C(m_ads, m_adc_channel, &vout);
  if(ERROR_I2C_TIMEOUT == err) 
  {
    VENT_DEBUG_ERROR("Sensor read I2C timeout failure:", m_sensor_id);
    this->set_error(ERROR_SENSOR_READ);
    return -1;
  } else {
     this->set_error(SUCCESS);
  }
  m_raw_voltage = vout * 1000;
  pressure = get_pressure_MPXV7002DP(vout);
  //add the correction done with calibration
  pressure = pressure-m_calibrationinpressure;
  m_value = pressure;
  if(m_sensor_id == SENSOR_DP_A0){
    flowrate = SPYRO_KSYSTEM_DP0 * sqrt(abs(pressure));
  }
  if(m_sensor_id == SENSOR_DP_A1){
    flowrate = SPYRO_KSYSTEM_DP1 * sqrt(abs(pressure));
  }
  //flowrate = SPYRO_KSYSTEM * sqrt(abs(pressure));
  if(pressure > 0)
    flowrate = -1*flowrate;
  present_ts = millis();

#if DEBUG_DP_PRESSURE_SENSOR_SHORTLOG  
  Serial.print("present_ts:");
  Serial.print(present_ts);  
  Serial.print(", _prev_samplecollection_ts:");
  Serial.print(_prev_samplecollection_ts);  
#endif

  accumlated_time = (present_ts - _prev_samplecollection_ts);
    if(flowrate > FLOWRATE_MIN_THRESHOLD) {
      //accflow = (accumlated_time*flowrate*1000)/60000;
      accflow = (accumlated_time*flowrate)/60;
    }
    _prev_samplecollection_ts = present_ts;


#if DEBUG_DP_PRESSURE_SENSOR
      Serial.print(" sensorType->");
      Serial.print(sensorId2String(m_sensor_id));
      Serial.print("::");
      Serial.print("C");
      Serial.print(" ");
      Serial.print(m_adc_channel);
      Serial.print(", V*1000");
      Serial.print(" ");
      Serial.print(vout * 1000, 6);
      Serial.print(", P");
      Serial.print(" ");
      Serial.print(pressure, 6);
      Serial.print(", Cal");
      Serial.print(" ");
      Serial.print(m_calibrationinpressure, 6);
      Serial.print(", F");
      Serial.print(" ");
      Serial.print(flowrate, 6);
      Serial.print(", AF");
      Serial.print(" ");
      Serial.print(accflow, 6);
      Serial.print(", acc_time  ");
      Serial.print(accumlated_time);
      if(m_dp == 1) {
        Serial.print(", Total AF");
        Serial.print(" ");
        Serial.println(this->m_data.current_data.flowvolume, 6);
      }
#elif DEBUG_DP_PRESSURE_SENSOR_SHORTLOG
      Serial.print(" sensorType->");
      Serial.print(sensorId2String(m_sensor_id));
      Serial.print(", acc_time  ");
      Serial.print(accumlated_time);        
      Serial.print(", Total AF");
      Serial.print(" ");
      Serial.println(this->m_data.current_data.flowvolume, 6);
#endif
  }
  VENT_DEBUG_FUNC_END();
  return accflow;
}

/*
* Vout = Vs (0.2P + 0.5) +- accuracy%VFSS
* P = (Vout - accuracy*VFSS/100 - (Vs * 0.5))/(0.2 * Vs)
*/
float pressure_sensor::get_pressure_MPXV7002DP(float vout) {
  float pressure = 0.0;
  float correction = (MPXV7002DP_ACCURACY * MPXV7002DP_VFSS);
  pressure = (vout - correction - (MPXV7002DP_VS * 0.5))/(0.2 * MPXV7002DP_VS);
  //pressure = (vout + correction - (MPXV7002DP_VS * 0.5))/(0.2 * MPXV7002DP_VS);
  pressure = ((m_lastPressure * 0.3) + (pressure * 0.7));
  m_lastPressure = pressure;
  return pressure;
}
