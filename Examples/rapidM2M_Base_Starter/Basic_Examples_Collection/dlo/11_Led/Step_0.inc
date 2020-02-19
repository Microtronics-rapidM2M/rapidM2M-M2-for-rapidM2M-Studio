/**
 *                  _     _  __  __ ___  __  __
 *                 (_)   | ||  \/  |__ \|  \/  |
 *  _ __ __ _ _ __  _  __| || \  / |  ) | \  / |
 * | '__/ _` | '_ \| |/ _` || |\/| | / /| |\/| |
 * | | | (_| | |_) | | (_| || |  | |/ /_| |  | |
 * |_|  \__,_| .__/|_|\__,_||_|  |_|____|_|  |_|
 *           | |
 *           |_|
 *
 * Simple "LED" Example
 *
 * Toggles LED2 every second and turns off all other LEDs
 *
 * Only compatible with rapidM2M M2xx and rapidM2M M3
 * Special hardware circuit necessary
 * Recommendation: use rapidM2M Base Starter
 *
 * @version 20200219
 */

/* Pin configuration */
const
{
	PIN_LED1_R = 1,														// RGB LED 1: Red		(GPIO1 is used)
	PIN_LED1_G = 2,														// RGB LED 1: Green	(GPIO2 is used)
	PIN_LED1_B = 3,														// RGB LED 1: Blue	(GPIO3 is used)
	PIN_LED2   = 0,														// LED 2: Green			(GPIO0 is used)
	PIN_LED3   = 4,														// LED 3: Green			(GPIO4 is used)

	LED_ENABLE  = RM2M_GPIO_HIGH,							// By setting the GPIO to "high" the LED is turned on
	LED_DISABLE = RM2M_GPIO_LOW,							// By setting the GPIO to "low" the LED is turned off
};

/* Global variables declarations */
static iLedState;														// Current state of LED 2 (1=^ On; x=^Off)

/* 1 sec. timer is used for the general program sequence */
#callback MainTimer()
{
	if(iLedState)															// If LED 2 is currently "On" ->
	{
		digitalWrite(PIN_LED2, LED_DISABLE);		// Turns off LED 2

		#log("[LED] off");
	}
	else																			// Otherwise (i.e. LED 2 is off)
	{
		digitalWrite(PIN_LED2, LED_ENABLE);				// Turns on LED 2

		#log("[LED] on");
	}

	// Change state of LED
	iLedState = !iLedState;										// Toggles the variable which holds the current state of LED 2
}

/* Application entry point */
main()
{
	/* Initialisation of a cyclic 1 sec. timer */
	setInterval(MainTimer, 1000);

	/* Sets signal direction for GPIOs used to control LEDs to "Output" and turns off all LEDs */
	digitalWrite(PIN_LED1_R, LED_DISABLE);
	digitalWrite(PIN_LED1_G, LED_DISABLE);
	digitalWrite(PIN_LED1_B, LED_DISABLE);
	digitalWrite(PIN_LED2, LED_DISABLE);
	digitalWrite(PIN_LED3, LED_DISABLE);
}