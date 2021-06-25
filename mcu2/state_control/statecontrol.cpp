#include "../BoardDefines.h"

#include "statecontrol.h"
#include "../sensors/sensors.h"
#include "../debug.h"

#define SENSOR_DATA_PRECISION 100000
#define SENSOR_OFFSET_APPLY_TH 50.0
//#define SPYRO_KSYSTEM_OFFSET_DP0  0.0   // Ksystem assumed for spyro
#define SPYRO_KSYSTEM_OFFSET_DP1 0 // Ksystem assumed for spyro

bool bSendInitCommand = false;
bool minPressureForMaskOn = false;
bool bSendPeakHighDetected = false;
bool bSendPeepLowDetected = false;
bool bBreathDetectedFlag = false;

// Control statemachine gloabl variable
ControlStatesDef_T geCtrlState = CTRL_DO_NOTHING;
ControlStatesDef_T geCtrlPrevState = CTRL_DO_NOTHING;

/*!< Commands array contains static command values to be sent to ventilator master */

static const String commands[] =
    {
        "$VMST0000&", /**< stop stepper motor 0*/
        "$VMST0001&", /**< start stepper motor*/
        "$VMO20100&", /**< oxygen solenoid cylinder line off*/
        "$VMO20101&", /**< oxygen solenoid cylinder line on 3*/
        "$VMO20200&", /**< oxygen solenoid hospital line off*/
        "$VMO20201&", /**< oxygen solenoid hospital line on 5*/
        "$VMSV0100&", /**< inhalation  solenoid off*/
        "$VMSV0101&", /**< inhalation  solenoid on*/
        "$VMSV0200&", /**< exhalation  solenoid off*/
        "$VMSV0201&", /**< exhalation  solenoid on*/
        "$VMSV0300&", /**< peak pressure relief off*/
        "$VMSV0301&", /**< peak pressure relief on*/
        "$VMP1xxxx&", /**< set tidal volume */
        "$VMP2xxxx&", /**< set BPM */
        "$VMP3xxxx&", /**< set peak pressure*/
        "$VMP4xxxx&", /**< set Fio2*/
        "$VMP5xxxx&", /**< set inhalation exhalation ratio */
        "$VMPP0000&", /**< slave entering parameter edit */
        "$VMPP1111&", /**< slave completed parameter edit */
        "$VMIN0000&", /**< initialize master*/
        "$VMIN0001&", /**< initialize stepper module*/
        "$VMIN0002&", /**< initialize stepper module*/
        "$VMIN0003&"  /**< initialize breath detection*/
                      //"$VS0199990000&", //CM-compression cycle begin, (peep,plat)--Hex values
                      //"$VS0399990000&" /EX-Expansion Cycle begin,(pip)--Hex Values
};

String serial2_rxdata = "";

static String p1;
static String p2;

static String payload;
static String payload2;



int peepErr = 0;
int tviErr = 0;
int tveErr = 0;
int pipErr = 0;
float pmax = 0;
float plat = 0;
bool refreshfullscreen_inhale = true;
bool refreshfullscreen_exhale = true;
unsigned long exhale_refresh_timeout = 0;
//displayManager dM;
/* P1 and P2 are connected to relay board
   get the values via serial interface 
*/
static float sensor_pressure[2] = {0.0, 0.0};
static int sensor_pressure_mv[2] = {0, 0};

bool bvmFailure = false;
unsigned long int breathCount = 0;


void initCtrlStateControl(void) {

	VENT_DEBUG_INFO("Uart Receive Data buffers init : Done  ", 0 );

    serial2_rxdata.reserve(30);

	p1.reserve(20);
    p2.reserve(20);
	
	payload.reserve(20);
	payload2.reserve(20);
	
}

int Ctrl_send_packet(int cmdIndex)
{
  Serial3.print(commands[cmdIndex]);
  return 0;
}

int Ctrl_send_packet(String name, long value)
{
  VENT_DEBUG_FUNC_START();
  //
  String param = "";
  String command = "";
  if (name == tidl_volu.parm_name)
  {
    param = TV_PARAM;
  }
  else if (name == inex_rati.parm_name)
  {
    param = IER_PARAM;
  }
  else if (name == resp_rate.parm_name)
  {
    param = RR_PARAM;
  }
  else if (name == peak_press.parm_name)
  {
    param = PEAK_PARAM;
  }
  else if (name == cal_gp1.parm_name)
  {
    param = GP0_PARAM;
  }
  else if (name == cal_gp2.parm_name)
  {
    param = GP1_PARAM;
  }
  else
  {
    VENT_DEBUG_ERROR("ERROR: Trying to send invalid packet", -1);
    return -1;
  }
  command = Ctrl_CreateCommand(param, value);
  Serial3.print(command);
  DebugPort.print("Sending Packet:");
  DebugPort.println(command);
  VENT_DEBUG_INFO("Command", command);

  VENT_DEBUG_FUNC_END();
  return 0;
}

void Ctrl_store_received_packet(String data)
{
  serial2_rxdata = data;
#ifdef DEBUG_RECEIVED_DATA 
  DebugPort.println();
  DebugPort.print("serial2_rxdata : ");
  DebugPort.println(serial2_rxdata);
#endif  
}

void Ctrl_ProcessRxData(displayManager &dM)
{

  String command;
  long int state;

  //displayManager dM;
#ifdef DEBUG_RECEIVED_DATA  
    DebugPort.println();
	DebugPort.print("serial2_rxdata begin of fn process rx data: ");
	DebugPort.println(serial2_rxdata);
#endif  

  p1 = serial2_rxdata.substring(1, 3);
  p2 = serial2_rxdata.substring(3, 5);
  payload = serial2_rxdata.substring(5, 9);
  int value;
  if (p1 == VENTSLAVE)
  {
    if (p2 == SYNCH)
    {
      // DebugPort.print("MachineFlag::");
      // DebugPort.println(payload.toInt());
  
      if (payload.toInt() == 0)
      {
              machineOn = false;
			  digitalWrite(LED_6_PIN, LOW);
      }
    }
    else if (p2 == COMP)
    {

	  machineOn = true;
	  digitalWrite(LED_6_PIN, HIGH);
	  
#ifdef DEBUG_RECEIVED_DATA  
	  DebugPort.println();
	  DebugPort.print("serial2_rxdata, process rx data: p2=COMP :  ");
	  DebugPort.println(serial2_rxdata);
	  DebugPort.print("Payload : ");
	  DebugPort.print(payload);
#endif  
      state = p2.toInt();
      payload2 = serial2_rxdata.substring(9, 13);

#ifdef DEBUG_RECEIVED_DATA 

      DebugPort.print("Payload2 : ");
	  DebugPort.print(payload2);
	  
      DebugPort.print("PEEP: ");
      DebugPort.println((payload.toFloat() / 10));
      DebugPort.print("PLAT: ");
      DebugPort.println((payload2.toFloat() / 10));
	  
     // DebugPort.print("STATE: ");
     // DebugPort.println(ControlStatesDef_T(state));
#endif

      dM.setDisplayParam(DISPLAY_PEEP, (payload.toFloat() / 10));
      dM.setDisplayParam(DISPLAY_PLAT, (payload2.toFloat() / 10));

	  if ((ControlStatesDef_T(state)) >= CTRL_UNKNOWN_STATE)
      {
        VENT_DEBUG_ERROR("Payload with Incorrect State", state);
      }
      else
      {
        geCtrlState = ControlStatesDef_T(state);
      }
    }
    else if (p2 == EXPAN)
    {

	
#ifdef DEBUG_RECEIVED_DATA  
		
		DebugPort.println();
		DebugPort.print("serial2_rxdata, fn process rx data: p2=EXPAN :  ");
		DebugPort.println(serial2_rxdata);
        // payload = serial2_rxdata.substring(5, 10); // this is for PIP to accept5 digit value
        DebugPort.print("PIP: ");
        DebugPort.println((payload.toFloat() / 10));

#endif  
      state = p2.toInt();

      dM.setDisplayParam(DISPLAY_PIP, (payload.toFloat() / 10));
      if ((ControlStatesDef_T(state)) >= CTRL_UNKNOWN_STATE)
      {
        VENT_DEBUG_ERROR("Payload with Incorrect State", state);
      }
      else
      {
        geCtrlState = ControlStatesDef_T(state);
      }
    }
    else if (p2 == "O2")
    {
      if (0 == params[E_O2_INPUT].value_curr_mem)
      {
        Ctrl_send_packet(OXY_SOLE_CYL_ONN);
      }
      else
      {
        Ctrl_send_packet(OXY_SOLE_HOS_O2_ONN);
      }
    }
    else if (p2 == GP0_PARAM)
    {
      DebugPort.print("P7_Calibration for GP0 :");
      DebugPort.println(serial2_rxdata);
      persist_write_calvalue(SENSOR_PRESSURE_A0, (serial2_rxdata.substring(5, 13).toFloat() / SENSOR_DATA_PRECISION));
    }
    else if (p2 == GP1_PARAM)
    {
      DebugPort.print("P8_Calibration for GP1 :");
      DebugPort.println(serial2_rxdata);
      persist_write_calvalue(SENSOR_PRESSURE_A1, (serial2_rxdata.substring(5, 13).toFloat() / SENSOR_DATA_PRECISION));
    }
    else if (p2 == PARAMGP_RAW)
    {
      //DebugPort.print("G1: ");
      //DebugPort.println(serial2_rxdata);

      payload2 = serial2_rxdata.substring(9, 13);
      sensor_pressure_mv[0] = payload.toInt();
      sensor_pressure_mv[1] = payload2.toInt();
    }
    else if (p2 == PARAMGP_PRS)
    {
      DebugPort.print("G2: ");
      DebugPort.println(serial2_rxdata);
      payload2 = serial2_rxdata.substring(9, 13);
      sensor_pressure[0] = (payload.toFloat() / 100);
      sensor_pressure[1] = (payload2.toFloat() / 100);
    }
    else
    {
      int index;
      index = payload.toInt();
      if (index < MAX_CTRL_PARAMS)
      {
        value = params[index].value_curr_mem;
        command = Ctrl_CreateCommand(p2, value);
        Serial3.print(command);
      //  DebugPort.println(command);
      }
    }
  }
}

/*
   Function to build the command to be sent to Ventilator Master
*/
String Ctrl_CreateCommand(String paramName, long value)
{
  String command;
  char paddedValue[3];
  command = START_DELIM;
  command += VENT_MAST;
  command += paramName;
  if (paramName == GP0_PARAM || paramName == GP1_PARAM)
  {
    char paddedValue3[15];
    sprintf(paddedValue3, "%08lu", value);
    command += paddedValue3;
    // DebugPort.print("cal value sending :  ");
    // DebugPort.print(paramName);
    // DebugPort.print(" == ");
    // DebugPort.println(value);
    // DebugPort.print(" == ");
    // DebugPort.println(paddedValue3);
  }
  else
  {
    sprintf(paddedValue, "%04d", value);
    command += paddedValue;
  }
  command += END_DELIM;
  return command;
}

void Ctrl_Start()
{
  geCtrlState = CTRL_INIT;
}

void Ctrl_Stop()
{
  geCtrlState = CTRL_STOP;
}
void Ctrl_StateMachine_Manager(const float *sensor_data, sensorManager &sM, displayManager &dM)
{
  VENT_DEBUG_FUNC_START();
  switch (geCtrlState)
  {
  case CTRL_INIT:
  {
    Serial3.print(commands[INIT_MASTER]);
    DebugPort.println(commands[INIT_MASTER]);

    geCtrlState = CTRL_DO_NOTHING;
  }
  break;
  case CTRL_EXPANSION_HOLD:
  case CTRL_COMPRESSION_HOLD:
  case CTRL_UNKNOWN_STATE:
  case CTRL_DO_NOTHING: // Always enter this during idle scenarios.
    break;
  case CTRL_COMPRESSION:
  {
    if (geCtrlPrevState != geCtrlState)
    {
      peepErr = 0;
      pipErr = 0;
      tveErr = 0;
     // DebugPort.print("TVe");
      dM.setDisplayParam(DISPLAY_TVE, sensor_data[SENSOR_DP_A1]);
      if ((sensor_data[SENSOR_DP_A1] < params[E_TV].value_curr_mem * 0.85))
      {
        tveErr = -1;
      }
      if ((sensor_data[SENSOR_DP_A1] > params[E_TV].value_curr_mem * 1.15))
      {
        tveErr = 1;
      }
     // DebugPort.println(sensor_data[SENSOR_DP_A1]);
      sM.enable_sensor(DP_A0 |O2 );
      refreshfullscreen_inhale = true;
      //  _refreshRunTimeDisplay = true;
    }
  }
  break;
  case CTRL_EXPANSION:
  {
    
    

    if (geCtrlPrevState != geCtrlState)
    {
      VENT_DEBUG_INFO("SC :EX ", sensor_data[SENSOR_DP_A1]);
      tviErr = 0;
     // DebugPort.print("TVi");
     // DebugPort.println(sensor_data[SENSOR_DP_A0]);
      // dM.setDisplayParam(DISPLAY_TVI,20.0);
      dM.setDisplayParam(DISPLAY_TVI, sensor_data[SENSOR_DP_A0]);
      if (sensor_data[SENSOR_DP_A0] * 1.085 < 100)
      {
        bvmFailure = true;
      }
      if ((sensor_data[SENSOR_DP_A0] < params[E_TV].value_curr_mem * 0.85))
      {
        tviErr = -1;
      }
      if ((sensor_data[SENSOR_DP_A0] > params[E_TV].value_curr_mem * 1.15))
      {
        tviErr = 1;
      }
      sM.enable_sensor(DP_A1 | O2);
      breathCount++;
      refreshfullscreen_exhale = true;
      //    _refreshRunTimeDisplay = true;
      exhale_refresh_timeout = millis() + 500;
    }
  }
  break;
  case CTRL_STOP:
  {
    Serial3.print(commands[STPR_STP]);
    geCtrlState = CTRL_DO_NOTHING;
    peepErr = 0;
    tviErr = 0;
    tveErr = 0;
    pipErr = 0;
  }
  break;
  default:
    VENT_DEBUG_ERROR("ERROR: Unknown state received in State machine", 0);
    break;
  }
  if (geCtrlPrevState != geCtrlState)
  {
    geCtrlPrevState = geCtrlState;
    bSendPeakHighDetected = false;
    bSendPeepLowDetected = false;
    bBreathDetectedFlag = false;
    if (geCtrlState == CTRL_COMPRESSION)
    {
      minPressureForMaskOn = false;
    }
  }
  VENT_DEBUG_FUNC_END();
}

void persist_write_calvalue(sensor_e s, float val)
{
  if (s <= SENSOR_PRESSURE_A1)
  {
    long int store_param = (long int)(val * SENSOR_DATA_PRECISION);
    DebugPort.print("Stored value :");
    DebugPort.println(store_param);
    //eeprom needs 2 bytes , so *2 is added
    store_sensor_data_long(EEPROM_CALIBRATION_STORE_ADDR + (s * sizeof(store_param)), store_param);
    VENT_DEBUG_INFO("Store Param", store_param);
    DebugPort.print("init :sensorType ");
    DebugPort.println(s);
    DebugPort.println(EEPROM_CALIBRATION_STORE_ADDR + s * 4, HEX);
    DebugPort.println(val * SENSOR_DATA_PRECISION, HEX);
    //p1,p2,calvalue total 4 packets will be received
  }
  else
    return -1;
}

float send_calvalue(sensor_e s) //send_calvalue functionName
{
  float val = 0.0;
  if (s <= SENSOR_PRESSURE_A1)
  {
    //int needs 2 byes , so index multiplied by 2
    val = retrieve_sensor_data_long(EEPROM_CALIBRATION_STORE_ADDR + (s * sizeof(long int)));
    // val /= SENSOR_DATA_PRECISION;
    DebugPort.print("Retrived Value :");
    DebugPort.println(val);
    VENT_DEBUG_INFO("m_calibrationinpressure*SENSOR_DATA_PRECISION", val);
    DebugPort.print("init :sensorType ");
    DebugPort.println(s);
    DebugPort.println(EEPROM_CALIBRATION_STORE_ADDR + s * sizeof(long int), DEC);
    DebugPort.println(val * SENSOR_DATA_PRECISION, HEX);

    if (s == SENSOR_PRESSURE_A0)
    {
      Ctrl_send_packet("Cal_GP1", (long)val); //P1
    }
    if (s == SENSOR_PRESSURE_A1)
    {
      Ctrl_send_packet("Cal_GP2", (long)val); //P2
    }

    //"$VSF30000&" send p1
    //$VSF40000& send p2
    return val;
  }
  else
    return -1;
}

// "$VSF10000&" //to disable the flag to stop data transfer
//2 "$VSF10001&" // to enable  the flag  to start data transfer  start the calibration
//3 "$VSF20000&" //to disable p1, p2 data  transfer pressure
//1 "$VSF20001&" // to enable p1, p2 data transfer pressure
//3 "$VSF30000&" //to disable p1, p2 data  transfer voltage
//1 "$VSF30001&" // to enable p1, p2 data transfer  voltage
// "$VSF40000&" // to send eprom read calibration data to relay module
void mcu0_calibrate_sensor()
{
  /* 2 send commands for calibration*/
  Serial3.print("$VMF10001&");
}

void mcu0_enable_sensors_pressure(bool en)
{
  /*send the command for pressure sensor to enable or disable*/
  if (true == en)
  {
    /* 1send enable command */
    Serial3.print("$VMF20001&");
  }
  else
  {
    /* 3 send disable command */
    Serial3.print("$VMF20000&");
  }
}
void mcu0_enable_sensors_voltage(bool en)
{
  /*send the command for pressure sensor to enable or disable*/
  if (true == en)
  {
    /* 1send enable command */
    Serial3.print("$VMF30001&");
  }
  else
  {
    /* 3 send disable command */
    Serial3.print("$VMF30000&");
  }
}

float mcu0_read_pressure(sensor_e s) //$VMP50000$ p1,p2 pressure
{
  if (s <= SENSOR_PRESSURE_A1)
  {
    return sensor_pressure[s];
  }
  else
    return -1;
}

int mcu0_read_rawvoltage(sensor_e s) //$VMP60000& p1,p2 mv
{
  if (s <= SENSOR_PRESSURE_A1)
  {
    return sensor_pressure_mv[s];
  }
  else
    return -1;
}
