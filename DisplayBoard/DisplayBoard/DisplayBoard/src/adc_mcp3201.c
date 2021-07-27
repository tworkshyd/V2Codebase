/**
 * \file
 *
 * \brief ADC MCP3201 driver.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \addtogroup doc_driver_adc_mcp3201
 *
 * \section doc_driver_adc_mcp3201_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <adc_mcp3201.h>
#include "atmel_start_pins.h"
#include "util/delay.h"

#define POWER_UP_DELAY 300    // us
#define ADCResult_Mask 0x0FFF // 12 bit masking

void MCP3201_select(void);
void MCP3201_deselect(void);

void ADC_1_init()
{

	/* Enable SPI */
	PRR0 &= ~(1 << PRSPI);

	SPCR = 1 << SPE                     /* SPI module enable: enabled */
	       | 0 << DORD                  /* Data order: disabled */
	       | 1 << MSTR                  /* Master/Slave select: enabled */
	       | 0 << CPOL                  /* Clock polarity: disabled */
	       | 0 << CPHA                  /* Clock phase: disabled */
	       | 0 << SPIE                  /* SPI interrupt enable: disabled */
	       | (0 << SPR1) | (0 << SPR0); /* SPI Clock rate selection: fosc/4 */

	// SPSR = (0 << SPI2X); /* Disable double SPI speed */
}

void ADC_1_cs_low()
{
	MCP3201_select();
}

void ADC_1_cs_high()
{
	MCP3201_deselect();
}

void ADC_1_start_conversion(void)
{
	ADC_1_cs_low();

	_delay_us(POWER_UP_DELAY);
	while (PB3_get_level() == 0) {
	}

	ADC_1_cs_high();
}

void ADC_1_stop_conversion(void)
{
	ADC_1_cs_high();
}

uint16_t ADC_1_get_conversion_result(void)
{
	uint8_t  readData[2];
	uint16_t ADCResult;

	ADC_1_cs_low();

	for (uint8_t i = 0; i < 2; i++) {
		SPDR = 0;
		while (!(SPSR & (1 << SPIF)))
			;

		readData[i] = SPDR;
	}

	ADC_1_stop_conversion();

	ADCResult = readData[0];
	ADCResult = (ADCResult << 8) | readData[1];
	ADCResult = (ADCResult >> 1) & ADCResult_Mask;

	return ADCResult;
}
