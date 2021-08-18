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


// Definitions  : Global Variables --------------------------------------------
char        input_buffer[BUFF_LEN];
uint16_t    read_spot;
char        temp_string[33] = "Tworks!!";

// ISR Definitions ------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Static Function    :
// Summary			  :
// Parameters		  :
// Returns            :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ISR (USART3_RX_vect) {  // sets up the interrupt to receive any data coming in
    
    input_buffer[read_spot] = UDR3;
    read_spot++;//and "exports" if you will the data to a variable outside of the register
    //until the main program has time to read it. makes sure data isn't lost as much
    if (read_spot == BUFF_LEN) 
        read_spot = 0;
    
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


// Definitions  : Global Functions --------------------------------------------




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Got through and set up the registers for UART
void uart3_init (void) {
    
    UCSR3B |= (1 << RXEN3)  | (1 << TXEN3);  // transmit side of hardware
    UCSR3C |= (1 << UCSZ30) | (1 << UCSZ31); // receive side of hardware

    UBRR3L =  BAUD_PRESCALE;        // set the baud to 9600, have to split it into the two registers
    UBRR3H = (BAUD_PRESCALE >> 8);  // high end of baud register

    UCSR3B |= (1 << RXCIE3);        // receive data interrupt, makes sure we don't loose data

    #if DEBUG
//      uart3_send_str("0x04 - UART is up...");
    #endif

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void uart3_send_byte (uint8_t data) {
    /*
    Use this to send a 8bit long piece of data
    */
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
    UDR3 = data; //send the data
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
    UDR3 = '\n';//send a new line just to be sure
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void uart3_send_word (uint16_t data) {
    /*
    Use this to send a 16bit long piece of data
    */
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
    UDR3 = data;//send the lower bits
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
    UDR3 = (data >> 8); //send the higher bits
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
    UDR3 = '\n';//send a new line just to be sure
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void uart3_send_str (char *data) {
    /*
    Use this to send a string, it will split it up into individual parts
    send those parts, and then send the new line code
    */
    while (*data) 
    {
        while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
        UDR3 = *data; //goes through and splits the string into individual bits, sends them
        data += 1;//go to new bit in string
    }
    while ((UCSR3A & (1 << UDRE3)) == 0);//make sure the data register is cleared
        UDR3 = '\n';//send a new line just to be sure
    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t uart_get (void) {
    /*
    gets data from the register that the interrupt stored it
    in coming data into, returning it to the calling function as 8 bit long data
    */
    UCSR3B |= (1 << RXCIE3);

    //    sei();
    //    sleep_mode();
    //    cli();
    uint8_t b;
    
    if(read_spot == 0)
        b = input_buffer[sizeof(input_buffer) - 1];
    else
        b = input_buffer[read_spot - 1];
    if(b == '\r')
        b = '\n';
    return b;
    
}



/* uart.c -- ends here..*/






//------------------------ Scratch Area ---------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*void uart3_init (uint16_t ubrr_value)   {
    
    // todo
        // validate 'ubrr_value'

    UBRR3 = ubrr_value;

	// 9600-8-E-1
	// That is, baud rate of 9600 bps
	// 8 data bits
	// Even parity
	// 1 stop bit
	UCSR3B = (1 << TXEN3) | (1 <<  RXEN3) | (1 << RXCIE3); // And enable interrupts
	UCSR3C = (1 << UPM31) | (1 << UCSZ31) | (1 << UCSZ30);
    
}
*/

////void uart3_init (uint16_t   baudrate)   {
////    
//////    UART3_SET_BAUD_RATE (baudrate);
//////    UART3_SET_MODE_aSYSNCHRONOUS ();
//////    UART3_TRANSMIT_ENABLE ();    
//////    UART3_RECEIVE_ENABLE ();    
//////    
//////    /* Set frame format: 8data, 2stop bit */
//////	UCSR3C = (1 << USBS3) | (3 << UCSZ30);
//////    
//////    UART3_TX_COMPLETE_INTR_EN ();
//// 
////    	/* Set baud rate */
////	UBRR3H = (unsigned char)(baudrate >> 8);
////	UBRR3L = (unsigned char)baudrate;
////
////	/* Enable receiver and transmitter */
////	UCSR3B = (1<<RXEN3)|(1<<TXEN3);
////	/* Set frame format: 8data, 2stop bit */
////	UCSR3C = (1<<USBS3)|(3<<UCSZ30);
////    
//////    // Enable interrupts
//////	sei();
////    
////    
////    
////}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global Function :
// Summary         :
// Parameters      :
// Returns         :
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void adc_init (void) {
//    
//	// Setup ADC
//	// Enable left adjust a resolution of 8 bits is enough
//	// and select first ADC channel
//	ADMUX = (1 << ADLAR);
//
//	// Enable the ADC unit, and use a pre-scaler
//	// of 64 which gives us a fadc of 125kHz
//	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
//    
//}


/**
 * Called when USART completes receiving data
 *
 * It checks if there's no error, and if the character r is received,
 * and ADC conversion is started
 */
//ISR (USART3_RX_vect) {
//    
//	// Called when data received from USART
//
//	// Read UDR register to reset flag
//	unsigned char data = UDR3;
//
//	// Check for error
//	if ((UCSR3A & ((1 << FE0) | (1 << DOR3) | (1 << UPE3))) == 0)	{
//		// No error occurred
//		if (data == 'r')	{
//			// Start ADC Conversion
//			ADCSRA = (1 << ADSC);
//		}
//	}
//}

///**
// * Called when the data register accepts new data
// *
// * When this occurs, we can write new data through the USART,
// * and in this case we write the ADCH value.
// */
//ISR (USART3_UDRE_vect)  {
//    
//	// Write ADC value
//	UDR3 = ADCH;
//
//	// Disable this interrupt
//	UCSR3B &= ~(1 << UDRIE3);
//    
//}

/**
 * Called when the ADC completes a conversion.
 *
 * It enables the USART Data register empty interrupt,
 * so when ready, the uC can send this value back to the computer
 */
//ISR (ADC_vect)  {
//    
//	static uint8_t i = 0;
//
//	UCSR3B |= (1 << UDRIE3);
//
//	i++;
//
//	if (i < 6)	{
//		// Start a new conversion
//		ADCSRA = (1 << ADSC);
//	}
//	else	{
//		i = 0;
//	}
//    
//}