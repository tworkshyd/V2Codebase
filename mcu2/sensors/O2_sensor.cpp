/**************************************************************************/
/*!
    @file     O2_sensor.cpp

    @brief 	  O2 sensor module

    @author   Tworks

	@defgroup VentilatorModule

    Module to read and measure O2 % from the sensor
	@{
*/
/**************************************************************************/
#include "../BoardDefines.h"

#include "O2_sensor.h"

#define AVOID_EEPROM 			0
#define NUM_OF_SAMPLES_O2		3
#define EEPROM_O2_CALIB_ADDR	0xC

// Default O2 Calibration Values for Envitech O2 Sensor
#define O2_CALIBRATION_VOLTAGE_ACCURACY ( 100 )

#define O2_2_FACTORY_VALUE 			(long int)(11.44 * O2_CALIBRATION_VOLTAGE_ACCURACY )
#define O2_22_FACTORY_VALUE 		(long int)(109.68 * O2_CALIBRATION_VOLTAGE_ACCURACY )
#define O2_100_FACTORY_VALUE 		(long int)(595.88 *O2_CALIBRATION_VOLTAGE_ACCURACY )


//int const x_samples[NUM_OF_SAMPLES_O2] = {20, 216, 1000};
int const x_samples[NUM_OF_SAMPLES_O2] = {50, 216, 960};		// as per the reference gases used .. N2 with 95% purity & O2 with 96% purity

long int yO2VoltX1000[NUM_OF_SAMPLES_O2] = { O2_2_FACTORY_VALUE, O2_22_FACTORY_VALUE, O2_100_FACTORY_VALUE };

//void write_to_eeprom(unsigned int numOfIntWrites, int addr, int val[NUM_OF_SAMPLES_O2]);

/*
 * Function to initialize the O2 sensors
 */
int o2_sensor::init()
 {
	int err = 0;
    m_sensor_id = SENSOR_O2;	
    VENT_DEBUG_FUNC_START();

#if 1 /// code works only if this active ????

    long int calibValue = 0 ;
	for ( int index = 0 ; index < 3 ; index++ ) {
		
		calibValue = retrieve_sensor_data_long( EEPROM_O2_CALIB_ADDR + (index * sizeof(long int) ) );
	}

#endif

//	VENT_DEBUG_INFO("O2 : sensor_zero_calibration", 0 );

	err = sensor_zero_calibration();
	
    VENT_DEBUG_FUNC_END();
	
	return err;
	
}


/*
 * main function to calibrate and get m (slope) and c (constant) after curve fitting
 * read y data from eeprom
 * convert from byte to float
 * use in algo to calc m and c values.
 */
int o2_sensor::sensor_zero_calibration()
{

  float x = 0, sigmaX = 0, sigmaY = 0, sigmaXX = 0, sigmaXY = 0, denominator = 0, y = 0;
  long int value = 0;
  int result;
  result = SUCCESS;
  
  VENT_DEBUG_FUNC_START();

  for (int index = 0; index < NUM_OF_SAMPLES_O2; index++) 
  {

	value = retrieve_sensor_data_long( EEPROM_O2_CALIB_ADDR + (index * sizeof(long int) ) );
    y = ((float)value)/O2_CALIBRATION_VOLTAGE_ACCURACY;

   
    x = (float(x_samples[index]))/10;

	sigmaX += x;
    sigmaY += y;
	
    sigmaXX += x * x;
    sigmaXY += x * y;
  }
	
  denominator = (NUM_OF_SAMPLES_O2 * sigmaXX) - (sigmaX*sigmaX);
  
  if (denominator != 0) {
    m_slope = ((NUM_OF_SAMPLES_O2 * sigmaXY) - (sigmaX*sigmaY)) / denominator;
    m_const = ((sigmaY*sigmaXX) - (sigmaX*sigmaXY)) / denominator;
  	result = SUCCESS;
  } else {
    m_slope = 0;
    m_const = 0;
    result =  ERROR_SENSOR_CALIBRATION;
	VENT_DEBUG_ERROR("Error: O2 calibration failed!!", result);
  }
  
  VENT_DEBUG_INFO("O2 Sensor : m_slope", m_slope);
  VENT_DEBUG_INFO("O2 Sensor : m_const", m_const);
  
  VENT_DEBUG_FUNC_END();
  return result;
}


/*
 * Function to reset the local data structures
 */
void o2_sensor::reset_sensor_data() 
{
	this->m_data.current_data.O2 = this->m_data.previous_data.O2 = 0;
}

float o2_sensor::capture_and_read(void) 
{
  float o2_value = 0.0;
  float vout = 0.0;
  int err = 0;
  
  //VENT_DEBUG_FUNC_START();

 err = ADS1115_ReadVoltageOverI2C(m_ads, m_adc_channel, &vout);  

  if(ERROR_I2C_TIMEOUT == err) {
    VENT_DEBUG_ERROR("O2 Sensor read I2C timeout failure:", m_sensor_id);
    return ERROR_SENSOR_READ;
  } else {
     this->set_error(SUCCESS);
  }
  
  m_raw_voltage = vout*1000;

  o2_value = (m_raw_voltage - m_const) / m_slope ;
  
 
#if DEBUG_PRESSURE_SENSOR
  if ((millis() - m_lasO2UpdatedTime) > SENSOR_DISPLAY_REFRESH_TIME)
  {  
    m_lasO2UpdatedTime = millis();
    DebugPort.print("sensorType->");
    DebugPort.print(sensorId2String(m_sensor_id));
    DebugPort.print("::"); 
    DebugPort.print("C");
    DebugPort.print(" ");
    DebugPort.print(m_adc_channel);
    DebugPort.print(", V");
    DebugPort.print(" ");
    DebugPort.print(vout, 4);
    DebugPort.print(" ");
    DebugPort.print(", O2");
    DebugPort.print(" ");
    DebugPort.print((o2_value), 4);
    DebugPort.print(", raw");
    DebugPort.print(" ");
    DebugPort.println((m_raw_voltage ), 4);
  }
#endif
  this->m_data.current_data.O2 = o2_value ;
  //VENT_DEBUG_FUNC_END();
  return this->m_data.current_data.O2;
}

int o2_sensor::reset_calibration_data(int index) {

	int returnValue = 1;
	
	if ( ( index >= 0 ) && ( index <= 2) ) {
		
	  store_sensor_data_long(EEPROM_O2_CALIB_ADDR + (index * sizeof(long int)), yO2VoltX1000[index]);
      //store_sensor_data_long(EEPROM_CALIBRATION_STORE_ADDR + (m_sensor_id * sizeof(store_param)), store_param);
	  
	  returnValue = 0 ;
	}

	return returnValue ;
}
