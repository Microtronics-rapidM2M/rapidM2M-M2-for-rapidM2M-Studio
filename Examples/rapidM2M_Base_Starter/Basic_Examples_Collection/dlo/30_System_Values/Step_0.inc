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
 * 
 * Simple "System Values" Example
 *
 * Reads the last valid values for Vin and Vaux from the system and issues them every second via the console 
 *
 * Only compatible with rapidM2M M2xx
 * 
 *
 * @version 20190508  
 */

/* 1s Timer used to read the last valid values for Vin and Vaux from the system and issue them via the console   */
#callback Timer1s()
{
  new aSysValues[TMx_SysValue];             // Temporary memory for the internal measurement values
  
  Mx_GetSysValues(aSysValues);              // Reads the last valid values for Vin and Vaux from the system 
											// The interval for determining these values is 1sec. and cannot be changed.
  
  // Issues the last valid values for Vin and Vaux via the console
  #log ("Vin = %dmV Vaux = %dmV", aSysValues.VIn, aSysValues.VAux)
}

/* Application entry point */
main()
{
	setInterval(Timer1s,1000);              // Initialisation of a 1 sec. timer used for the general program sequence
}