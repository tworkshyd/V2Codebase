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
  ads1.setGain(GAIN_ONE);

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
int sensorManager::init()
{
  int err = 0;

  VENT_DEBUG_FUNC_START();
  
  err += ads1115_init();
  //err += _pS1.init();
  //err += _pS2.init();
  err += _dpS1.init();
  err += _dpS2.init();
  err += _o2S.init();

  VENT_DEBUG_FUNC_END();
  return err;
}


/*
 * Function to enable specific sensors
 * Takes a parameter of different sensor combinations
 */
void sensorManager::enable_sensor(unsigned int flag) 
{
   Serial.print("");
  VENT_DEBUG_FUNC_START();
  if((_enabled_sensors & DP_A0) && !(flag & DP_A0)) {
    _dpS1.reset_sensor_data();
  }
  if((_enabled_sensors & DP_A1) && !(flag & DP_A1)) {
    _dpS2.reset_sensor_data();
  }
  if((_enabled_sensors & O2) && !(flag & O2)) {
    _o2S.reset_sensor_data();
  }
  _enabled_sensors = flag;
  
  VENT_DEBUG_FUNC_END();
}

unsigned int sensorManager::get_enable_sensors() {
  return _enabled_sensors;
}

static int A0_count = 0;
static int A1_count = 0;
float sensorManager::capture_and_read_data(sensor_e s)
  {
    VENT_DEBUG_FUNC_START();
    float data = 0.0;
    
   // Serial.print("enabled_sensors: "+_enabled_sensors);
    //Serial.print("Sensor index :");
    //Serial.println(s); 
    switch(s) {
      case SENSOR_DP_A0:
        if(sM._enabled_sensors & DP_A0){
      data =  sM._dpS1.capture_and_read();
#if 0
    // Serial.print("Sensor DP_A0 with index :");
    // Serial.println(s);
        A0_count++;
        Serial.print("A0 value:");
        Serial.println(A0_count);
        A1_count=0;
#endif
      }
        break;
      case SENSOR_DP_A1:
        if(sM._enabled_sensors & DP_A1)
      {
        data =  sM._dpS2.capture_and_read();
#if 0
        A1_count++;
        Serial.print("A1 value:");
        Serial.println(A1_count);
        A0_count = 0;
#endif        
      }   // Serial.print("Sensor DP_A1 with index :");
    // Serial.println(s);
        break;
      case SENSOR_O2:
        if(sM._enabled_sensors & O2)
          data =  sM._o2S.capture_and_read(); 
    // Serial.print("Sensor O2 with index :");
    // Serial.println(s);
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


