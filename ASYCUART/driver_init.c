/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

/*! The buffer size for USART */
#define SERIAL_BUFFER_SIZE 16

struct usart_async_descriptor SERIAL;

static uint8_t SERIAL_buffer[SERIAL_BUFFER_SIZE];

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void SERIAL_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_FLEXCOM7);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void SERIAL_PORT_init()
{

	gpio_set_pin_function(PA27, MUX_PA27B_FLEXCOM7_RXD);

	gpio_set_pin_function(PA28, MUX_PA28B_FLEXCOM7_TXD);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void SERIAL_init(void)
{
	SERIAL_CLOCK_init();
	SERIAL_PORT_init();
	usart_async_init(&SERIAL, FLEXCOM7, SERIAL_buffer, SERIAL_BUFFER_SIZE, _usart_get_usart_async());
}

void system_init(void)
{
	init_mcu();

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	SERIAL_init();
}
