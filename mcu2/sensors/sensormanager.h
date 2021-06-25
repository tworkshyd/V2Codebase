#pragma once
#include "../BoardDefines.h"

#include "sensors.h"
#include "pressure_sensor.h"
#include "O2_sensor.h"

Adafruit_ADS1115 ads(ADS1015_ADDRESS, ADS115_INT_PIN);
Adafruit_ADS1115 ads1(ADS1015_ADDRESS_1, ADS115_INT_PIN_1);

class sensorManager {
public:
/**************************************************************************/
/*!

    @brief  Function to initialize all modules in sensors

	@param  none

    @return indicates 0 for SUCCESS and -1 for FAILURE
*/
/**************************************************************************/
int init(void);
/**************************************************************************/
/*!

    @brief   Function to enable specific sensors or Takes a parameter of different sensor combinations
 
    @param flag defines which sensor or a combination of sensors to be enabled

    @param source parameter to set the source of input

    @return none
*/
/**************************************************************************/
void enable_sensor(unsigned int flag);

float capture_and_read_data(sensor_e s);

/**************************************************************************/
/*!

    @brief  Function  Checks the previous pressure readings and find if there are any dip in pressure 
                    
    @param paranName paramter is used to specify the 

    @return -1 for invlaid sensors, 0 for no dip 1 for a dip in pressure
 
*/
/**************************************************************************/
int read_sensor_rawvoltage(sensor_e s);
float read_sensor_pressurevalues(sensor_e s);
int start_calibration(void);
unsigned int get_enable_sensors();

private:
  pressure_sensor _dpS1= pressure_sensor(&ads, 0, SENSOR_DP_A0, true);
  pressure_sensor _dpS2 = pressure_sensor(&ads, 1, SENSOR_DP_A1, true);
  o2_sensor _o2S = o2_sensor(&ads1, 0);
  unsigned int _enabled_sensors = 0;
};
