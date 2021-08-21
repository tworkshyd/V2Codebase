
/**************************************************************************/
/*!
    @file     ctrl_display.h

    @brief     This Module contains 

    @author   Tworks
     @{
*/
/**************************************************************************/

#pragma once
#include "../BoardDefines.h"

#include "lcd.h"
#include "../encoder/encoder.h"
#include "../sensors/sensormanager.h"

#define MAIN_MENU_LENGTH 4
#define SUB_MENU1_LENGTH 3

boolean drawSplashScreenMenu = false;
boolean _refreshSplashEditScreen = true;

static int serviceLevelIndex = -1;
static int statusScreenIndex = -2;
#define SPLASH_EDIT_MENU_TIMEOUT 7000

void CurrentO2Value(int i);
void ResetO2(int i);
void CalibrateO2(int i);
void CalibExit(int i);
void Adc();
void Com();
void Sensor();
void Valves();
void ExitDaignostic();

// // fun_ptr_arr is an array of function pointers
void (*oxygenCalibFunc_arr[4])(int) = {CurrentO2Value, ResetO2, CalibrateO2, CalibExit};
void (*diagnosticFunc_arr[5])() = {Adc, Com, Sensor, Valves, ExitDaignostic};
const char *diagnosticFuncName[5] = {"  adc ", "  com ", "sensor", "valves", " Exit "};

void drawServiceLevelScreen(int screenIndex, RT_Events_T eRTState);

const char *oxySettings[4] = {"", "  Reset  ", "Calibrate", "   Exit  "}; //pot2
const char *mainMenu[MAIN_MENU_LENGTH] = {" exit diag mode", " O2-Calib", " Check ADS1115", " Read All"};
const char *subMenu1[SUB_MENU1_LENGTH] = {" 0% ", " 21%", "100%"}; //pot1

void diagO2Sensor(void);
void diagAds1115(void);
void sensorstatus(void);
void diagSolStatus(void);


#define TV_DEFAULT_VALUE 350
#define RR_DEFAULT_VALUE 12
#define PMAX_DEFAULT_VALUE 60
#define IER_DEFAULT_VALUE 2

#define DBNC_INTVL_SW 500 /*!< millisecs delay before switch debounce  */
#define POT_HIGH 1023     /*!< maximum pot high value  */

/*Let us start writing from the 16th memory location.
   This means Parameter 1 will be stored in locs with addr 16, 17, 18, 19
   and Parameter 1 will be stored in 20, 21, 22, 23 and so on
*/


#define UNIT_CMH2O "cmH20"

#define SAVE_FLAG " SAVE "              /*!< defines the String for SAVE_FLAG   */
#define SAVE_FLAG_CHOSEN "<SAVE>"       /*!< defines the String for SAVE_FLAG_CHOSEN  */
#define CANC_FLAG " CANCEL "            /*!< defines the String for CANC_FLAG  */
#define CANC_FLAG_CHOSEN "<CANCEL> "    /*!< defines the String for CANC_FLAG_CHOSEN  */

#define CAL_GP1 "Cal_GP1"
#define CAL_GP2 "Cal_GP2"
/*!< mode_headers array difines all the modes */

const unsigned char splashScreenTopBottomBuffer[21] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0};

const unsigned char splashScreenMiddleBuffer[21] = {
    0xFF, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0xFF, 0x0};

typedef enum
{
  E_EXIT = 0,
  E_TV,
  E_BPM,
  E_FiO2,
  E_IER,
  E_PEEP,
  E_PIP,
  E_O2_INPUT,
  E_OP_MODE,
  SHOW_VOL,
  SHOW_PRESSURE,
  E_CALVALUE_GP1,
  E_CALVALUE_GP2,
  LCD_CONTRAST,
  OXYGEN_CALIB,
  MAX_EDIT_MENU_ITEMS = 15
} eMainMenu;

#define MAX_CTRL_PARAMS 9 /*!< Total number of control parameters  */

const String mainEditMenu[MAX_EDIT_MENU_ITEMS] = {"EXIT EDIT MENU", "TV   : ", "RR   : ", "FiO2 : ", "IER  : ", "PEEP : ", "PIP  : ", "O2in : ", "OpMode: ", "Volt : ", "Pres : "};
struct ctrl_parameter_t
{
  const eMainMenu index;  /*!< index variable stores the index according to the ctrl_parameter_t params array variables*/
  const String parm_name; /*!< It describes the control parameter name */
  const int readPortNum;  /*!< It describes the pin number associated with control parameter*/
  const int min_val;      /*!< It describes the minimum value associated with control parameter */
  const int max_val;      /*!< It describes the maximum value associated with control parameter */
  const String units;     /*!< It describes the units associated with control parameter */
  int incr;               /*!< It describes the incrementation factor associated with control parameter */
  int value_curr_mem;     /*!< It describes the current memory value associated with control parameter */
  int value_new_pot;      /*!< It describes the current pot value associated with control parameter */
};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for exit handler  */
const ctrl_parameter_t exit_menu = {E_EXIT, mainEditMenu[E_EXIT], 0,
                                    0, 0,
                                    "", 0,
                                    0, 0};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for tidal volume  */
const ctrl_parameter_t tidl_volu = {E_TV, mainEditMenu[E_TV], TIDAL_VOLUME_PIN,
                                    200, 650,
                                    "ml   ", 50,
                                    350, 0};
/*!< default values assigned according to the ctrl_parameter_t Structure variables for BPM */
const ctrl_parameter_t resp_rate = {E_BPM, mainEditMenu[E_BPM], RR_PIN,
                                    4, 39,
                                    "BPM  ", 1,
                                    10, 0};
/*!< default values assigned according to the ctrl_parameter_t Structure variables for Fio2 */
const ctrl_parameter_t fio2_perc = {E_FiO2, mainEditMenu[E_FiO2], FiO2_PIN,
                                    14, 99,
                                    "%    ", 1,
                                    0, 0};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for IER Ratio */
//DISP_ENC_CLK // //READ THROUGH ENCODER
const ctrl_parameter_t inex_rati = {E_IER, mainEditMenu[E_IER], IER_PIN,
                                    0, 2,
                                    "ratio", 1,
                                    2, 0};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for PeeP */
const ctrl_parameter_t peep_pres = {E_PEEP, mainEditMenu[E_PEEP], DISP_ENC_CLK, //READ THROUGH ENCODER
                                    0, 20,
                                    UNIT_CMH2O, 1,
                                    0, 0};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for PeakPressure */
const ctrl_parameter_t peak_press = {E_PIP, mainEditMenu[E_PIP], PMAX_PIN,
                                     29, 99,
                                     UNIT_CMH2O, 1,
                                     0, 0};
/*!< default values assigned according to the ctrl_parameter_t Structure variables for o2_input */
const ctrl_parameter_t o2_input = {E_O2_INPUT, mainEditMenu[E_O2_INPUT], 0,
                                   0, 0,
                                   "", 0,
                                   0, 0};

/*!< default values assigned according to the ctrl_parameter_t Structure variables for o2_input */
const ctrl_parameter_t op_mode = {E_OP_MODE, mainEditMenu[E_OP_MODE], 0,
                                  0, 0,
                                  "", 0,
                                  0, 0};

const ctrl_parameter_t show_voltage = {SHOW_VOL, mainEditMenu[SHOW_VOL], 0,
                                       0, 0,
                                       "", 0,
                                       0, 0};

const ctrl_parameter_t show_pressure = {SHOW_PRESSURE, mainEditMenu[SHOW_PRESSURE], 0,
                                        0, 0,
                                        "", 0,
                                        0, 0};

const ctrl_parameter_t cal_gp1 = {E_CALVALUE_GP1, CAL_GP1, 0,
                                  0, 1024,
                                  "", 0,
                                  0, 0};

const ctrl_parameter_t cal_gp2 = {E_CALVALUE_GP2, CAL_GP2, 0,
                                  0, 1024,
                                  "", 0,
                                  0, 0};
const ctrl_parameter_t lcd_contrast = {LCD_CONTRAST, "LCD Contrast", FiO2_PIN,
                                       -10, 90,
                                       "", 10,
                                       0, 0};

const ctrl_parameter_t oxygenCalib = {OXYGEN_CALIB, " O2 Calibration", RR_PIN,
                                      -10, 90,
                                      "", 10,
                                      0, 0};
/*!< Array contains all the control parameter values  */
/*order should be same as in eMainMenu*/
static ctrl_parameter_t params[] = {exit_menu, tidl_volu, resp_rate, fio2_perc, inex_rati, peep_pres,
                                    peak_press, o2_input, op_mode, show_voltage, show_pressure, cal_gp1, cal_gp2, lcd_contrast, oxygenCalib};

// global variables here
enum STATE
{
  STATUS_MENU,
  STATUS_MENU_TO_EDIT_MENU,
  EDIT_MENU,
  EDIT_MENU_TO_SUB_EDIT_MENU,
  SUB_EDIT_MENU,
  SUB_EDIT_MENU_TO_EDIT_MENU,
  EDIT_MENU_TO_STATUS_MENU,
};

enum eDisplayPrm
{
  DISPLAY_TVI,
  DISPLAY_TVE,
  DISPLAY_PEEP,
  DISPLAY_PIP,
  DISPLAY_PLAT
};
const char clearScreenBuffer[21] = {
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x0};

const char topBottomLineBuffer[18] = {
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x00};


static int aboutScreenIndex = 0;

boolean errorStatus = false;
boolean pressure_flag = false;
boolean millivolt_flag = false;

void SetDefaultAllParam();
void SetBasicParam();
void SetDefaultCalibration();
void exitFactory();
// fun_ptr_arr is an array of function pointers
void (*fun_ptr_arr[])() = {SetDefaultAllParam, SetBasicParam, SetDefaultCalibration, exitFactory};

boolean showAboutScreenSubMenu = false;
const char *factorySettings[4] = {"All Parameters", "TV,IER,RR,Pmax", " Calibration  ", "     Exit     "};
class displayManager
{
public:

  /* Methods Used by Other Classes */
  void displayManagerloop(float *sensor_data, sensorManager &sM);
  void errorDisplay(ErrorDef_T errorState);
  void setDisplayParam(eDisplayPrm param, float value);
  void clearDisplay(void);

private:

  /* Utilities Methods */  
  float getDisplayParam(eDisplayPrm param);
  int rectifyBoundaries(int value, int minimum, int maximum);
  int getCalibratedParamFromPot(ctrl_parameter_t param);
  int getCalibValue(int potValue, int paramIndex);
  void drawRuntimeTopBottomLines(int currentPage, int totalPages, int topRight, int bottomLeft);
 
  /* Display Screens Related Methods */
  void displayStatusScreen(float *sensor_data, int statusScreenIndex);
  void pressureSensorsStatusScreen(void);
  void ierRrOxygenStatusScreen(float *sensor_data);
  void tidalVolumeStatusScreen(void);

  /* Edit Screens Related Methods */
  void editMenuHandler(RT_Events_T eRTState);
  void fio2SettingScreen(RT_Events_T eRTState);
  void aboutScreen(RT_Events_T eRTState);
  void lcdSettingScreen(RT_Events_T eRTState);
  void drawEditMenu();
  
  /* Edit Screens Related Methods */
  void drawServiceMenuScreen1(RT_Events_T eRTState); 
 
  /*BOOTUP Screens Related Methods */
  void drawSensorValueMenu(RT_Events_T eRTState);
  void drawSensorvoltageMenu(RT_Events_T eRTState);
  void drawOxygenCalibScreen(RT_Events_T eRTState, sensorManager sM);
  void drawDiagnosticScreen(RT_Events_T eRTState);
  void drawMessageScreen(const char* pHeading, int xPos, int yPos, const char* pMessage);

  //variables from here
  volatile STATE _dpState = STATUS_MENU;
  volatile short _currItem = 1;
  int _currentSaveFlag = 1;
  volatile unsigned long _lastEditMenuTime = 0;
  volatile unsigned long _lastSubEditMenuTime = 0;
  /*default true , we need to do lcd.clear once moved out from setup*/
  bool _refreshRunTimeDisplay = true;
  bool _refreshEditScreenDisplay = true;
  long unsigned _lastDisplayTime = 0;
  long unsigned _lastEditDisplayTime = 0;
  sensorManager *m_sM;
  float m_display_tve = 0;
  float m_display_tvi = 0;
  float m_display_peep = 0;
  float m_display_plat = 0;
  float m_display_pip = 0;
  float m_o2_sensor_data = 0;
  
};
/**@}*/
