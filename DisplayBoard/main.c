/*
 * File:   main.c
 * Author: abdul
 *
 * Created on 9 August, 2021, 5:04 PM
 */


#include <xc.h> 
#include <avr/io.h>

//#define F_CPU   16000000
#define F_CPU   8000000

#include <util/delay.h>



#include ".\inc\atmega2560.h"


void platform_init (void)   {
    
    DIR_OUTPUT_PORTA(GPIO_PIN_ALL);
    
}


int main(void) {
    
    platform_init ();
    
    // DDRC = (1 << DDC7);
    
    
    while (1)
    {
        // PINC = (1 << PINC7);
        SET_PORTA (GPIO_PIN_1);
        //_delay_ms (1000);
        RESET_PORTA (GPIO_PIN_1);
        //_delay_ms (1000);
    }
    
}
