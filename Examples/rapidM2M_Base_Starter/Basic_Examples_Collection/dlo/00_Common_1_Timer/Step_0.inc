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
 * Extended rapidM2M "Hello World" Example
 * Prints "Hello World" every second to the development console
 *
 * Compatible with every rapidM2M hardware
 *
 * @version 20190816
 */


/* Cyclic 1s timer used to issue "Hello World" every second via the console */
#callback Timer1s()
{
  static iCounter = 0;                      // Number of times "Hello World" was issued via the console

  iCounter++;                               // Increase Counter
  #log("Hello World %d", iCounter);   		// Prints "Hello World" and the current counter reading to the console
}

/* Application entry point */
main()
{
  /* Initialisation of a cyclic 1 sec. timer */
  setInterval(Timer1s, 1000);
}