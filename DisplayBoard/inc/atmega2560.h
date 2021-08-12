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
#define DIR_OUTPUT_PORTJ(x)		(DDRJ |= (x))
#define DIR_OUTPUT_PORTK(x)		(DDRK |= (x))
#define DIR_OUTPUT_PORTL(x)		(DDRL |= (x))

// Set to 'In-put' either a pin / all pins / a combination of pins using '|' 
#define DIR_INPUT_PORTA(x)		(DDRA &= ~(x))
#define DIR_INPUT_PORTB(x)		(DDRB &= ~(x))
#define DIR_INPUT_PORTC(x)		(DDRC &= ~(x))
#define DIR_INPUT_PORTD(x)		(DDRD &= ~(x))
#define DIR_INPUT_PORTE(x)		(DDRE &= ~(x))
#define DIR_INPUT_PORTF(x)		(DDRF &= ~(x))
#define DIR_INPUT_PORTG(x)		(DDRG &= ~(x))
#define DIR_INPUT_PORTH(x)		(DDRH &= ~(x))
#define DIR_INPUT_PORTJ(x)		(DDRJ &= ~(x))
#define DIR_INPUT_PORTK(x)		(DDRK &= ~(x))
#define DIR_INPUT_PORTL(x)		(DDRL &= ~(x))


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
#define WRITE_PORTA(x, y)	((y) == 0? (PORTA &= ~(x)) : (PORTA |= (x)))
#define WRITE_PORTB(x, y)	((y) == 0? (PORTB &= ~(x)) : (PORTB |= (x)))
#define WRITE_PORTC(x, y)	((y) == 0? (PORTC &= ~(x)) : (PORTC |= (x)))
#define WRITE_PORTD(x, y)	((y) == 0? (PORTD &= ~(x)) : (PORTD |= (x)))
#define WRITE_PORTE(x, y)	((y) == 0? (PORTE &= ~(x)) : (PORTE |= (x)))
#define WRITE_PORTF(x, y)	((y) == 0? (PORTF &= ~(x)) : (PORTF |= (x)))
#define WRITE_PORTG(x, y)	((y) == 0? (PORTG &= ~(x)) : (PORTG |= (x)))
#define WRITE_PORTH(x, y)	((y) == 0? (PORTH &= ~(x)) : (PORTH |= (x)))



// ADC control Macros 
// 1. (26.8.1) ADMUX ? ADC Multiplexer Selection Register [0x7C]
#define	ADC_VREF_TURN_OFF()			(ADMUX &= ~(1 << REFS1 | 1 << REFS0))
	// todo-remaining combinations
#define ADC_RIGHT_ADJUST_RESULT()	(ADMUX &= ~(1 << ADLAR))
#define ADC_LEFT_ADJUST_RESULT()	(ADMUX |=  (1 << ADLAR))
	// remaining bits are handled along with MUX05 bit.. 
	
// 2. (26.8.2) ADCSRB ? ADC Control and Status Register B [0x7B]
//   i. ADC single Ended inputs selection
#define ADC_SELECT_SEI_ADC0()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x00)
#define ADC_SELECT_SEI_ADC1()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x01)
#define ADC_SELECT_SEI_ADC2()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x02)
#define ADC_SELECT_SEI_ADC3()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x03)
#define ADC_SELECT_SEI_ADC4()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x04)
#define ADC_SELECT_SEI_ADC5()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x05)
#define ADC_SELECT_SEI_ADC6()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x06)
#define ADC_SELECT_SEI_ADC7()        	(ADCSRB &=  ~(1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x07)
#define ADC_SELECT_SEI_ADC8()        	(ADCSRB  |=  (1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x00)
#define ADC_SELECT_SEI_ADC9()        	(ADCSRB  |=  (1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x01)	
#define ADC_SELECT_SEI_ADC10()        	(ADCSRB  |=  (1 << MUX5);	\
										 (ADMUX   = ~(ADMUX & 0x1F) | 0x02)	
		// todo-remaining combinations									
//	 ii. ADC Differential inputs selection
		// todo

// 3. (26.8.3) ADCSRA ? ADC Control and Status Register A [0x7A]]
#define ENABLE_ADC ()               (ADCSRA |=  (1 << ADEN))
#define DISABLE_ADC()               (ADCSRA &= ~(1 << ADEN))
#define ADC_START_CONVER()          (ADCSRA |=  (1 << ADSC))
#define ADC_STOP_CONVER()           (ADCSRA &= ~(1 << ADSC))
#define ADC_AUTO_TRIGGR_ENABLE()    (ADCSRA |=  (1 << ADATE))
#define ADC_AUTO_TRIGGR_DISABLE()   (ADCSRA &= ~(1 << ADATE))
#define ADC_INTERRUPT_FLAG_STATUS() (ADCSRA &   (1 << ADIF))
#define ADC_INTERRUPT_ENABLE()      (ADCSRA |=  (1 << ADIE))
#define ADC_INTERRUPT_DISABLE()     (ADCSRA &= ~(1 << ADIE))
// For pre-scaler need to set ADPS2:0 bits in ADCSRA
#define ADC_PRE_SCALER_DIV2()       (ADCSRA = ((ADCSRA & 0x07) | 0x01))
#define ADC_PRE_SCALER_DIV4()       (ADCSRA = ((ADCSRA & 0x07) | 0x02))
#define ADC_PRE_SCALER_DIV8()       (ADCSRA = ((ADCSRA & 0x07) | 0x03))
#define ADC_PRE_SCALER_DIV16()      (ADCSRA = ((ADCSRA & 0x07) | 0x04))
#define ADC_PRE_SCALER_DIV32()      (ADCSRA = ((ADCSRA & 0x07) | 0x05))
#define ADC_PRE_SCALER_DIV64()      (ADCSRA = ((ADCSRA & 0x07) | 0x06))
#define ADC_PRE_SCALER_DIV128()     (ADCSRA = ((ADCSRA & 0x07) | 0x07))

// 4. (26.8.4) ADCL and ADCH ? The ADC Data Register [0x79 & 0x78]
#define ADC_DATA_REG_LOW()			(ADCL)
#define ADC_DATA_REG_HIGH()			(ADCH)

// 5. (26.8.5) ADCSRB ? ADC Control and Status Register B [0X7B]
// • Bit 2:0 – ADTS2:0: ADC Auto Trigger Source
#define ADC_TRIGG_FREE_RUNNG()		(ADCSRB = ((ADCSRB & 0x07) | 0x00))
#define ADC_TRIGG_ANALOG_COMP()		(ADCSRB = ((ADCSRB & 0x07) | 0x01))
#define ADC_TRIGG_EXTI_REQUEST()	(ADCSRB = ((ADCSRB & 0x07) | 0x02))
// remaining todo

// 6. (26.8.6) DIDR0 ? Digital Input Disable Register 0   [0x7E]]
// to set as ADC pins from ADC0 to ADC7, where 'x' could be 0x01 - 0xFF
#define DIGITAL_INPUT_DISABLE_A7_A0(x)      (DIDR0 = x)		

// 7. (26.8.7) DIDR2 ? Digital Input Disable Register 2   [0x7D]
// to set as ADC pins from ADC8 to ADC15, where 'x' could be 0x01 - 0xFF
#define DIGITAL_INPUT_DISABLE_A15_A8(x)     (DIDR2 = x)		




#endif	/* ATMEGA2560_H */

//------------------------ Scratch Area ---------------------------------------

