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
 * Simple rapidM2M "Conditional" Example
 * 
 * Example on how to use if and switch statements in rapidM2M projects
 * 
 * Compatible with every rapidM2M hardware
 *
 * @version 20190816
 */

/* Application entry point */
main()
{
  /* Get date and time */
  new sDateTime[TrM2M_DateTime];
  rM2M_GetDateTime(sDateTime);
  
  if(sDateTime.hour < 11)
  {
	#log("Good Morning!");
  }
  else if(sDateTime.hour > 16)
  {
	#log("Good Evening!");
  }
  else
  {
	#log("Enjoy your meal!");
  }
  
  /* Note: No break statement is used inside a switch statement */
  switch(sDateTime.DoW)
  {
	case 5:
	  #log("Today is saturday!");
	case 6:
	  #log("Today is sunday!");
	default:
	  #log("Today is a workday!");
  }
}