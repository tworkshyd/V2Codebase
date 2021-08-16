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
 * File			: timer.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains timer related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 12-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 
#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>

// include project files - #include "" ----------------------------------------
#include "../inc/timer.h"
#include "../inc/atmega2560.h"


// '#' defines ----------------------------------------------------------------
#define LED     PA6
//  4 msecs approx.
//#define SYSTICK_TIMER_COUNT         (63974)
//#define SYSTICK_TIMER_PRESCALER     (1 << CS10)  // Timer mode with 1 no pre-scaling

//// 1. came down to 3usecs, Freq = 163kHz
//#define SYSTICK_TIMER_COUNT         (65535)
//#define SYSTICK_TIMER_PRESCALER     (1 << CS10)  // Timer mode with 1 no pre-scaling

//// 2. came down to 17usecs, Freq = 30kHz
//#define SYSTICK_TIMER_COUNT         (65535)
//#define SYSTICK_TIMER_PRESCALER     ((1 << CS12) | (0 << CS11) | (0 << CS10))  // Timer mode with 1 no pre-scaling

//// 3. came down to 65usecs, Freq = 7kHz
//#define SYSTICK_TIMER_COUNT         (65535)
//#define SYSTICK_TIMER_PRESCALER     ((1 << CS12) | (0 << CS11) | (1 << CS10))  // Timer mode with 1 no pre-scaling

//// 4. came down to 3 Secs, Freq = 131.6mHz
//#define SYSTICK_TIMER_COUNT         (55530)
//#define SYSTICK_TIMER_PRESCALER     ((1 << CS12) | (0 << CS11) | (1 << CS10))  // Timer mode with 1 no pre-scaling

////// 5. came down to 650 msecs, Freq = 781.3mHz
////#define SYSTICK_TIMER_COUNT         (55000)
////#define SYSTICK_TIMER_PRESCALER     ((1 << CS12) | (0 << CS11) | (1 << CS10))  // Timer mode with 1 no pre-scaling

//// 6. came down to 640 usecs, Freq = 757.3Hz
//#define SYSTICK_TIMER_COUNT         (55000)
//#define SYSTICK_TIMER_PRESCALER     ((0 << CS12) | (0 << CS11) | (1 << CS10))  // Timer mode with 1 no pre-scaling

// 7. came down to 1.000 msecs, Freq = 1.000kHz
#define SYSTICK_TIMER_COUNT         (51001)
#define SYSTICK_TIMER_PRESCALER     ((0 << CS12) | (0 << CS11) | (1 << CS10))  // Timer mode with 1 no pre-scaling

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
ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	//PORTA ^= (1 << LED);		
    
    PORTA |= (1 << LED);	
    
    volatile unsigned int i = 100;
    while (i--)
        ;
    
    PORTA &= ~(1 << LED);	

	// TCNT1 = 63974;   // for 1 sec at 16 MHz
    // TCNT1 = 639;
    TCNT1 = SYSTICK_TIMER_COUNT;
            
    // Clear Interrupt flag..
    TIFR1 &= ~(1 << TOV1);
    
    
    
}







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
void systick_timer_init (void)  {
    

    DDRD = (0x01 << LED);     //Configure the PORTD4 as output
	
	// TCNT1 = 63974;   // for 1 sec at 16 MHz	
	TCNT1 = SYSTICK_TIMER_COUNT;

	TCCR1A = 0x00;
    //	TCCR1B = (1 << CS10) | (1 << CS12);;  // Timer mode with 1024 prescler
	TCCR1B = SYSTICK_TIMER_PRESCALER;
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
    
}


// Definitions  : Global Variables --------------------------------------------
// Definitions  : Global Functions --------------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
    Steps to configure the Timer Interrupt:

    Load the TCNT1 register with the value calculated above.
    Set CS10 and CS12 bits to configure pre-scalar of 1024
    Enable timer1 overflow interrupt(TOIE1), the register is shown below
    Enable global interrupts by setting global interrupt enable bit in SREG
    Toggle the LED in the ISR and reload the TCNT value.
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned int TIM16_ReadTCNTn ( void )    {
    
    unsigned char   sreg;
    unsigned int    i;
    
    /* Save global interrupt flag */
    sreg = SREG;
    /* Disable interrupts */
//    __disable_interrupt();
   
    /* Read TCNTn into i */
    i = SYSTICK_TIMER;
    /* Restore global interrupt flag */
    SREG = sreg;
    
    return i;
    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TIM16_WriteTCNTn ( unsigned int i ) {
    
    unsigned char sreg;
    // unsigned int i;
    
    /* Save global interrupt flag */
    sreg = SREG;
    /* Disable interrupts */
//    __disable_interrupt();
    /* Set TCNTn to i */
    
    SYSTICK_TIMER = i;
    /* Restore global interrupt flag */
    SREG = sreg;

}



/* timer.c -- ends here..*/



//------------------------ Scratch Area ---------------------------------------

