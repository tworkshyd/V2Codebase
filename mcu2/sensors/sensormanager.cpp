/**************************************************************************/
/*!
    @file     sensors.cpp

    @brief 	  Base class for all sensor types

    @author   Tworks

	@defgroup VentilatorModule

    All sensor interfaces are available in this
	file.
	@{
*/
/**************************************************************************/
#include "../BoardDefines.h"

#include "sensormanager.h"
#include "pressure_sensor.cpp"
#include "O2_sensor.cpp"
#include "ads1115_utils.c"

#define MAX_PS2_SAMPLES 		10
#define THRESHOLD_COMPARE_INDEX 	2
#define DIP_THRESHOLD 			1 //better to be lower than PEEP

sensorManager sM;

/*
 * Function to initialize ads1115 board
 */
int ads1115_init() 
{
  VENT_DEBUG_FUNC_START();
  ads.begin();
  ads.setGain(GAIN_ONE);
  ads1.begin();
  ads1.setGain(OXYGEN_SENSOR_GAIN);

  VENT_DEBUG_INFO ("ADC Init Done", 0);
  VENT_DEBUG_FUNC_END();	
  return 0;
}


int no_of_sensorsenabled(unsigned int n) {
  int count=0;
  while(n!=0){
  if(n & 1){ //if current bit 1
    count++;//increase count
  }
    n=n>>1;//right shift
  }
  return count;
}

/*
 * Function to initialize all modules in sensors
 */
int sensorManager::init(sensor_flags_e flag)
{
  int err = 0;

  VENT_DEBUG_FUNC_START();
  
  err += ads1115_init();

  if ( flag & DP_A0 ) {
	  err += _dpS1.init();
  }
  
  if ( flag & DP_A1 ) {
	  err += _dpS2.init();
  }

  if ( flag & O2 ) {
	  err += _o2S.init();
  }
  
  VENT_DEBUG_FUNC_END();
  
  return err;
}
/*
 * Function to enable specific sensors
 * Takes a parameter of different sensor combinations
 */
void sensorManager::enable_sensor(unsigned int flag) 
{
  VENT_DEBUG_FUNC_START();
  if((flag & DP_A0)) {
    _dpS1.reset_sensor_data();
  }
  if((flag & DP_A1)) {
    _dpS2.reset_sensor_data();
  }
  if((flag & O2)) {
    _o2S.reset_sensor_data();
  }
  _enabled_sensors = flag;
  
  VENT_DEBUG_FUNC_END();
}

unsigned int sensorManager::get_enable_sensors() {
  return _enabled_sensors;
}


float sensorManager::capture_and_read_data(sensor_e s)
  {
    VENT_DEBUG_FUNC_START();
    float data = 0.0;
	
    switch(s) {
      case SENSOR_DP_A0:
        if(_enabled_sensors & DP_A0)	{
			  data =  _dpS1.capture_and_read();
			  DebugPort.print ("computed volume DP_A0 : ");
			  DebugPort.println (data);
		  
			  // y = mx + c
			  data =  data * CALIB_SLOPE_TVi + CALIB_CONST_TVi;
			  DebugPort.print ("Adjusted volume DP_A0 : ");
			  DebugPort.println (data);
		}
        break;
      case SENSOR_DP_A1:
        if(_enabled_sensors & DP_A1)	{
			data =  _dpS2.capture_and_read();
			DebugPort.print ("computed volume DP_A1 : ");
			DebugPort.println (data);
		
			// y = mx + c
			data =  data * CALIB_SLOPE_TVe + CALIB_CONST_TVe;
			DebugPort.print ("Adjusted volume DP_A1 : ");
			DebugPort.println (data);
		}		  
        break;
      case SENSOR_O2:
        if(_enabled_sensors & O2)
         data =  _o2S.capture_and_read();
        break;
      default:
        VENT_DEBUG_ERROR(" ERROR: Invalid Read Request for Sensor", sensorId2String(s));
        VENT_DEBUG_FUNC_END();
        break;

    }

    VENT_DEBUG_FUNC_END();
    // if any error occurs reset to 0 and return
    if (data < 0) data = 0.0;
    return data;
  }

int sensorManager::start_calibration(void) 
{
  sensor *p_sensor = NULL;
  int err = ERROR_UNKNOWN;

  VENT_DEBUG_FUNC_START();

  for (int idx = 0; idx < MAX_SENSORS; idx++) 
  {
    p_sensor = NULL;
    switch(idx) {
      case SENSOR_PRESSURE_A0:
      //  p_sensor = &_pS1;
        break;
      case SENSOR_PRESSURE_A1:
      //  p_sensor = &_pS2;
        break;
      case SENSOR_DP_A0:
        p_sensor = &_dpS1;
        break;
      case SENSOR_DP_A1:
        p_sensor = &_dpS2;
        break;
    }
    if (NULL != p_sensor)
	{
      err = p_sensor->sensor_zero_calibration();
    if ( SUCCESS != err) 
	  {
		 VENT_DEBUG_ERROR ("Calibration Failed for Sensor", err);
      return err;
  }
	}
  }
  
  VENT_DEBUG_FUNC_END();
  return SUCCESS;
}

int sensorManager::read_sensor_rawvoltage(sensor_e s) {
  sensor *p_sensor = NULL;
  int err = 0;

  VENT_DEBUG_FUNC_START();

  switch(s) {
    case SENSOR_PRESSURE_A0:
	 // p_sensor = &_pS1;
	break;
	case SENSOR_PRESSURE_A1:
	 // p_sensor = &_pS2;
	break;
	case SENSOR_DP_A0:
	  p_sensor = &_dpS1;
	break;
	case SENSOR_DP_A1:
	  p_sensor = &_dpS2;
	break;
	case SENSOR_O2:
	  p_sensor = &_o2S;
	break;
	default:
	  VENT_DEBUG_ERROR ("Invalid Read request", s);
	  VENT_DEBUG_FUNC_END();
	  return -1;
	break;
  }
  err = p_sensor->get_error();
  if(err)
  {
	  VENT_DEBUG_ERROR ("Invalid Read request", s);
	  VENT_DEBUG_FUNC_END();
	  return -1;
  }
  
  VENT_DEBUG_FUNC_END();
  return p_sensor->read_rawvoltage();
}

float sensorManager::read_sensor_pressurevalues(sensor_e s) {
  sensor *p_sensor = NULL;
  int err = 0;

  VENT_DEBUG_FUNC_START();

  switch(s) {
    case SENSOR_PRESSURE_A0:
	 // p_sensor = &_pS1;
	break;
	case SENSOR_PRESSURE_A1:
	 // p_sensor = &_pS2;
	break;
	case SENSOR_DP_A0:
	  p_sensor = &_dpS1;
	break;
	case SENSOR_DP_A1:
	  p_sensor = &_dpS2;
	break;
	case SENSOR_O2:
	  p_sensor = &_o2S;
	break;
	default:
	  VENT_DEBUG_ERROR ("Invalid Read request", s);
	  VENT_DEBUG_FUNC_END();
	  return -1;
	break;
  }
  err = p_sensor->get_error();
  if(err) {
	  VENT_DEBUG_ERROR ("Invalid Read request", s);
	  VENT_DEBUG_FUNC_END();
	  return -1;
  }
  
  VENT_DEBUG_FUNC_END();
  return p_sensor->read_sensorpressure();
}


