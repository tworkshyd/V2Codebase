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
 * File			: uart.h 
 * Author		: Firmware Team member
 * Comments		: Header file which contains uart related defines and 
 *              function prototypes.
 * Revision history: 
 *				Created on 17-Aug-2021
 */


 
// Guard condition to prevent multiple file inclusion
#ifndef uart_h
#define	uart_h

// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// include project files - #include "" ----------------------------------------

// '#' defines ----------------------------------------------------------------
//#define FOSC        8000000
//#define BAUDRATE    9600
#define FOSC        (16000000)
#define BAUDRATE    (9600)



// 'Macros' -------------------------------------------------------------------
#define UBRR        ((FOSC / (BAUDRATE << 4)) - 1)
// UART0 & 1
    // todo

// UART2
    // todo

// UART3
// 1. (23.6.2) UCSRnA ? USART MSPIM Control and Status Register 3 A
#define IS_UART3_RECEIVE_COMPLETE()     (UCSR3A & (1 << RXC3))
#define IS_UART3_TRANSMIT_COMPLETE()    (UCSR3A & (1 << TXC3))
#define IS_UART3_TX_DATA_REG_EMPTY()    (UCSR3A & (1 << UDRE3))

// 2. (23.6.3) UCSRnB ? USART MSPIM Control and Status Register n B
#define UART3_RX_COMPLETE_INTR_EN()     (UCSR3B |=  (1 << RXCIE3))
#define UART3_RX_COMPLETE_INTR_DIS()    (UCSR3B &= ~(1 << RXCIE3))

#define UART3_TX_COMPLETE_INTR_EN()     (UCSR3B |=  (1 << TXCIE3))
#define UART3_TX_COMPLETE_INTR_DIS()    (UCSR3B &= ~(1 << TXCIE3))

#define UART3_DATA_REG_EMTY_INTR_EN()   (UCSR3B |=  (1 << UDRIE3))
#define UART3_DATA_REG_EMTY_INTR_DIS()  (UCSR3B &= ~(1 << UDRIE3))

#define UART3_RECEIVE_ENABLE()          (UCSR3B |=  (1 << RXEN3))
#define UART3_RECEIVE_DISABLE()         (UCSR3B &= ~(1 << RXEN3))

#define UART3_TRANSMIT_ENABLE()         (UCSR3B |=  (1 << TXEN3))
#define UART3_TRANSMIT_DISABLE()        (UCSR3B &= ~(1 << TXEN3))



// 3. (23.6.4) UCSRnC ? USART MSPIM Control and Status Register n C
// Bit 7:6 - UMSELn1:0: USART Mode Select
#define UART3_SET_MODE_aSYSNCHRONOUS()  (UCSR3C |= (0 << UMSEL31) | (0 << UMSEL30))  //  Asynchronous USART
#define UART3_SET_MODE_SYSNCHRONOUS()   (UCSR3C |= (0 << UMSEL31) | (1 << UMSEL30))  //  Synchronous USART
#define UART3_SET_MODE_MASTER_SPI()     (UCSR3C |= (1 << UMSEL31) | (1 << UMSEL30))  //  Master SPI (MSPIM)
//#define UART3_SET_DATA_ORDR_LSB_FIRST() (UCSR3C |= (1 << UDORD3))
//#define UART3_SET_LEADING_EDGE_SAMPLE() (UCSR3C |= (1 << UCPHA3))
//#define UART3_SET_CLOCK_POLARITY()      (UCSR3C |= (1 << UCPOL3))

// 4. (22.10.5) UBRRnL and UBRRnH ? USART Baud Rate Registers
#define UART3_SET_BAUD_RATE(x)          (UBRR3 = ((FOSC / (x << 4)) - 1))

// 5. (22.10.1) UDRn ? USART I/O Data Register n
#define UART3_READ_DATA_REGISTER()      (UDR3)
#define UART3_WRITE_DATA_REGISTER(x)    (UDR3 = x)



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
// Declarations : Global Variables --------------------------------------------


// Declarations : Functions ---------------------------------------------------
void uart3_init (uint16_t ubrr_value);
void adc_init   (void);
    




#endif	/* uart_H */

//------------------------ Scratch Area ---------------------------------------


