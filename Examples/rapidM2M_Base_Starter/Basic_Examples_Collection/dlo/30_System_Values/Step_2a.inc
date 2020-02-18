
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
 * transmitted periodically (transmission interval) to the server. The interval for recording and transmitting
 * measurement data can be configured via the server.
 *
 * Note: To use the recorded data within the interface of the Server (e.g. reports, visualisations, graphics, etc.)
 *       it is necessary to define a Data Descriptor
 *
 * Note: This example uses poor practice: When downward configs are not available on the device they are simply
 *       replaced with some defaults (for easy demonstration).
 *
 * Only compatible with rapidM2M M2xx
 *
 *
 * @version 20200218
 */

const
{
  INTERVAL_RECORD = 60,                     // Interval of record [s]
  INTERVAL_TX     = 10 * 60,                // Interval of transmission [s]
}

/* Global variable to store the current configuration */
static aSettings[DDE_Settings]

/* Global variables for the remaining time until certain actions are triggered */
static iTxTimer;                            // Sec. until the next transmission

/* Static byte array that should be used as a buffer to output debug information via the console or watch panel */
static aConsole_Buffer[2048];

 /* 1s Timer used to read the last valid values for Vin and Vaux from the system and issue them via the console   */
#callback Timer1s()
{
	static iLastRecord;                       // Timestamp used for the last record

	// If the measurement interval has expired (i.e. a record should be generated)
	if ( momentElapsed(iLastRecord, aSettings.RecInterval) )
	{
		#log("Create Record");
		RecordData(iLastRecord);                // Calls up the function to record the data
	}

	iTxTimer--;                               // Counter counting down the sec. to the next transmission
	if(iTxTimer <= 0)                         // When the counter has expired ->
	{
		#log("Start Transmission");
		catch( rM2M_TxStart());                 // Initiates a connection to the server
		iTxTimer = aSettings.TxInterval;        // Resets counter var. to current transmission interval [sec.]
	}

	// Issues the seconds until the next transmission to the watch panel
	#watch("iTxTimer=%d", iTxTimer);
}

/**
 * Function that should be called up when a config block is changed to make it possible to react
 * to a changed configuration received from the server
 *
 * @param cfgIdx:s32 - Number of the changed configuration memory block starting with 0 for the first memory block
 */
#callback ReadConfig(cfgIdx)
{
	// If the changed configuration is the "Settings" config -> Calls up the function to apply the "Settings" config
	if (cfgIdx==DDE_Settings_idx) onUplinkApply_Settings();
}

/* Application entry point */
main()
{
	// Provides the firmware with a buffer from the RAM area reserved for the device logic that is used to output debug information via the console or watch panel
	setbuf( aConsole_Buffer, 4 * sizeof (aConsole_Buffer));

	setInterval(Timer1s,1000);                // Initialisation of a 1 sec. timer used for the general program sequence
	watchUplinkConfigs(ReadConfig);           // Specification of the function that should be called up when a config block is changed
	onUplinkApply_Settings();                 // Inits configuration

	iTxTimer  = aSettings.TxInterval;         // Sets counter variable to current transmission interval

	//Starts initial connection
	catch(rM2M_TxSetMode(RM2M_TXMODE_WAKEUP));// Sets the connection type to "Wakeup" mode
	catch(rM2M_TxStart());                    // Initiates a connection to the server

}
/**
 * Reads the last valid values for Vin and Vaux from the system, compiles the data record to
 * be saved and transfers the compounded data record to the system to be recorded
 *
 * @param iTimestamp:s32 - Time stamp that should be used for the recording
 */
RecordData(iTimestamp)
{
	new aSysValues[TMx_SysValue];       // Temporary memory for the internal measurement values
	new aRecData[DDE_measurements];     // Temporary memory for the data record to be saved

	Mx_GetSysValues(aSysValues);        // Reads the last valid values for Vin and Vaux from the system
	                                    // The interval for determining these values is 1sec. and cannot be changed.

	aRecData.Vin  = aSysValues.VIn;     // Copies the last valid values for Vin into the temporary memory for the data record to be saved
	aRecData.Vaux = aSysValues.VAux;    // Copies the last valid values for Vaux into the temporary memory for the data record to be saved

	DDE_measurements_write(iTimestamp, aRecData);// Transfers data record to the system to be recorded

}

/* Function used to apply a changed "Settings" configuration received from the server
   If the configuration cannot be read from the system, the global variables are initialised with default values. */
onUplinkApply_Settings()
{
	new iResult;                        // Temporary memory for the return value of a function
	new aReadSettings[DDE_Settings]     // Temporary memory for the read configuration

	iResult= DDE_Settings_read(aReadSettings);// Reads config from the system and copies it to the temporary memory.

	if(iResult< OK)                     // If the config is invalid or not available
	{
		// Initialises the configuration with default values
		aReadSettings.RecInterval = INTERVAL_RECORD;
		aReadSettings.TxInterval  = INTERVAL_TX;
		#log("created new Config");
	}

	// If the received record interval does not correspond to that of the global variable -> Issues received value via the console
	if(aSettings.RecInterval != aReadSettings.RecInterval) #log("RecInterval changed to %d s", aReadSettings.RecInterval);

	// If the received transmission interval does not correspond to that of the global variable ->
	if(aSettings.TxInterval != aReadSettings.TxInterval)
	{
		#log("TxInterval changed to %d s", aReadSettings.TxInterval); // Issues received value via the console
		iTxTimer = aReadSettings.TxInterval; // Sets counter var. to the new transmission interval immediately
	}

	aSettings = aReadSettings;          // Copies received values into the global variable
	
	return OK;
}
