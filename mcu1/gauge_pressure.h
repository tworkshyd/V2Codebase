/**
 * @enum   sensor_e
 * @brief   Identifier for all sensors present in the system
 */
typedef enum {
  SENSOR_PRESSURE_A0 = 0, /**< peak pressure sensor MPX5010 */
  SENSOR_PRESSURE_A1, /**< peep pressure sensor MPX5010 */  
  SENSOR_DP_A0,       /**< differential pressure sensor for Peak pressure MPXV7002DP */  
  SENSOR_DP_A1,       /**< differential pressure sensor for peep pressure MPXV7002DP */
  SENSOR_O2,          /**< oxygen sensor */
  MAX_SENSORS
} sensor_e;

//#ifdef MPX5010
#define MPX5010_VS              (5.0)
#define MPX5010_VFSS            (0.0)
#define MPX5010_ACCURACY        (0.05 * MPX5010_VFSS)
#define MPX5010_ZERO_READING    1 // 0.25 volts corresponds to 0.25/ADS115_MULTIPLIER = 2000 
#define MPX5010_ERROR_THRESHOLD 20


float ADC_ReadVolageATMega(int channel);
float get_pressure_MPX5010(int channel, float *raw_voltage);
float get_calibrated_pressure_MPX5010(sensor_e s_idx, float *raw_voltage);
float get_zerocal_offset_MPX5010(sensor_e s_idx);
void apply_zerocal_offset_MPX5010(sensor_e s_idx, float offset);
void calibrate_MPX5010();
