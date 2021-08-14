/*
 * File:   main.c
 * Author: abdul
 *
 * Created on 9 August, 2021, 5:04 PM
 */


#include <xc.h> 
#include <avr/io.h>

#include "./inc/platform.h"
#include "./inc/system_tests.h"





int main(void) {
    
    platform_init ();
    
    // DDRC = (1 << DDC7);
    
    
    while (1)
    {
        // PINC = (1 << PINC7);
        //SET_PORTA (GPIO_PIN_1);
        //BUZZER_CNTRL(1);
        //_delay_ms (1000);
        //RESET_PORTA (GPIO_PIN_1);
        //BUZZER_CNTRL(0);
        //_delay_ms (1000);
        
        test_leds ();
    }
    
}
