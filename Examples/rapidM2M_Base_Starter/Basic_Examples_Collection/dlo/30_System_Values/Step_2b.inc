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
 * Note: In contrast to example "30_System_Values_2a" this example uses the "uplink" library. Example "30_System_Values_2a"
 *       is extended with a mechanism which grants the downward configs to be valid before the device starts its regular
 *       operation.
 *
 * Only compatible with rapidM2M M2xx
 *
 *
 * @version 20200218
 */

/* Global variable to store the current configuration */
static aSettings[DDE_Settings];

/* Global variables for the remaining time until certain actions are triggered */
static iTxTimer;

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
 * The ~uplink library signals important event
 *
 * @param ev:s32     - uplink event
 * @param param: s32 - optional parameter, meaning depends on ev:
 *                         UPLINK_EV_WAITING       - bitmask of missing configs
 *                         UPLINK_EV_ONLINE_DIAL   - 0 upon boot, or last seen TrM2M_TxError after uplink broken
 *                         UPLINK_EV_ONLINE_BROKEN - last seen TrM2M_TxError
 */
onUplinkEvent( ev, param)
{
	switch(ev)
	{
		/* If at least one DOWNwards config is not locally available (i.e. have not been received from the server yet)
		   the device is waiting to receive config(s) from the server                                                       */
		case UPLINK_EV_WAITING:
		{
			#log( "?HOLD - waiting for initialization by backend... (%x)",param);
		}

		/* When all DOWNwards configs are locally available (i.e. have been received from the server) and therefore the
		   regular operation can start                                                                                      */
		case UPLINK_EV_READY:
		{
			#log( "Initialization Done!");

			setInterval(Timer1s,1000);            // Initialisation of a 1 sec. timer used for the general program sequence
		}

	}
}

/* Application entry point */
main()
{
	// Provides the firmware with a buffer from the RAM area reserved for the device logic that is used to output debug information via the console or watch panel
	setbuf( aConsole_Buffer, 4 * sizeof (aConsole_Buffer));
	
	uplinkInit(UPLINK_WAKEUP, true);          // Starts fully automatic uplink control
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
 *
 * @return s32	- OK:	 Config valid
 *              - ERROR: Config is invalid or not available
 */
onUplinkApply_Settings()
{
	new iResult;                        // Temporary memory for the return value of a function
	new aReadSettings[DDE_Settings];    // Temporary memory for the read configuration

	iResult= DDE_Settings_read(aReadSettings);// Reads config from the system and copies it to the temporary memory.

	if(iResult == OK)                   // If the config is valid
	{
		// If the received record interval does not correspond to that of the global variable -> Issues received value via the console
		if(aSettings.RecInterval != aReadSettings.RecInterval) #log("RecInterval changed to %d s", aReadSettings.RecInterval);

		// If the received transmission interval does not correspond to that of the global variable ->
		if(aSettings.TxInterval != aReadSettings.TxInterval)
		{
			#log("TxInterval changed to %d s", aReadSettings.TxInterval);	// Issues received value via the console
			iTxTimer = aReadSettings.TxInterval; // Sets counter var. to the new transmission interval immediately
		}

		aSettings = aReadSettings;        // Copies received values into the global variable
	}
	return iResult                      // Returns the result of the read config function
}