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
 * File			: platform.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains platform related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 11-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 
#include <stdio.h> 

// include project files - #include "" ----------------------------------------
#include "../inc/platform.h"


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
void uc_init   (void);
void gpio_init (void);


// Static Definitions of Variables --------------------------------------------
// Static Definitions of Functions --------------------------------------------

//=============================================================================
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//=============================================================================
void uc_init (void)  {
    
    // Place holder for MCU clock source / frequency changes (if needed).
    
}

//=============================================================================
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//=============================================================================
void gpio_init (void)   {
    
    // port A
    // DIR_INPUT_PORTA ();
    DIR_OUTPUT_PORTA (UL0_LED1_PIN | UL1_LED2_PIN | UL2_LED3_PIN |      \
                      UL3_LED4_PIN | UL4_LED5_PIN | UL5_LED6_PIN | BUZZER_PIN);
                      
    // port B
    // DIR_INPUT_PORTB ();
    DIR_OUTPUT_PORTB (CS_DIGIPOT_PIN | LCD_DB4_PIN | LCD_DB5_PIN |      \
                         LCD_DB6_PIN | LCD_DB7_PIN);
                      
    // port C
    // ALL pins are NCs
                      
    // port D
       // I2C BUS to initialize
    DIR_INPUT_PORTD (INTA_ROTENC_PIN | INTB_ROTENC_PIN | RDY_EXT_PIN);
    // DIR_OUTPUT_PORTD ();
    
    // port E
    DIR_INPUT_PORTE (START_BUTTON_PIN | BUTTON_ROTENC_PIN);
    // DIR_OUTPUT_PORTE ();
                 
    // port F
    // DIR_INPUT_PORTF ();
    // DIR_OUTPUT_PORTF ();
	AIN_POT0_ADC0();
	AIN_POT1_ADC1();
	AIN_POT2_ADC2();
    AIN_POT3_ADC3();
	
    // port G
    // DIR_INPUT_PORTG ();
    // DIR_OUTPUT_PORTG ();
                      
    // port H
    // DIR_INPUT_PORTH ();
    // DIR_OUTPUT_PORTH ();
        // UART2 to initialize
                      
    // port J
    // DIR_INPUT_PORTJ ();
    // DIR_OUTPUT_PORTJ ();
       // UART3 to initialize
                      
       // port K
    // DIR_INPUT_PORTK ();
    DIR_OUTPUT_PORTK (LCD_BCK_LIGHT_PIN | LCD_RS_PIN | LCD_EN_PIN);
 
    // port L
    // DIR_INPUT_PORTJ ();
    // DIR_OUTPUT_PORTJ ();
}




// Definitions  : Global Variables --------------------------------------------


// Definitions  : Global Functions --------------------------------------------


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_init (void)   {
    
    uc_init ();
    gpio_init ();
    systick_timer_init ();
    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1msec_tasks (void) {
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_10msec_tasks (void)   {

    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_100msec_tasks (void)   {

    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1sec_tasks (void)   {
    
    systemtick_secs++; 
       
    c_secs++;
    if (c_secs >= 59)   {
        c_secs = 0;
        f_1min = 1;
        c_mins++;
        systemtick_mins++;
        if (c_mins >= 59)   {
            c_mins = 0;
            f_1hr = 1;
            c_hrs++;
            if (c_hrs >= 24)    {
                c_hrs = 0;
                f_1day = 1;
                // i am tired now..!! 
            }            
        }       
    }
    
    
    // periodic 1 sec tasks .. to be called here below..

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1min_tasks (void)   {
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1hr_tasks (void)     {
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1day_tasks (void)   {
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1month_tasks (void)   {
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void platform_1year_tasks (void)   {
    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/* platform.c -- ends here.. */



//------------------------ Scratch Area ---------------------------------------

