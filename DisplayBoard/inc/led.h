/*
 * MIT License
 *
 * Copyright (c) 2020 T Works Foundation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/*
 * File			: led.h 
 * Author		: Firmware Team member
 * Comments		: Header file which contains led related defines and 
 *              function prototypes.
 * Revision history: 
 *				Created on 12-Aug-2021
 */


 
// Guard condition to prevent multiple file inclusion
#ifndef LED_H
#define	LED_H

// include processor files - #include <>  -------------------------------------

// include project files - #include "" ----------------------------------------
#include "../inc/platform.h"


// '#' defines ----------------------------------------------------------------
#define MAX_SCROLL_COUNT    (10)


// LEDs assignments 
#define RX_DBG_PORT     UL0_LED1_PIN
#define TX_DBG_PORT     UL1_LED2_PIN
#define SYSTEM_LED      UL2_LED3_PIN
#define I2C_COMM        UL3_LED4_PIN
#define RX_COMM_PORT    UL4_LED5_PIN
#define TX_COMM_PORT    UL5_LED6_PIN    


// 'Macros' -------------------------------------------------------------------

// Declarations : Classes -----------------------------------------------------
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

// Declarations : Structure ---------------------------------------------------
// Declarations : Unions ------------------------------------------------------
// Declarations : Enums -------------------------------------------------------
enum LED_E {
    
    LED_OFF    = 0,
    LED_ON     = 1,
    LED_BLINK  = 2,
    LED_TOGGLE = 3, 
    
};

enum LED_ID_E {
    
    LED_1 = 0x01,
    LED_2 = 0x02,
    LED_3 = 0x04,
    LED_4 = 0x08,
    LED_5 = 0x10,
    LED_6 = 0x20,
    
    LED_MSB  = LED_6,
    
    ALL_LEDs = LED_1 | LED_2 | LED_3 | \
               LED_4 | LED_5 | LED_6,
    
    LED_MASK = ALL_LEDs,
   
};


// Declarations : Global Variables --------------------------------------------
extern uint8_t     led_status_byte;    // 0 - OFF, 1 - ON, one bit for each LED
extern uint8_t     led_blink_byte;
extern uint8_t     led_toggle_byte;


// Declarations : Functions ---------------------------------------------------
void led_control        (enum LED_ID_E led_sel, enum LED_E led_cmd);
void led_scroll_up      (enum LED_ID_E led_sel);
void led_scroll_down    (enum LED_ID_E led_sel);
void led_clear_all_leds (void);


#endif	/* LED_H */

//------------------------ Scratch Area ---------------------------------------


