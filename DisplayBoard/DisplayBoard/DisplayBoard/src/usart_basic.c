/**
 * \file
 *
 * \brief USART basic driver.
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
 * \defgroup doc_driver_usart_basic USART Basic
 * \ingroup doc_driver_usart
 *
 * \section doc_driver_usart_basic_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <compiler.h>
#include <clock_config.h>
#include <usart_basic.h>
#include <atomic.h>

#include <stdio.h>

#if defined(__GNUC__)

int USART_0_printCHAR(char character, FILE *stream)
{
	USART_0_write(character);
	return 0;
}

FILE USART_0_stream = FDEV_SETUP_STREAM(USART_0_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
	USART_0_write(outChar);
	return outChar;
}
#endif

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
int8_t USART_0_init()
{

	// Module is in UART mode

	/* Enable USART2 */
	PRR1 &= ~(1 << PRUSART2);

#define BAUD 115200

#include <utils/setbaud.h>

	UBRR2H = UBRRH_VALUE;
	UBRR2L = UBRRL_VALUE;

	UCSR2A = USE_2X << U2X2 /*  */
	         | 0 << MPCM2;  /* Multi-processor Communication Mode: disabled */

	UCSR2B = 1 << RXCIE2    /* RX Complete Interrupt Enable: enabled */
	         | 0 << UDRIE2  /* USART Data Register Empty Interupt Enable: disabled */
	         | 1 << RXEN2   /* Receiver Enable: enabled */
	         | 1 << TXEN2   /* Transmitter Enable: enabled */
	         | 0 << UCSZ22; /*  */

	// UCSR2C = (0 << UMSEL21) | (0 << UMSEL20) /*  */
	//		 | (0 << UPM21) | (0 << UPM20) /* Disabled */
	//		 | 0 << USBS2 /* USART Stop Bit Select: disabled */
	//		 | (1 << UCSZ21) | (1 << UCSZ20); /* 8-bit */

#if defined(__GNUC__)
	stdout = &USART_0_stream;
#endif

	return 0;
}

/**
 * \brief Enable RX and TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable()
{
	UCSR2B |= ((1 << TXEN2) | (1 << RXEN2));
}

/**
 * \brief Enable RX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_rx()
{
	UCSR2B |= (1 << RXEN2);
}

/**
 * \brief Enable TX in USART_0
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_0_enable_tx()
{
	UCSR2B |= (1 << TXEN2);
}

/**
 * \brief Disable USART_0
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void USART_0_disable()
{
	UCSR2B &= ~((1 << TXEN2) | (1 << RXEN2));
}

/**
 * \brief Get recieved data from USART_0
 *
 * \return Data register from USART_0 module
 */
uint8_t USART_0_get_data()
{
	return UDR2;
}

/**
 * \brief Check if the usart can accept data to be transmitted
 *
 * \return The status of USART TX data ready check
 * \retval false The USART can not receive data to be transmitted
 * \retval true The USART can receive data to be transmitted
 */
bool USART_0_is_tx_ready()
{
	return (UCSR2A & (1 << UDRE2));
}

/**
 * \brief Check if the USART has received data
 *
 * \return The status of USART RX data ready check
 * \retval true The USART has received data
 * \retval false The USART has not received data
 */
bool USART_0_is_rx_ready()
{
	return (UCSR2A & (1 << RXC2));
}

/**
 * \brief Check if USART_0 data is transmitted
 *
 * \return Receiver ready status
 * \retval true  Data is not completely shifted out of the shift register
 * \retval false Data completely shifted out if the USART shift register
 */
bool USART_0_is_tx_busy()
{
	return (!(UCSR2A & (1 << TXC2)));
}

/**
 * \brief Read one character from USART_0
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART_0 module
 */
uint8_t USART_0_read()
{
	while (!(UCSR2A & (1 << RXC2)))
		;
	return UDR2;
}

/**
 * \brief Write one character to USART_0
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */
void USART_0_write(const uint8_t data)
{
	while (!(UCSR2A & (1 << UDRE2)))
		;
	UDR2 = data;
}

#include <stdio.h>

#if defined(__GNUC__)

int USART_1_printCHAR(char character, FILE *stream)
{
	USART_1_write(character);
	return 0;
}

FILE USART_1_stream = FDEV_SETUP_STREAM(USART_1_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
	USART_0_write(outChar);
	return outChar;
}
#endif

/* Static Variables holding the ringbuffer used in IRQ mode */
static uint8_t          USART_1_rxbuf[USART_1_RX_BUFFER_SIZE];
static volatile uint8_t USART_1_rx_head;
static volatile uint8_t USART_1_rx_tail;
static volatile uint8_t USART_1_rx_elements;
static uint8_t          USART_1_txbuf[USART_1_TX_BUFFER_SIZE];
static volatile uint8_t USART_1_tx_head;
static volatile uint8_t USART_1_tx_tail;
static volatile uint8_t USART_1_tx_elements;

void USART_1_default_rx_isr_cb(void);
void (*USART_1_rx_isr_cb)(void) = &USART_1_default_rx_isr_cb;
void USART_1_default_udre_isr_cb(void);
void (*USART_1_udre_isr_cb)(void) = &USART_1_default_udre_isr_cb;

void USART_1_default_rx_isr_cb(void)
{
	uint8_t data;
	uint8_t tmphead;

	/* Read the received data */
	data = UDR3;
	/* Calculate buffer index */
	tmphead = (USART_1_rx_head + 1) & USART_1_RX_BUFFER_MASK;

	if (tmphead == USART_1_rx_tail) {
		/* ERROR! Receive buffer overflow */
	} else {
		/* Store new index */
		USART_1_rx_head = tmphead;

		/* Store received data in buffer */
		USART_1_rxbuf[tmphead] = data;
		USART_1_rx_elements++;
	}
}

void USART_1_default_udre_isr_cb(void)
{
	uint8_t tmptail;

	/* Check if all data is transmitted */
	if (USART_1_tx_elements != 0) {
		/* Calculate buffer index */
		tmptail = (USART_1_tx_tail + 1) & USART_1_TX_BUFFER_MASK;
		/* Store new index */
		USART_1_tx_tail = tmptail;
		/* Start transmission */
		UDR3 = USART_1_txbuf[tmptail];
		USART_1_tx_elements--;
	}

	if (USART_1_tx_elements == 0) {
		/* Disable UDRE interrupt */
		UCSR3B &= ~(1 << UDRIE3);
	}
}

/**
 * \brief Set call back function for USART_1
 *
 * \param[in] cb The call back function to set
 *
 * \param[in] type The type of ISR to be set
 *
 * \return Nothing
 */
void USART_1_set_ISR_cb(usart_cb_t cb, usart_cb_type_t type)
{
	switch (type) {
	case RX_CB:
		USART_1_rx_isr_cb = cb;
		break;
	case UDRE_CB:
		USART_1_udre_isr_cb = cb;
		break;
	default:
		// do nothing
		break;
	}
}

/* Interrupt service routine for RX complete */
ISR(USART3_RX_vect)
{
	if (USART_1_rx_isr_cb != NULL)
		(*USART_1_rx_isr_cb)();
}

/* Interrupt service routine for Data Register Empty */
ISR(USART3_UDRE_vect)
{
	if (USART_1_udre_isr_cb != NULL)
		(*USART_1_udre_isr_cb)();
}

bool USART_1_is_tx_ready()
{
	return (USART_1_tx_elements != USART_1_TX_BUFFER_SIZE);
}

bool USART_1_is_rx_ready()
{
	return (USART_1_rx_elements != 0);
}

bool USART_1_is_tx_busy()
{
	return (!(UCSR3A & (1 << TXC3)));
}

/**
 * \brief Read one character from USART_1
 *
 * Function will block if a character is not available.
 *
 * \return Data read from the USART_1 module
 */
uint8_t USART_1_read(void)
{
	uint8_t tmptail;

	/* Wait for incoming data */
	while (USART_1_rx_elements == 0)
		;
	/* Calculate buffer index */
	tmptail = (USART_1_rx_tail + 1) & USART_1_RX_BUFFER_MASK;
	/* Store new index */
	USART_1_rx_tail = tmptail;
	ENTER_CRITICAL(R);
	USART_1_rx_elements--;
	EXIT_CRITICAL(R);

	/* Return data */
	return USART_1_rxbuf[tmptail];
}

/**
 * \brief Write one character to USART_1
 *
 * Function will block until a character can be accepted.
 *
 * \param[in] data The character to write to the USART
 *
 * \return Nothing
 */
void USART_1_write(const uint8_t data)
{
	uint8_t tmphead;

	/* Calculate buffer index */
	tmphead = (USART_1_tx_head + 1) & USART_1_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (USART_1_tx_elements == USART_1_TX_BUFFER_SIZE)
		;
	/* Store data in buffer */
	USART_1_txbuf[tmphead] = data;
	/* Store new index */
	USART_1_tx_head = tmphead;
	ENTER_CRITICAL(W);
	USART_1_tx_elements++;
	EXIT_CRITICAL(W);
	/* Enable UDRE interrupt */
	UCSR3B |= (1 << UDRIE3);
}

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
int8_t USART_1_init()
{

	// Module is in UART mode

	/* Enable USART3 */
	PRR1 &= ~(1 << PRUSART3);

#define BAUD 115200

#include <utils/setbaud.h>

	UBRR3H = UBRRH_VALUE;
	UBRR3L = UBRRL_VALUE;

	UCSR3A = USE_2X << U2X3 /*  */
	         | 0 << MPCM3;  /* Multi-processor Communication Mode: disabled */

	UCSR3B = 1 << RXCIE3    /* RX Complete Interrupt Enable: enabled */
	         | 0 << UDRIE3  /* USART Data Register Empty Interupt Enable: disabled */
	         | 1 << RXEN3   /* Receiver Enable: enabled */
	         | 1 << TXEN3   /* Transmitter Enable: enabled */
	         | 0 << UCSZ32; /*  */

	// UCSR3C = (0 << UMSEL31) | (0 << UMSEL30) /*  */
	//		 | (0 << UPM31) | (0 << UPM30) /* Disabled */
	//		 | 0 << USBS3 /* USART Stop Bit Select: disabled */
	//		 | (1 << UCSZ31) | (1 << UCSZ30); /* 8-bit */

	uint8_t x;

	/* Initialize ringbuffers */
	x = 0;

	USART_1_rx_tail     = x;
	USART_1_rx_head     = x;
	USART_1_rx_elements = x;
	USART_1_tx_tail     = x;
	USART_1_tx_head     = x;
	USART_1_tx_elements = x;

#if defined(__GNUC__)
	stdout = &USART_1_stream;
#endif

	return 0;
}

/**
 * \brief Enable RX and TX in USART_1
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX and TX enable-bits in the USART control register
 *
 * \return Nothing
 */
void USART_1_enable()
{
	UCSR3B |= ((1 << TXEN3) | (1 << RXEN3));
}

/**
 * \brief Enable RX in USART_1
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the RX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_1_enable_rx()
{
	UCSR3B |= (1 << RXEN3);
}

/**
 * \brief Enable TX in USART_1
 * 1. If supported by the clock system, enables the clock to the USART
 * 2. Enables the USART module by setting the TX enable-bit in the USART control register
 *
 * \return Nothing
 */
void USART_1_enable_tx()
{
	UCSR3B |= (1 << TXEN3);
}

/**
 * \brief Disable USART_1
 * 1. Disables the USART module by clearing the enable-bit(s) in the USART control register
 * 2. If supported by the clock system, disables the clock to the USART
 *
 * \return Nothing
 */
void USART_1_disable()
{
	UCSR3B &= ~((1 << TXEN3) | (1 << RXEN3));
}

/**
 * \brief Get recieved data from USART_1
 *
 * \return Data register from USART_1 module
 */
uint8_t USART_1_get_data()
{
	return UDR3;
}
