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
 * transmitted periodically (transmission interval) to the server. The interval for recording
 * and transmitting measurement data as well as the transmission mode (interval, wakeup or
 * online) can be configured via the server.
 *
 * Note: To use the recorded data within the interface of the Server (e.g. reports, visualisations, graphics, etc.)
 *       it is necessary to define a Data Descriptor
 *
 * Only compatible with rapidM2M M2xx
 *
 *
 * @version 20190813
 */

const
{
	INTERVAL_RECORD = 60,                     // Interval of record [s]
	INTERVAL_TX     = 10 * 60,                // Interval of transmission [s]
}

/* Global variable to store the current configuration */
static aSettings[DDE_Settings]

/* Global variables for the remaining time until certain actions are triggered */
static iTxTimer;

/* 1s Timer used to read the last valid values for Vin and Vaux from the system and issue them via the console   */
#callback Timer1s()
{
	static iLastRecord;						// Timestamp used for the last record

	// If the measurement interval has expired (i.e. a record should be generated)
	if ( momentElapsed(iLastRecord, aSettings.RecInterval) )
	{
		#log("Create Record");
		RecordData(iLastRecord);			// Calls up the function to record the data
	}

	iTxTimer--;                             // Counter counting down the sec. to the next transmission
		if(iTxTimer <= 0)                       // When the counter has expired ->
		{
		#log("Start Transmission");
		catch( rM2M_TxStart());				// Initiates a connection to the server
		iTxTimer = aSettings.TxInterval;	// Resets counter var. to current transmission interval [sec.]
		}

	// Issues the seconds until the next transmission to the console
	#log("iTxTimer=%d", iTxTimer);
}

/**
 * Is called up for each UPwards config after starting automatic uplink control ("uplinkInit" function) to read
 * back transmitted information from local storage
 *
 * @param cfgId:s32 - Id of the configuration memory block (Use "DDE_xxx_id" located in "~auto-dde-inc")
 */
public onUplinkRestore( cfgId) {
}

/**
 * Is called up when a config block is changed to make it possible to react to a changed configuration received from the server.
 * It is also called up for each DOWNwards config after starting automatic uplink control ("uplinkInit" function) to recover last
 * known settings from local storage or to set safe fallback values for the config
 *
 * @param cfgId:s32 - Id of the changed configuration memory block (Use "DDE_xxx_id" located in "~auto-dde-inc")
 */
public onUplinkApply( cfgId)
{
	// If the changed configuration is the "Settings" config -> Calls up the function to apply the "Settings" config and return the function result
	if(cfgId == DDE_Settings_id) return ApplySettings();

	#log("?onUplinkApply %d", cfgId);
	return ERROR;							// Returns ERROR if a config not handled by this script was received
}

/* Is called up if at least one DOWNwards configs is not locally available (i.e. have not been received from the server yet)
	The device is waiting to receive config(s) from the server                                                            */
public onUplinkWaiting()
{
	#log( "?HOLD - waiting for initialization by backend...");
}

/**
 * Is called up when all DOWNwards configs are locally available (i.e. have been received from the server) and
 * therefore the regular operation can start
 *
 * @return s32	- Connection type to be used
 */
public onUplinkReady()
{
	setInterval(Timer1s,1000);              // Initialisation of a 1 sec. timer used for the general program sequence

	return aSettings.TxMode;                // Returns current transmission mode
}

/* Application entry point */
main()
{
	uplinkInit();							// Starts fully automatic uplink control
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

/* Function used to apply a changed "Settings" configuration received from the server
 *
 * @return s32	- OK:	 Config valid
 *              - ERROR: Config is invalid or not available
 */
ApplySettings()
{
	new iResult;							// Temporary memory for the return value of a function
	new aReadSettings[DDE_Settings] 		// Temporary memory for the read configuration

	iResult= DDE_Settings_read(aReadSettings);// Reads config from the system and copies it to the temporary memory.

	if(iResult == OK)						// If the config is valid
	{
		// If the received record interval does not correspond to that of the global variable -> Issues received value via the console
		if(aSettings.RecInterval != aReadSettings.RecInterval) #log("RecInterval changed to %d s", aReadSettings.RecInterval);

		// If the received transmission interval does not correspond to that of the global variable ->
		if(aSettings.TxInterval != aReadSettings.TxInterval)
		{
			#log("TxInterval changed to %d s", aReadSettings.TxInterval);	// Issues received value via the console
			iTxTimer = aReadSettings.TxInterval;							// Sets counter var. to the new transmission interval immediately
		}

		// If the received transmission mode does not correspond to that of the global variable ->
		if(aSettings.TxMode != aReadSettings.TxMode)
		{
			#log("TxMode changed to %d", aReadSettings.TxMode);		    	// Issues received value via the console
			uplinkMode(aReadSettings.TxMode);							    // Sets connection type to the received value
		}

		aSettings = aReadSettings;											// Copies received values into the global variable
	}
	return iResult							// Returns the result of the read config function
}