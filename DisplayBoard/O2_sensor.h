/**************************************************************************/
/*!
    @file     O2_sensor.h

    @brief 	  O2 sensor module

    @author   Tworks

	@defgroup OxygenSensorModule

    O2 sensor module - allows clients to read sensor readings
	sensor data is updated to local data structure in a timer
	in frequent intervals
	The same readings can be read or reset using utility APIs
	@{
*/
/**************************************************************************/
#ifndef __O2_SENSOR_H__
#define __O2_SENSOR_H__

//#include "../BoardDefines.h"
#include "BoardDefines.hh"

#include "sensors.h"



#define AVOID_EEPROM             0
#define NUM_OF_SAMPLES_O2       3
#define EEPROM_O2_CALIB_ADDR    0xC

// Default O2 Calibration Values for Envitech O2 Sensor
#define O2_CALIBRATION_VOLTAGE_ACCURACY ( 100 )

#define O2_2_FACTORY_VALUE (11.44 * O2_CALIBRATION_VOLTAGE_ACCURACY )
#define O2_22_FACTORY_VALUE (109.68 * O2_CALIBRATION_VOLTAGE_ACCURACY )
#define O2_100_FACTORY_VALUE (595.88 *O2_CALIBRATION_VOLTAGE_ACCURACY )



/**************************************************************************/
/*!
    @brief  Class to handle oxygen sensor, inherits base sensor class
*/
/**************************************************************************/
class o2_sensor : public sensor {
	protected: /* protected variables */
		float m_slope;		/*!< Stores the slope measured from the sensor voltage at different water level  */
		float m_const;		/*!< Stores the const measured from the sensor voltage at different water level  */
		int m_adc_channel;	/*!< adc channel where the sensor is connected to */
		Adafruit_ADS1115 *m_ads;	/*!< ADS board where the sensor is connected to */
	protected: /* protected functions */
    unsigned long m_lasO2UpdatedTime = 0;
		/**
		 *   @brief  Stores the default calibrated values to memory during boot
		 *   @param  None
		 *   @return returns 0 on success and -1 on failure as integer
		 **/
		//int store_default_o2_calibration_data(void);

	public: /* public functions */
		/**
		 *   @brief  Constructor for O2 sensors
		 *           Initializes O2 sensor variables
		 **/
		o2_sensor(Adafruit_ADS1115 *ads, int adc_channel) : sensor() { m_adc_channel = adc_channel; m_ads = ads; };
       /**
		 *   @brief  Function to initialize the O2 sensor
		 *   @param  None
		 *   @return 0 on success and -1 on error
		 **/
		int init(void);
        /**
		 *   @brief  Function to read sensor data
		 *   @param  None
		 *   @return Returns the readings from sensor
		 **/
		void reset_sensor_data(void);
        /**
		 *   @brief  Function to read and update sensor data in local data structures (called in timer interrupt)
		 *   @param  None
		 *   @return None
		 **/

/**
 *   @brief  reads the value from ADC and coverts to pressure/acc_flow value
 *   @param  None
 *   @return returns sensor value
 **/
    float capture_and_read(void);

    int sensor_zero_calibration(void);

	static int reset_calibration_data(int index);


};

#endif /*__O2_SENSOR_H__*/

/**@}*/
