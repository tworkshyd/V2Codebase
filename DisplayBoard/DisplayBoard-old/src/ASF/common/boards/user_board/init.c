/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>




void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	int		value = 0;
	
	ioport_set_pin_dir(BUZZER,		IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_1_UL0,	IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_2_UL1,	IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_3_UL2,	IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_4_UL3,	IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_5_UL4,	IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED_6_UL5,	IOPORT_DIR_OUTPUT);
	
	
	ioport_set_pin_level(BUZZER,	 value);
	ioport_set_pin_level(LED_1_UL0,	~value);
	ioport_set_pin_level(LED_2_UL1,	 value);
	ioport_set_pin_level(LED_3_UL2,	~value);
	ioport_set_pin_level(LED_4_UL3,	 value);
	ioport_set_pin_level(LED_5_UL4,	~value);
	ioport_set_pin_level(LED_6_UL5,	 value);
						 
	
	
}
