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
 * File			: uart.c 
 * Author		: Firmware Team member
 * Comments		: Code file which contains uart related variables  &  
 *  			function definition.
 * Revision history: 
 *				Created on 17-Aug-2021
 */



// include processor files - #include <>  -------------------------------------
#include <xc.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


// include project files - #include "" ----------------------------------------
#include "../inc/uart.h"


// Local '#' defines ----------------------------------------------------------



// Local 'Macros' -------------------------------------------------------------
									

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
/**
 * Called when USART completes receiving data
 *
 * It checks if there's no error, and if the character r is received,
 * and ADC conversion is started
 */
ISR (USART3_RX_vect) {
    
	// Called when data received from USART

	// Read UDR register to reset flag
	unsigned char data = UDR3;

	// Check for error
	if ((UCSR3A & ((1 << FE0) | (1 << DOR3) | (1 << UPE3))) == 0)	{
		// No error occurred
		if (data == 'r')	{
			// Start ADC Conversion
			ADCSRA = (1 << ADSC);
		}
	}
}

/**
 * Called when the data register accepts new data
 *
 * When this occurs, we can write new data through the USART,
 * and in this case we write the ADCH value.
 */
ISR (USART3_UDRE_vect)  {
    
	// Write ADC value
	UDR3 = ADCH;

	// Disable this interrupt
	UCSR3B &= ~(1 << UDRIE3);
    
}

/**
 * Called when the ADC completes a conversion.
 *
 * It enables the USART Data register empty interrupt,
 * so when ready, the uC can send this value back to the computer
 */
ISR (ADC_vect)  {
    
	static uint8_t i = 0;

	UCSR3B |= (1 << UDRIE3);

	i++;

	if (i < 6)	{
		// Start a new conversion
		ADCSRA = (1 << ADSC);
	}
	else	{
		i = 0;
	}
    
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


// Definitions  : Global Variables --------------------------------------------
// Definitions  : Global Functions --------------------------------------------



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void usart_init (uint16_t ubrr_value)   {
	
    UBRR3 = ubrr_value;

	// 9600-8-E-1
	// That is, baudrate of 9600bps
	// 8 databits
	// Even parity
	// 1 stopbit
	UCSR3B = (1 << TXEN3) | (1 << RXEN3) | (1 << RXCIE3); // And enable interrupts
	UCSR3C = (1 << UPM31) | (1 << UCSZ31) | (1 << UCSZ30);
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void adc_init (void) {
    
	// Setup ADC
	// Enable left adjust a resolution of 8 bits is enough
	// and select first ADC channel
	ADMUX = (1 << ADLAR);

	// Enable the ADC unit, and use a prescaler
	// of 64 which gives us a fadc of 125kHz
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





/* uart.c -- ends here..*/



//------------------------ Scratch Area ---------------------------------------

