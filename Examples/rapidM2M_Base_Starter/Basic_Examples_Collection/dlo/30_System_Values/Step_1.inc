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
 * Extended "System Values" Example
 *
 * Reads the last valid values for Vin and Vaux periodically (record interval) from the system and stores the 
 * generated data record in the flash of the system. The measurement data generated this way is then 
 * transmitted periodically (transmission interval) to the server. 
 * 
 * Note: To use the recorded data within the interface of the server (e.g. reports, visualisations, graphics, etc.)
 *       it is necessary to define a Data Descriptor
 *
 * 
 * Only compatible with rapidM2M M2xx
 * 
 *
 * @version 20190508  
 */

const
{
  INTERVAL_RECORD = 60,                     // Interval of record [s]
  INTERVAL_TX     = 10 * 60,                // Interval of transmission [s]
}

/* Global variables for the remaining time until certain actions are triggered */
static iTxTimer;                            // Sec. until the next transmission 

 /* 1s Timer used to read the last valid values for Vin and Vaux from the system and issue them via the console   */
#callback Timer1s()
{
	static iLastRecord;						// Timestamp used for the last record
	
	// If the measurement interval has expired (i.e. a record should be generated)
	if ( momentElapsed(iLastRecord, INTERVAL_RECORD) )
	{
		#log("Create Record");     
		RecordData(iLastRecord);			// Calls up the function to record the data 
	  }
  
	  iTxTimer--;                          	// Counter counting down the sec. to the next transmission
	  if(iTxTimer <= 0)                       // When the counter has expired -> 
	  {
		#log("Start Transmission");
		catch( rM2M_TxStart());				// Initiates a connection to the server
		iTxTimer = INTERVAL_TX;             // Resets counter var. to defined transmission interval [sec.]  
	  }
  
	  // Issues the seconds until the next transmission to the console
	  #log("iTxTimer=%d", iTxTimer);
}

/* Application entry point */
main()
{
	setInterval(Timer1s,1000);              // Initialisation of a 1 sec. timer used for the general program sequence

	iTxTimer  = INTERVAL_TX;				// Sets counter variable to defined transmission interval

	catch( rM2M_TxStart());					// Initiates a connection to the server
}

/**
 * Reads the last valid values for Vin and Vaux from the system, compiles the data record to  
 * be saved and transfers the compounded data record to the system to be recorded            
 *
 * @param iTimestamp:s32 - Time stamp that should be used for the recording
 */   
RecordData(iTimestamp)
{
	new aSysValues[TMx_SysValue];			// Temporary memory for the internal measurement values
	new aRecData[DDE_measurements];			// Temporary memory for the data record to be saved

	Mx_GetSysValues(aSysValues);			// Reads the last valid values for Vin and Vaux from the system 
											// The interval for determining these values is 1sec. and cannot be changed.
	
	aRecData.Vin  = aSysValues.VIn;			// Copies the last valid values for Vin into the temporary memory for the data record to be saved
	aRecData.Vaux = aSysValues.VAux;		// Copies the last valid values for Vaux into the temporary memory for the data record to be saved

	DDE_measurements_write(iTimestamp, aRecData);// Transfers data record to the system to be recorded
}	