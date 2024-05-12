/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 *  EXAMPLE: led Blink
 *
 *
 */

#include "pico/stdlib.h"

#define HAL_DELAY_BLINK_LED 500 
#define GPIO_PIN_LED_ON		1
#define GPIO_PIN_LED_OFF	0

int main(void) {

#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
	// Defino
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

	// Inicializa el pin 25 del gpio
    gpio_init(LED_PIN);

	// Configura el pin 25 como salida
    gpio_set_dir(LED_PIN, GPIO_OUT);

	// buble infinito del blink led on board
    while (true) {
            
        // set gpio_pin_led => "0"
        gpio_put(LED_PIN, GPIO_PIN_LED_OFF);
        
        // wait N ms
        sleep_ms(HAL_DELAY_BLINK_LED);

		// set gpio_pin_led => "0"
        gpio_put(LED_PIN, GPIO_PIN_LED_ON);
        
		// wait  N ms        
        sleep_ms(HAL_DELAY_BLINK_LED);
    }
#endif

	return 0;
}


