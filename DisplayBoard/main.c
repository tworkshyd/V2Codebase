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




int main (void) {
    
    platform_init ();
    
    
    while (1)
    {
            
        // WDT trigger
        
        
        // periodic tasks.. will be called here..
        if (f_10msecs)  {
            f_10msecs = 0;
            platform_10msec_tasks();           
        }
        else if (f_100msecs)    {
            f_100msecs = 0;
            platform_100msec_tasks();
        }
        else if (f_1sec)    {
            f_1sec = 0;
            platform_1sec_tasks();
        }
        else {
            // continuous tasks.. go here..
        }
        
    }
    
}




//------------------------ Scratch Area ---------------------------------------



        // PINC = (1 << PINC7);
        //SET_PORTA (GPIO_PIN_1);
        //BUZZER_CNTRL(1);
        //_delay_ms (1000);
        //RESET_PORTA (GPIO_PIN_1);
        //BUZZER_CNTRL(0);
        //_delay_ms (1000);
        
        // test_leds ();
