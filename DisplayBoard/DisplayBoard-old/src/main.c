/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* Bare minimum empty user application template
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# "Insert system clock initialization code here" comment
* -# Minimal main function that starts with a call to board_init()
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/
#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	
	volatile unsigned long  freq;
	
	freq = sysclk_get_main_hz();
	freq = sysclk_get_source_clock_hz ();

	freq += 10;

	board_init();
	
	/* Insert application code here, after the board has been initialized. */

	int value = 0;
	
	value = freq;

	while (1)
	{
		value = ~value;
//  		ioport_set_pin_level(BUZZER,	 value);
		ioport_set_pin_level(LED_1_UL0,	 ~value);
		delay_us(1);
		ioport_set_pin_level(LED_1_UL0,	 value);
		delay_us(5);
		
// 		delay_us(10);
// 		ioport_set_pin_level(LED_2_UL1,	 ~value);
// 		delay_us(100);
// 		ioport_set_pin_level(LED_3_UL2,	 value);
// 		delay_ms(1);
// 		ioport_set_pin_level(LED_4_UL3,	 ~value);
// 		delay_ms(10);
// 		ioport_set_pin_level(LED_5_UL4,	 value);
// 		delay_ms(100);
// 		ioport_set_pin_level(LED_6_UL5,	 ~value);
// 		delay_ms(1000);
		
		
	}

}
