//Error Indicators (23-28)
#define LED_1_PIN       23  
#define LED_2_PIN       24
#define LED_3_PIN       25
#define LED_4_PIN       26
#define LED_5_PIN       27
#define LED_6_PIN       28 //led pin near IC 

//SPI pins for lcd contrast control
#define LCD_CS_DIGIPOT       53
#define LCD_SCK_DIGIPOT      52
#define LCD_MOSI_DIGIPOT      51  

//Display
#define DISPLAY_1_PIN       A10   // Changed
#define DISPLAY_2_PIN       A11   // Changed
#define DISPLAY_3_PIN       A12   // Changed
#define DISPLAY_4_PIN       A13   // Changed
#define DISPLAY_RS_PIN       43   // Changed
#define DISPLAY_EN_PIN       42   // Changed
#define DISPLAY_BACK_LED_PIN       44   // Changed

//Control Pots
#define TIDAL_VOLUME_PIN   A1 //A4      //Unchanged  5
#define RR_PIN             A2 //A2    //Unchanged
#define PMAX_PIN           A3 //A1    //Unchanged
#define FiO2_PIN           A0 // A0    //Unchanged


#define POT_5_PIN     A8    // New
#define OXYGEN_ANALOG_PIN   A9  //on extensiion header (Pin number 7) //not used
#define ADS115_INT_PIN      38 // Interrupt pin ALRT ..on extensiion header (Pin number 5)
#define ADS115_INT_PIN_1    A15 // Interrupt pin ALRT ..on extensiion header (Pin number 5)

// ADDR pin is connected to ground on extensiion header (Pin number 4,6,8,10,12,14,16,18,20)
#define ADS115_I2C_SCL_PIN         21   //Unchanged  on extensiion header (Pin number 1)
#define ADS115_I2C_SDA_PIN         20   // Unchanged on extensiion header (Pin number 3)

#define AUTO_MODE      30            /*!< Auto mode  is attached to pin 10*/ //not used
#define ASSISTED_MODE  31            /*!< Assisted mode  is attached to pin 10*/ //not used

 //I2C
#define I2C_SCL_PIN         21   //Unchanged
#define I2C_SDA_PIN         20   // Unchanged

#define BUZZER_PIN          22

//Encoder Pins
#define DISP_ENC_SW          3   
#define DISP_ENC_CLK         19 
#define DISP_ENC_DT          18  

//Start Stop Button
#define RESET_SWITCH        2

// Serial0
//#define TX_PIN               D1   // Unchanged
//#define RX_PIN               D0   // Unchanged

// Serial3
//#define TX_PIN               15   // Unchanged
//#define RX_PIN               14   // Unchanged

// Serial RS485
#define TX_PIN_485               1   // Unchanged
#define RX_PIN_485               0   // Unchanged
#define RTS_PIN_485              6     //not connected on schematics

#define POWER_SUPPLY_FAILURE   47    /*!< Power Supply Failure  is attached to pin 40 */ //not connected
#define GAS_SUPPLY_FAILURE     48    /*!< Gas Supply Failure  is attached to pin 41 */   //not connected
#define MECH_FAILSAFE_VALVE    49    /*!< Mech Failsafe Valve  is attached to pin 42 */  //not connected

//#define ONBOARD_LED_PIN  13

#define O2_CYN_SWITCH 40
#define O2_HOSP_SWITCH 41

#define BoardToBoard Serial3
#define BoardToPlotter Serial
#define BoardToRaspberryPi  Serial2
 
/*
 * PinOut
 * With comments on changes from older one.
 */
