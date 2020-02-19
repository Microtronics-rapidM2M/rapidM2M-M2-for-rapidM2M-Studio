  
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
 * Extended "LED" Example
 *
 * Toggles LED2 and LED3 every second and turns off all other LEDs.
 * If LED2 is on, then LED3 is off and vice versa.
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
static iLedState;														// Current LED state ( 	1=^ LED2 "On" and LED3 "Off"
																						//											x=^ LED2 "Off" and LED3 "On")

/* 1 sec. timer is used for the general program sequence */
#callback MainTimer()
{
	if(iLedState)															// If LED 2 is currently "On" and LED 3 is currently "Off
	{
		digitalWrite(PIN_LED2, LED_DISABLE);		// Turns off LED 2
		digitalWrite(PIN_LED3, LED_ENABLE);			// Turns on LED 3

		#log("[LED] 2:off 3:on");
	}
	else																			// Otherwise (i.e. LED2 "Off" and LED3 "On")
	{
		digitalWrite(PIN_LED2, LED_ENABLE);				// Turns on LED 2
		digitalWrite(PIN_LED3, LED_DISABLE);			// Turns off LED 3

		#log("[LED] 2:on 3:off");
	}

	iLedState = !iLedState;										// Toggles the variable which holds the current LED state
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