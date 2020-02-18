/*
 *                  _     _  __  __ ___  __  __
 *                 (_)   | ||  \/  |__ \|  \/  |
 *  _ __ __ _ _ __  _  __| || \  / |  ) | \  / |
 * | '__/ _` | '_ \| |/ _` || |\/| | / /| |\/| |
 * | | | (_| | |_) | | (_| || |  | |/ /_| |  | |
 * |_|  \__,_| .__/|_|\__,_||_|  |_|____|_|  |_|
 *           | |
 *           |_|
 *
 * Simple "Data Types" Example
 *
 * Example on how to implement, handle and convert integer, floating-point and boolean variables in
 * rapidM2M projects.
 *
 * Compatible with every rapidM2M hardware
 *
 * @version 20200218
 */

/* Application entry point */
main()
{
  new iInt;                                 // Integer variable
  new bool:bBool;                           // Boolean variable
  new Float:fFloat;                         // Floating-Point variable

  // Static byte array that should be used as a buffer to output debug information via the console or watch panel
  static aConsole_Buffer[2048];

  // Provides the firmware with a buffer from the RAM area reserved for the device logic that is used to output debug information via the console or watch panel
  setbuf( aConsole_Buffer, 4 * sizeof (aConsole_Buffer));

  /* Prints default values to the console */
  #log("Integer default value: %d", iInt);
  #log("Floating-Point default value: %f", fFloat);
  #log("Boolean default value: %d", bBool);
  
  iInt = 3141592;
  
  /* An integer division without type conversion is performed. Decimal places are cut off. */
  #log("Result without type conv.: %d", iInt/1000);
  
  /* An integer division with explicit type conversion is performed. */
  #log("Result with expl. type conv.: %4.3f", float(iInt)/1000);
  
  /* An integer division with implicit type conversion is performed. */
  #log("Result with impl. type conv.: %4.3f", iInt/1000.0);

  /* Example for boolean variables (behave similar to s32 integer cells) */
  while(bBool == true || bBool == false)
  {
	/* Checks boolean data type variable */
	if(bBool == false)
	{
	  #log("bBool false");
	  bBool++;
	}
	else if(bBool == true)
	{
	  #log("bBool true");
	  bBool++;
	  break;
	}
  }
  /* Boolean variables can also contain other values like in C99 standard */
  #log("bBool = %d", bBool);
}