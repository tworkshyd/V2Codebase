#include <atmel_start.h>

int main(void)
{
	
	volatile int value = 0;
	
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	while (1)
	{
		value = ~value;
		UL3_LED4_set_level(
		// <y> Initial level
		// <id> pad_initial_level
		// <false"> Low
		// <true"> High
		value);
	}
}
