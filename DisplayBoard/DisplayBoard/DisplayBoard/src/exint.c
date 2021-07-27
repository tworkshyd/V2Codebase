/**
 * \file
 *
 * \brief USART related functionality implementation.
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
 */

/**
 * \addtogroup doc_driver_exint
 *
 * \section doc_driver_exint_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <exint.h>

/**
 * \brief Initialize EXTERNAL_IRQ_0 interface
 *
 * \return Initialization status.
 */
int8_t EXTERNAL_IRQ_0_init()
{

	EICRA = (0 << ISC21) | (0 << ISC20) | // The low level of ISC2 generates an interrupt request
	        (0 << ISC31) | (0 << ISC30);  // The low level of ISC3 generates an interrupt request

	EICRB = (0 << ISC41) | (0 << ISC40) | // The low level of ISC4 generates an interrupt request
	        (0 << ISC51) | (0 << ISC50);  // The low level of ISC5 generates an interrupt request

	EIMSK = (1 << INT2) | // Enable external interrupt request 2
	        (1 << INT3) | // Enable external interrupt request 3
	        (1 << INT4) | // Enable external interrupt request 4
	        (1 << INT5);  // Enable external interrupt request 5

	PCICR = (1 << PCIE2); // Enable pin change interrupt 2

	PCMSK2 = (1 << PCINT23); // Pin change enable mask 23

	return 0;
}
