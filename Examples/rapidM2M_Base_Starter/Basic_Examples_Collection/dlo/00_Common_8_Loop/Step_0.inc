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
 * Simple rapidM2M "Loop" Example
 *
 * Example on how to use loops in rapidM2M projects
 *
 * Compatible with every rapidM2M hardware
 *
 * @version 20200218
 */

/* Application entry point */
main()
{
  new iBlock;
  
  // Static byte array that should be used as a buffer to output debug information via the console or watch panel
  static aConsole_Buffer[2048];

  // Provides the firmware with a buffer from the RAM area reserved for the device logic that is used to output debug information via the console or watch panel
  setbuf( aConsole_Buffer, 4 * sizeof (aConsole_Buffer));

  /* Calculates factorials in blocks of three */
  do
  {
	#log("--");
	for(new i = iBlock; i <= iBlock + 2; i++)
	{
	  /* Initial value must be 1 */
	  new iRes = 1;
	  new j = i;

	  while(j > 0)
	  {
		/* Calculates factorial for variable i with all positive integers smaller than i */
		iRes *= j--;
	  }

	  /* Prints factorial */
	  #log("%d! = %d", i, iRes);
	}
	iBlock += 3;
  } while(iBlock <= 8)

  #log("--");
}