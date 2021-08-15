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
 * File			: led.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains led related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 12-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// include project files - #include "" ----------------------------------------
#include "../inc/led.h"


// '#' defines ----------------------------------------------------------------


// 'Macros' -------------------------------------------------------------------
																			  
// Definitions  : Classes -----------------------------------------------------
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

// Definitions  : Structure ---------------------------------------------------
// Definitions  : Unions ------------------------------------------------------
// Definitions  : Enums -------------------------------------------------------



// ISR Definitions ------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//=============================================================================




// Static Declarations of Variables -------------------------------------------
// Static Declarations of Functions -------------------------------------------

// Static Definitions of Variables --------------------------------------------
// Static Definitions of Functions --------------------------------------------
//=============================================================================
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//=============================================================================


// Definitions  : Global Variables --------------------------------------------
uint8_t     led_status_byte;    // 0 - OFF, 1 - ON, one bit for each LED
//uint8_t     led_blink_byte;
//uint8_t     led_toggle_byte;
//uint8_t     led_byte;   
uint8_t     skip_count;   

// Definitions  : Global Functions --------------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void led_control (enum LED_ID_E led_sel, enum LED_E led_cmd)   {

    register uint8_t  i;
    register uint8_t  led_byte;
    
    
    // trim the incoming parameter
    led_sel &= LED_MASK;
    
    led_byte = led_status_byte;
    
    switch (led_cmd)    
    {       
        case LED_ON:
            led_byte |= led_sel; 
            LED_WORD_WRITE (led_byte);
            break;
        case LED_OFF:
            led_byte &= ~led_sel; 
            LED_WORD_WRITE (led_byte);
            break;
        case LED_BLINK:
            // temp blocking implementation
            // todo - delay to be made non-blocking
            for (i = 0; i < 6; i++)
            {
                led_byte ^= ~led_sel; 
                LED_WORD_WRITE (led_byte);
                _delay_ms(33);
            }
            break;
        case LED_TOGGLE:
            led_byte ^= led_sel; 
            LED_WORD_WRITE (led_byte);
            break;
    }
    
    led_status_byte = led_byte;
            
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void led_scroll_up (enum LED_ID_E led_sel, uint8_t scroll_cnt)  {
    
    // trim the incoming parameter
    led_sel &= LED_MASK;
    
    // validate 'delay' parameter
    if (scroll_cnt > MAX_SCROLL_COUNT)  {
        scroll_cnt = MAX_SCROLL_COUNT;
    }
    
    skip_count++;
    if (skip_count >= scroll_cnt) {
        skip_count = 0;
        led_status_byte <<= 1;
        if ((led_status_byte & LED_MASK) == 0)  {
            led_status_byte = led_sel;
        }
        LED_WORD_WRITE (led_status_byte);
    }
     
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void led_scroll_down (enum LED_ID_E led_sel, uint8_t scroll_cnt)  {
    
    // trim the incoming parameter
    led_sel &= LED_MASK;
    
    // validate 'delay' parameter
    if (scroll_cnt > MAX_SCROLL_COUNT)  {
        scroll_cnt = MAX_SCROLL_COUNT;
    }
    
    skip_count++;
    if (skip_count >= scroll_cnt) {
        skip_count = 0;
        led_status_byte >>= 1;
        if ((led_status_byte & LED_MASK) == 0)  {
            // led_status_byte = led_sel;
            int i;
            for (i = 0; i < 7; i++)
            {          
                if (led_sel & LED_MSB)  {
                    break;
                }
                led_sel <<= 1;
            }
            led_status_byte = led_sel;
        }
        LED_WORD_WRITE (led_status_byte);
    }
         
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void led_clear_all_leds (void)   {

    led_control (ALL_LEDs, LED_OFF);
    
}


/* led.c -- ends here..*/



//------------------------ Scratch Area ---------------------------------------

