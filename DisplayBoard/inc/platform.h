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
 * File			: platform.h 
 * Author		: Firmware Team member
 * Comments		: Header file which contains platform related defines and 
 *              function prototypes.
 * Revision history: 
 *				Created on 11-aug-2021
 */


 
// Guard condition to prevent multiple file inclusion
#ifndef PLATFORM_H
#define	PLATFORM_H

// include processor files - #include <>  -------------------------------------
#include <xc.h> 

// Note: Frequency 'F_CPU' must be set before inclusion of 'delay.h' file
#define F_CPU   16000000

#include <util/delay.h>
#include <xc.h> 
#include<avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>

// include project files - #include "" ----------------------------------------
#include "../inc/config.h" 
#include "../inc/atmega2560.h"
#include "../inc/timer.h"
#include "led.h"
#include "uart.h"


// '#' defines ----------------------------------------------------------------

//----------------------------------------------------------------------------
// 1. GPIO pin declarations
//----------------------------------------------------------------------------

// Port A
// Port assignment
#define BUZZER_PORT		(PORTA)
#define UL0_LED1_PORT   (PORTA)
#define UL1_LED2_PORT   (PORTA)
#define UL2_LED3_PORT   (PORTA)
#define UL3_LED4_PORT   (PORTA)
#define UL4_LED5_PORT   (PORTA)
#define UL5_LED6_PORT   (PORTA)
#define LED_WORD_PORT   (PORTA)

// Pin assignment
#define BUZZER_PIN		(GPIO_PIN_0)
#define UL0_LED1_PIN    (GPIO_PIN_1)
#define UL1_LED2_PIN    (GPIO_PIN_2)
#define UL2_LED3_PIN    (GPIO_PIN_3)
#define UL3_LED4_PIN    (GPIO_PIN_4)
#define UL4_LED5_PIN    (GPIO_PIN_5)
#define UL5_LED6_PIN    (GPIO_PIN_6)
// PINA7 --> NC
#define ALL_LED_PINs    (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)

#define LED_PIN_MASK    ALL_LED_PINs
#define LED_LSB_OFFSET_BITs     (1)     // 6 led's starting from A1 to A6


#define BUZZER_WRITE(x)		((x)? ( BUZZER_PORT  |= BUZZER_PIN  ) : (BUZZER_PORT   &= ~BUZZER_PIN  ))
#define UL0_LED1_WRITE(x)  	((x)? (UL0_LED1_PORT |= UL0_LED1_PIN) : (UL0_LED1_PORT &= ~UL0_LED1_PIN))
#define UL1_LED2_WRITE(x)  	((x)? (UL1_LED2_PORT |= UL1_LED2_PIN) : (UL1_LED2_PORT &= ~UL1_LED2_PIN))
#define UL2_LED3_WRITE(x)  	((x)? (UL2_LED3_PORT |= UL2_LED3_PIN) : (UL2_LED3_PORT &= ~UL2_LED3_PIN))
#define UL3_LED4_WRITE(x)  	((x)? (UL3_LED4_PORT |= UL3_LED4_PIN) : (UL3_LED4_PORT &= ~UL3_LED4_PIN))
#define UL4_LED5_WRITE(x)  	((x)? (UL4_LED5_PORT |= UL4_LED5_PIN) : (UL4_LED5_PORT &= ~UL4_LED5_PIN))
#define UL5_LED6_WRITE(x)  	((x)? (UL5_LED6_PORT |= UL5_LED6_PIN) : (UL5_LED6_PORT &= ~UL5_LED6_PIN))
// Handle all LEDs at once
#define LED_WORD_WRITE(x)  	(LED_WORD_PORT &= ~LED_PIN_MASK); (LED_WORD_PORT |= ((x << LED_LSB_OFFSET_BITs) & LED_PIN_MASK))


// temp		
 #define TEST_LED     UL5_LED6_PIN 



// Port B
#define CS_DIGIPOT_PIN	(GPIO_PIN_0)
// SCK Prog    
// MOSI Prog  
// MISO Prog 
#define LCD_DB4_PIN     (GPIO_PIN_4)
#define LCD_DB5_PIN     (GPIO_PIN_5)
#define LCD_DB6_PIN     (GPIO_PIN_6)
#define LCD_DB7_PIN     (GPIO_PIN_7)

// Port C
// ALL pins are NCs

// Port D
// I2C_SCL
// I2C_SDA
#define INTA_ROTENC_PIN (GPIO_PIN_2)
#define INTB_ROTENC_PIN (GPIO_PIN_3)
// NC
// NC
// NC
#define RDY_EXT_PIN     (GPIO_PIN_7)

// Port E
// NC
// NC
// NC
// NC
#define START_BUTTON_PIN        (GPIO_PIN_4)
#define BUTTON_ROTENC_PIN       (GPIO_PIN_5)
// NC
// NC

// Port F
#define AIN_POT0_ADC0()         DIGITAL_INPUT_DISABLE_A7_A0(GPIO_PIN_0)
#define AIN_POT1_ADC1()         DIGITAL_INPUT_DISABLE_A7_A0(GPIO_PIN_1)
#define AIN_POT2_ADC2()         DIGITAL_INPUT_DISABLE_A7_A0(GPIO_PIN_2)
#define AIN_POT3_ADC3()         DIGITAL_INPUT_DISABLE_A7_A0(GPIO_PIN_3)
// JTAG TCK
// JTAG TMS
// JTAG TDO
// JTAG TDI

// Port G
#define P1_SWITCH_PIN           (GPIO_PIN_0)
// PG1 - NC
// PG2 - NC
// PG3 - NC
// PG4 - NC
// PG5 - NC

// Port H
#define RXD_PROG_DBG            (GPIO_PIN_0)
#define TXD_PROG_DBG            (GPIO_PIN_1)
// PH2 - NC
// PH3 - NC
// PH4 - NC
// PH5 - NC
// PH6 - NC
// PH7 - NC


// Port J
#define TXD_TTL_COMM            (GPIO_PIN_0)
#define RXD_TTL_COMM            (GPIO_PIN_1)
// PJ2 - NC
// PJ3 - NC
// PJ4 - NC
// PJ5 - NC
// PJ6 - NC
// PJ7 - NC

// Port K
#define AIN_POT4_ADC8()         DIGITAL_INPUT_DISABLE_A15_A8(GPIO_PIN_0)
// PK1 - NC
#define AIN_VCC5V_ADC10()       DIGITAL_INPUT_DISABLE_A15_A8(GPIO_PIN_2)
// PK3 - NC
// PK4 - NC
// PK5 - NC
// PK6 - NC
// PK7 - NC
#define RDY_ADS_I2C	        	(GPIO_PIN_7)

// Port L
// PL0 - NC
// PL1 - NC
// PL2 - NC
// PL3 - NC
// PL4 - NC
#define LCD_BCK_LIGHT_PIN		(GPIO_PIN_5)
#define LCD_RS_PIN				(GPIO_PIN_6)
#define LCD_EN_PIN				(GPIO_PIN_7)



//----------------------------------------------------------------------------
// 2. GPIO Pins which differ between the PCB revisions are defined selectively 
//		in this section
//----------------------------------------------------------------------------

// Display Board PCB Rev-1.0
#if   DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_1_0
    // todo// todo
    #error "Note yet coded..!!"

// Display Board PCB Rev-2.1
#elif DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_2_1
    // todo
    #error "Note yet coded..!!"

// Display Board PCB Rev-2.2 (Dated : 16-May-2021)
#elif DISPLAY_BOARD_Rev == DISPLAY_BOARD_Rev_2_2


#else 
#error "Board NOT selected..!!"
#endif


// 'Macros' -------------------------------------------------------------------
#define  BUZZER_CNTRL(x)    BUZZER_WRITE(BUZZER_PIN, x)
#define  LED_CNTRL(x, y)    LED_WORD_WRITE(x, y)
#define  LED_GROUP_CNTRL(x) LED_WORD_WRITE(x)

#define  SYSTEM_RUNNING()   (LED_WORD_PORT ^= SYSTEM_LED)


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
void platform_init  (void);
void platform_1msec_tasks   (void);
void platform_10msec_tasks  (void);
void platform_100msec_tasks (void);
void platform_1sec_tasks    (void);
void platform_1min_tasks    (void);
void platform_1hr_tasks     (void);
void platform_1day_tasks    (void);
void platform_1month_tasks  (void);
void platform_1year_tasks   (void);



#endif	/* PLATFORM_H */

//------------------------ Scratch Area ---------------------------------------


