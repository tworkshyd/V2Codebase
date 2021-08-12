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
    
    DIR_OUTPUT_PORTA(GPIO_PIN_ALL);


}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void test_buzzer (void)   {
    
    SET_PORTA (GPIO_PIN_1);
    //_delay_ms (1000);
    RESET_PORTA (GPIO_PIN_1);
    //_delay_ms (1000);

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/* system_tests.c -- ends here.. */



//------------------------ Scratch Area ---------------------------------------

