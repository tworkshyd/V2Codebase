/**
* \file
*
* \brief User board configuration template
*
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

#ifndef CONF_BOARD_H
#define CONF_BOARD_H



#define BUZZER		IOPORT_CREATE_PIN(PORTA, 0)
#define LED_1_UL0	IOPORT_CREATE_PIN(PORTA, 1)
#define LED_2_UL1	IOPORT_CREATE_PIN(PORTA, 2)
#define LED_3_UL2	IOPORT_CREATE_PIN(PORTA, 3)
#define LED_4_UL3	IOPORT_CREATE_PIN(PORTA, 4)
#define LED_5_UL4	IOPORT_CREATE_PIN(PORTA, 5)
#define LED_6_UL5	IOPORT_CREATE_PIN(PORTA, 6)


#endif // CONF_BOARD_H
