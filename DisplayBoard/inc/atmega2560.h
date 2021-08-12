/*
 * File:   atmega2560.h
 * Author: abdul Wahab herial
 *
 * Created on 10 August, 2021, 6:18 PM
 */


// Guard condition to prevent multiple file inclusion
#ifndef ATMEGA2560_H
#define	ATMEGA2560_H

#include <avr/io.h>


/*
 * From ATmega640/V-1280/V-1281/V-2560/V-2561/V [DATASHEET] 
 * pg.no.68 [2549Q?AVR?02/2014]
    13.2.1 Configuring the Pin
    Each port pin consists of three register bits: DDxn, PORTxn, and PINxn. 
 * As shown in ?Table 13-34 and Table 13-
    35 relates the alternate functions of Port L to the overriding signals 
 * shown in Figure 13-5 on page 73.? on page 95,
    the DDxn bits are accessed at the DDRx I/O address, the PORTxn bits at the 
 * PORTx I/O address, and the PINxn
    bits at the PINx I/O address.
    The DDxn bit in the DDRx Register selects the direction of this pin. 
 * If DDxn is written logic one, Pxn is configured
    as an output pin. If DDxn is written logic zero, Pxn is configured as an 
 * input pin.
    If PORTxn is written logic one when the pin is configured as an input pin, 
 * the pull-up resistor is activated. To switch
    the pull-up resistor off, PORTxn has to be written logic zero or the pin has
 *  to be configured as an output pin. The
    port pins are tri-stated when reset condition becomes active, even if no 
 * clocks are running.
    If PORTxn is written logic one when the pin is configured as an output pin, 
 * the port pin is driven high (one). If
    PORTxn is written logic zero when the pin is configured as an output pin, 
 * the port pin is driven low (zero).
        
*/

#define GPIO_PIN_ALL  (0xFF)
#define GPIO_PIN_0    (1 << PIN0)
#define GPIO_PIN_1    (1 << PIN1)
#define GPIO_PIN_2    (1 << PIN2)
#define GPIO_PIN_3    (1 << PIN3)
#define GPIO_PIN_4    (1 << PIN4)
#define GPIO_PIN_5    (1 << PIN5)
#define GPIO_PIN_6    (1 << PIN6)
#define GPIO_PIN_7    (1 << PIN7)


// Macro Definitions

// Port Directions setting (1 = output, 0 = input)

// Set to 'Out-put' either a pin / all pins / a combination of pins using '|' 
#define DIR_OUTPUT_PORTA(x)		(DDRA |= (x))
#define DIR_OUTPUT_PORTB(x)		(DDRB |= (x))
#define DIR_OUTPUT_PORTC(x)		(DDRC |= (x))
#define DIR_OUTPUT_PORTD(x)		(DDRD |= (x))
#define DIR_OUTPUT_PORTE(x)		(DDRE |= (x))
#define DIR_OUTPUT_PORTF(x)		(DDRF |= (x))
#define DIR_OUTPUT_PORTG(x)		(DDRG |= (x))
#define DIR_OUTPUT_PORTH(x)		(DDRH |= (x))

// Set to 'In-put' either a pin / all pins / a combination of pins using '|' 
#define DIR_INPUT_PORTA(x)		(DDRA &= ~(x))
#define DIR_INPUT_PORTB(x)		(DDRB &= ~(x))
#define DIR_INPUT_PORTC(x)		(DDRC &= ~(x))
#define DIR_INPUT_PORTD(x)		(DDRD &= ~(x))
#define DIR_INPUT_PORTE(x)		(DDRE &= ~(x))
#define DIR_INPUT_PORTF(x)		(DDRF &= ~(x))
#define DIR_INPUT_PORTG(x)		(DDRG &= ~(x))
#define DIR_INPUT_PORTH(x)		(DDRH &= ~(x))


// Port Input Data Register
// Read one bit at a time
#define READ_PORTA(x)		(PINA & (x))
#define READ_PORTB(x)		(PINB & (x))
#define READ_PORTC(x)		(PINC & (x))
#define READ_PORTD(x)		(PIND & (x))
#define READ_PORTE(x)		(PINE & (x))
#define READ_PORTF(x)		(PINF & (x))
#define READ_PORTG(x)		(PING & (x))
#define READ_PORTH(x)		(PINH & (x))


// Port Output Data Register
// Set either a pin, all pins or a combination of pins using '|' operator
#define SET_PORTA(x)		(PORTA |= (x))
#define SET_PORTB(x)		(PORTB |= (x))
#define SET_PORTC(x)		(PORTC |= (x))
#define SET_PORTD(x)		(PORTD |= (x))
#define SET_PORTE(x)		(PORTE |= (x))
#define SET_PORTF(x)		(PORTF |= (x))
#define SET_PORTG(x)		(PORTG |= (x))
#define SET_PORTH(x)		(PORTH |= (x))

// Reset either a pin, all pins or a combination of pins using '|' operator
#define RESET_PORTA(x)		(PORTA &= ~(x))
#define RESET_PORTB(x)		(PORTB &= ~(x))
#define RESET_PORTC(x)		(PORTC &= ~(x))
#define RESET_PORTD(x)		(PORTD &= ~(x))
#define RESET_PORTE(x)		(PORTE &= ~(x))
#define RESET_PORTF(x)		(PORTF &= ~(x))
#define RESET_PORTG(x)		(PORTG &= ~(x))
#define RESET_PORTH(x)		(PORTH &= ~(x))

// Write either a pin, all pins or a combination of pins with '|' operator
#define WRITE_PORTA(x, y)	((y) == true? (PORTA |= (x) : (PORTA &= ~(x))
#define WRITE_PORTB(x, y)	((y) == true? (PORTB |= (x) : (PORTB &= ~(x))
#define WRITE_PORTC(x, y)	((y) == true? (PORTC |= (x) : (PORTC &= ~(x))
#define WRITE_PORTD(x, y)	((y) == true? (PORTD |= (x) : (PORTD &= ~(x))
#define WRITE_PORTE(x, y)	((y) == true? (PORTE |= (x) : (PORTE &= ~(x))
#define WRITE_PORTF(x, y)	((y) == true? (PORTF |= (x) : (PORTF &= ~(x))
#define WRITE_PORTG(x, y)	((y) == true? (PORTG |= (x) : (PORTG &= ~(x))
#define WRITE_PORTH(x, y)	((y) == true? (PORTH |= (x) : (PORTH &= ~(x))




#endif	/* ATMEGA2560_H */

//------------------------ Scratch Area ---------------------------------------

