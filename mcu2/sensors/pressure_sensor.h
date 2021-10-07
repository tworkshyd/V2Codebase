/**************************************************************************/
/*!
    @file     pressure_sensor.h

    @brief 	  Pressure sensor module

    @author   Tworks

	@defgroup PressureSensorModule

    Pressure sensor module - allows clients to get sensor readings
	Sensor data is updated to local data structure in a timer
	in frequent intervals
	The same readings can be read or reset using utility APIs
	@{
*/
/**************************************************************************/
#ifndef __PRESSURE_SENSOR_H__
#define __PRESSURE_SENSOR_H__

#include "../BoardDefines.h"

#include "sensors.h"

#define DEBUG_PRESSURE_SENSOR 0
#define DEBUG_DP_PRESSURE_SENSOR 0
#define DEBUG_DP_PRESSURE_SENSOR_SHORTLOG 0


/*
* Pressure sensors configurations
*/
#define SPYRO_KSYSTEM           110 // Ksystem assumed for spyro
#define FLOWRATE_MIN_THRESHOLD  7
#define CALIBRATION_COUNT       20

// Slope adjustment for DP sensors
// 1. In-hale DP sensor
#define CALIB_SLOPE_TVi		(0.95)
#define CALIB_CONST_TVi		(-65.0)

// 1. Ex-hale DP sensor
#define CALIB_SLOPE_TVe		(0.99)
#define CALIB_CONST_TVe		(5.00)




/**************************************************************************/
/*!
    @brief  Class to handle Pressure sensor, inherits base sensor class
*/
/**************************************************************************/
class pressure_sensor : public sensor {
	protected:
		int m_dp;					/*!< Flag to store whether the object is pressure sensor or differential pressure sensor */
		int m_adc_channel;			/*!< adc channel where the sensor is connected to */
		Adafruit_ADS1115 *m_ads;	/*!< ADS board where the sensor is connected to */
		unsigned long m_lastmpx7002UpdatedTime = 0;
		unsigned long _prev_samplecollection_ts = 0;
		float m_calibrationinpressure = 0.0;
		bool m_calibrated = false;
		float m_lastPressure = 0.0;

	protected:
		/**
		 *   @brief  Utility function to read the differential pressure from MPXV7002 sensor
		 *   @param None
		 *   @return returns the differential pressure read from the sensor as float
		 **/
		float get_pressure_MPXV7002DP(float vout);
		/**
		 *   @brief  Utility function to read volume of air from spyro for specific sensor
		 *   @param None
		 *   @return returns the spyro volume as float
		 **/
		float get_spyro_volume_MPX7002DP(void);
  public:
		/**
		 *   @brief  Constructor for pressure sensors
		 *           Initializes pressure sensor variables
		 **/
      pressure_sensor(Adafruit_ADS1115 *ads, int adc_channel, sensor_e Id) : sensor() {
        m_dp = 0; 
        m_ads = ads;
        m_adc_channel = adc_channel;
        m_sensor_id = Id;
      }

      pressure_sensor(Adafruit_ADS1115 *ads, int adc_channel, sensor_e Id, bool bDiff) : sensor() {
        m_dp = 1; 
        m_ads = ads;
        m_adc_channel = adc_channel;
        m_sensor_id = Id;
      }

       /**
		 *   @brief  Function to initialize the O2 sensor
		 *   @param None
		 *   @return 0 on success and -1 on error
		 **/
		int init(void);

    /**
    *   @brief  Function to reset sensor data
    *   @param None
    *   @return None
    **/
		void reset_sensor_data(void);
        /**
		 *   @brief  Function to read and update sensor data in local data structures (called in timer interrupt)
		 *   @param None
		 *   @return None
		 **/

    float capture_and_read(void);
/**
 *   @brief  Calibrate the pressure sensor
 *   @param  None
 *   @return returns 0 on success and -1 on failure as integer
 **/
    int sensor_zero_calibration(void);
};

#endif /*__PRESSURE_SENSOR_H__*/

/**@}*/
