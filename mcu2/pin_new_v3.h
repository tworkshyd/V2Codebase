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
#define DISPLAY_1_PIN       13   // Changed
#define DISPLAY_2_PIN       12   // Changed
#define DISPLAY_3_PIN       11   // Changed
#define DISPLAY_4_PIN       10   // Changed
#define DISPLAY_EN_PIN       42   // Changed
#define DISPLAY_RS_PIN       43   // Changed

#define DISPLAY_BACK_LED_PIN       44   // Changed

//Control Pots
#define TIDAL_VOLUME_PIN   A0      //Unchanged  5
#define RR_PIN             A1     //Unchanged
#define PMAX_PIN           A2     //Unchanged
#define FiO2_PIN           A3     //Unchanged


#define POT_5_PIN     A8    // New
// #define OXYGEN_ANALOG_PIN   A9  //on extensiion header (Pin number 7) //not used
#define ADS115_INT_PIN      38 // Interrupt pin ALRT ..on extensiion header (Pin number 5)
#define ADS115_INT_PIN_1    A15 // Interrupt pin ALRT ..on extensiion header (Pin number 5)

// ADDR pin is connected to ground on extensiion header (Pin number 4,6,8,10,12,14,16,18,20)
#define ADS115_I2C_SCL_PIN         21   //Unchanged  on extensiion header (Pin number 1)
#define ADS115_I2C_SDA_PIN         20   // Unchanged on extensiion header (Pin number 3)



#define BUZZER_PIN          22

//Encoder Pins
#define DISP_ENC_SW          3   
#define DISP_ENC_CLK         19 
#define DISP_ENC_DT          18  

//Start Stop Button
#define RESET_SWITCH        2


// Serial Debug port Serial0
#define TXD_PROG               1   // Unchanged
#define RXD_PROG               0   // Unchanged
//#define DTS_PROG                   //not connected on schematics



#define O2_CYN_SWITCH 41
// #define O2_HOSP_SWITCH 40 //connected on schematic but not used

#define BoardToBoard Serial3
#define BoardToPlotter Serial

 
/*
 * PinOut
 * With comments on changes from older one.
 */
