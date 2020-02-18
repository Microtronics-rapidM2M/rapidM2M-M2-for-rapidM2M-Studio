  
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
 * Extended "Button" Example
 *
 * Evaluates the state of a button connected to the "INT0" pin and also detects if the button was
 * pressed only briefly or for a longer time
 *
 * If the button was pressed ("low" signal level) "[KEY] Key pressed" is issued via the console.
 * If the button was pressed and held for a longer time "[KEY] Long Push Detected" is issued via the console.
 * If the button was released after it had been pressed and held for a longer time "[KEY] Long key detected
 * and key released" is issued via the console.
 * If the button was released after it had been pressed briefly ("high" signal level) "[KEY] Key released"
 * is issued via the console.
 *
 * Only compatible with rapidM2M M2xx
 * Special hardware circuit necessary
 * Recommendation: use rapidM2M Base Starter
 *
 * @version 20190508
 */

const
{
	IRQ_KEY            = 0,                   // Interruptible pin "INT0" is used for the button
	CNT_LONG_PUSH_TIME = 5000,                // Time (ms) for which the button must be pressed to detect
	                                          // a long button press
}

/* Global variable declarations */
static iLongPushDetected;                   // Long key press detected

/* Function that is called when the timer to detect a long key press has expired  */
#callback KeyDetectLong()
{
  #log("[KEY] Long Push Detected");         // Issue the detection of a long key press via the console
  iLongPushDetected = 1;                    // Set "long key press detected" flag
}

/**
 * Function that should be called up when the button is pressed or released
 *
 * Note: An interruptible pin can only be configured to initiate the interrupt on the rising OR falling
 *       edges. If both pressing and releasing the button should be detected when an interrupt
 *       occurs, it is necessary to first deactivate the interrupt functionality and then activate the
 *       interrupt functionality again, but this time for the other edge.
 *
 * @param iKeyState:s32 - Signal level at the interruptible pin after the edge that triggered the
 *                        interrupt occurred
 *												0: "low" signal level
 *												1: "high" signal level
 */
#callback KeyChanged(iKeyState)
{
	/*  Deactivates the interrupt functionality and then activates the interrupt functionality again, but this time for the other edge
		If the current signal level is “low” (0) the interrupt functionality is activated for rising edges.
		If the current signal level is “high” (1) the interrupt functionality is be activated for falling edges.                       */
	setWatch( IRQ_KEY, iKeyState ? WATCH_FALLING : WATCH_RISING, KeyChanged);

	if(!iKeyState)                            // If the button was pressed ->
	{
		// Starts a timer for the purpose of detecting a long key press. If the timer expires, a flag is set by the function transferred.
 		setTimeout(KeyDetectLong,CNT_LONG_PUSH_TIME);
		
		#log("[KEY] Key pressed");              // Prints "[KEY] Key pressed" to the console
	}
	else                                      // Otherwise -> if the button was released ->
	{
		if(iLongPushDetected)                   // If the button had been pressed for a long time ->
		{
			// Prints "[KEY] Long key detected and key released" to the console
			#log("[KEY] Long key detected and key released");
	  
			iLongPushDetected = 0;                // Delete "long key press detected" flag
		}
		else                                    // Otherwise (i.e. the button was pressed only briefly) ->
		{
			/* Deletes the timer for long keystroke detection if the button has been pressed only for a short
			   time, so that it cannot expire and therefore the "long key press detected" flag is not set.   */
			clearTimeout(KeyDetectLong);

			#log("[KEY] Key released");     // Prints "[KEY] Key released" to the console
		}
	}
}

/* Application entry point */
main()
{
	/*  Initialisation of the interrupt input used for the button.
		An interrupt should accrue on a falling edge (i.e. button pressed) */
	setWatch( IRQ_KEY, WATCH_FALLING, KeyChanged);
}