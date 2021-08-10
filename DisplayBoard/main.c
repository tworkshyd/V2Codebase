/*
 * File:   main.c
 * Author: abdul
 *
 * Created on 9 August, 2021, 5:04 PM
 */


#include <xc.h>
#include <avr/io.h>

#define F_CPU   1000000

#include <util/delay.h>

void main(void) {
    
    DDRC = (1 << DDC7);
    
    
    while (1)
    {
        PINC = (1 << PINC7);
        _delay_ms (1000);
    }
    
}
