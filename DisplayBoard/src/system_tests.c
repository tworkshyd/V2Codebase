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
 * File			: system_tests.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains system_tests related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 12-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// include project files - #include "" ----------------------------------------
#include "../inc/platform.h"
#include "../inc/system_tests.h"


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


// Definitions  : Global Functions --------------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void test_leds (void)   {
    
    //int                 scroll_delay = 100;
    //static uint8_t      led_word = 1;
    //static uint8_t      count;
    //static uint8_t    mask = 1;
    
//    BUZZER_WRITE(1);		
//    _delay_ms (1000);
//    BUZZER_WRITE(0);
//    _delay_ms (5000);
   
//    UL0_LED1_WRITE(1);		
//    _delay_ms (1000);
//    UL0_LED1_WRITE(0);
//    _delay_ms (5000);
 
//    UL0_LED1_WRITE(1);		
//    UL1_LED2_WRITE(1);		
//    UL2_LED3_WRITE(1);		
//    UL3_LED4_WRITE(1);		
//    UL4_LED5_WRITE(1);		
//    UL5_LED6_WRITE(1);		
//    _delay_ms (scroll_delay);
//    UL0_LED1_WRITE(0);
//    _delay_ms (scroll_delay);	
//    UL1_LED2_WRITE(0);		
//	  _delay_ms (scroll_delay);
//    UL2_LED3_WRITE(0);		
//	  _delay_ms (scroll_delay);
//    UL3_LED4_WRITE(0);	
//	  _delay_ms (scroll_delay);	
//    UL4_LED5_WRITE(0);		
//	  _delay_ms (scroll_delay);
//    UL5_LED6_WRITE(0);	
//	  _delay_ms (scroll_delay);
    
//    led_word <<= 1;
//    if (led_word == 0x3F)
//        led_word = 0;
//    else if (led_word == 0)
//        led_word = 1;
    
//    led_control (LED_1, LED_OFF);
//    led_control (LED_2, LED_BLINK);
//    _delay_ms(3000);
//    led_control (LED_3, LED_TOGGLE);
//    _delay_ms(3000);
//    led_control (LED_4, LED_BLINK);
//    _delay_ms(3000);
//    led_control (LED_5, LED_TOGGLE);
//    _delay_ms(3000);
//    led_control (LED_6, LED_OFF);
//    _delay_ms(3000);
//    
//    led_control (LED_1, LED_ON);
//    led_control (LED_2, LED_OFF);
//    led_control (LED_3, LED_TOGGLE);
//    led_control (LED_4, LED_OFF);
//    led_control (LED_5, LED_TOGGLE);
//    led_control (LED_6, LED_ON);
//    _delay_ms(3000);  
    // LED_WORD_WRITE(led_word);

    
//    int i;
    
//    for (i = 0; i < 5; i++)
//    {
//        led_scroll_up (LED_1 | LED_2 | LED_3);
//        _delay_ms(333);
//    }
    
    
    //_delay_ms(1000);
    // led_clear_all_leds ();
    
//    for (i = 0; i < 5; i++)
//    {
//        led_scroll_down (LED_1 | LED_2 | LED_3);
//        _delay_ms(333);       
//    }

//    int i;
//    for (i = 0; i < 25; i++)
//    {
//        led_show_inhale ();
//        _delay_ms(333);
//    }
//    for (i = 0; i < 25; i++)
//    {
//        led_show_exhale ();
//        _delay_ms(333);
//    }
    
    // UL1_LED2_WRITE(x) 
    // UL2_LED3_WRITE(x) 
    // UL3_LED4_WRITE(x) 
    // UL4_LED5_WRITE(x) 
    // UL5_LED6_WRITE(x) 

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void test_buzzer (void)   {
        
    SET_PORTA (GPIO_PIN_1);
    _delay_ms (1000);
    RESET_PORTA (GPIO_PIN_1);
    _delay_ms (1000);

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/* system_tests.c -- ends here.. */



//------------------------ Scratch Area ---------------------------------------

