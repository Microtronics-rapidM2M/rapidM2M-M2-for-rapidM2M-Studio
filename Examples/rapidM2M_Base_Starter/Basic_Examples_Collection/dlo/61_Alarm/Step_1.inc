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
 * Extended "Alarm" Example
 *
 * Simulates and records (record interval) a temperature value that changes between 19°C and 31°C in 1°C steps.
 * The measurement data generated this way is then transmitted periodically (transmission interval) to the server.
 * If the temperature is greater than or equal to 25°C, an alarm is triggered. Once an alarm has been triggered and the
 * temperature falls to or below 25°C - 5% (i.e. 23,75°C) again, the alarm is released. In both cases (when triggering
 * or releasing the alarm) an alarm record is generated and transmitted to the server immediately.
 *
 * Compatible with every rapidM2M hardware
 *
 * @version 20200213
 */

const
{
	INTERVAL_RECORD = 10,                       // Interval of record [s]
	INTERVAL_TX     = 10 * 60,                  // Interval of transmission [s]
}
const Float:fAlarmThreshold = 25.0;           // If the temperature exceeds this threshold, an alarm is triggered.
const Float:fHysteresis     = 5.0;            // Hysteresis for releasing an active alarm [%]
                                              // Once an alarm has been triggered, the alarm is released if the temperature
                                              // falls below "fAlarmThreshold - Abs(fAlarmThreshold * (fHysteresis/100) )"

/* Global variables for the remaining time until certain actions are triggered */
static iTxTimer;                              // Sec. until the next transmission

static Float:fTemperature = 20.0;             // Global variables for the current temperature

/* 1 sec. timer is used for the general program sequence */
#callback Timer1s()
{
	static iLastRecord;                         // Timestamp used for the last record

	// If the measurement interval has expired (i.e. a record should be generated)
	if ( momentElapsed(iLastRecord, INTERVAL_RECORD) )
	{
		UpdateTemperature();                      // Calls up the function to update the current temperature
		RecordData(iLastRecord);                  // Calls up the function to record the data
		CheckAlarm();                             // Calls up the function which checks whether an alarm has to be triggered
	}
  
	iTxTimer--;                                 // Counter counting down the sec. to the next transmission
	if(iTxTimer <= 0)                           // When the counter has expired ->
	{
		#log("Start Transmission");
		catch( rM2M_TxStart());                   // Initiates a connection to the server
		iTxTimer = INTERVAL_TX;                   // Resets counter var. to defined transmission interval [sec.]
	}
	// Issues the seconds until the next transmission to the watch panel
	#watch("iTxTimer=%d", iTxTimer);
}

/* Application entry point */
main()
{
	/* Initialisation of a cyclic 1 sec. timer */
	setInterval(Timer1s, 1000);

	iTxTimer  = INTERVAL_TX;                    // Sets counter variable to defined transmission interval
	
	DDE_alerts_clear(DDE_measurements_id, 0);   // Clear alarm on start

	//Starts initial connection
	catch(rM2M_TxSetMode(RM2M_TXMODE_TRIG));    // Sets the connection type to "Wakeup" mode
	catch(rM2M_TxStart());                      // Initiates a connection to the server
}

/**
 * Compiles the data record to be saved and transfers the compounded data record to the system to be recorded
 *
 * @param iTimestamp:s32 - Time stamp that should be used for the recording
 */
RecordData(iTimestamp)
{
	new aRecData[DDE_measurements];             // Temporary memory for the data record to be saved

	// Copies the last valid value for Temperature into the temporary memory for the data record to be saved
	aRecData.Temperature = fTemperature;
	
	DDE_measurements_write(iTimestamp, aRecData);// Transfers data record to the system to be recorded
}

/**
 * Calculates the absolute value
 *
 * @param fValue:f32 - Value for which the absolute value should be determined
 * @return f32       - Absolute value of the transferred value
 *
 */
Float:Abs(Float:fValue)
{
 return ((fValue)<0 ? (-fValue) : (fValue))
}

/**
 * Checks whether an alarm has to be triggered or released
 *
 * If the temperature exceeds the threshold, an alarm is triggered. Once an alarm has been triggered the function
 * checks whether the temperature falls below the threshold again and releases the alarm in that case.
 *
 * Note: It is recommended to use a hysteresis. This means that the alarm should only be released again when the measured
 *       value falls 5% below the alarm threshold (see Step_1.inc)
 *
 */
CheckAlarm()
{
	static iIsAlarmActive;                      // Current alarm state(0 = no alarm, 1 = alarm active)

	// If an alarm is currently active and the temperature falls to or below the alarm threshold - 5% (hysteresis) ->
	if((iIsAlarmActive == 1) && ( fTemperature <= ( fAlarmThreshold - Abs( (fAlarmThreshold * fHysteresis/100) ) )   ))
	{
		// Clears alarm and starts transmission
		#log("clear Alarm");
		DDE_alerts_clear(DDE_measurements_id, 0); // Clear alarm on start
		catch(rM2M_TxStart());                    // Initiates a connection to the server
		iIsAlarmActive = 0;                       // Sets current alarm state to "no alarm"*/
	}
	// Otherwise -> If no alarm is currently active and the temperature is greater than or equal to the alarm threshold ->
	else if((iIsAlarmActive == 0) && (fTemperature >= fAlarmThreshold))
	{
    // Sets alarm and starts transmission
		#log("set Alarm");
		DDE_alerts_underflow(true,DDE_measurements_id, 0, fTemperature, fAlarmThreshold);// Sets alarm
		catch(rM2M_TxStart());                    // Initiates a connection to the server
		iIsAlarmActive = 1;                       // Sets current alarm state to "alarm active"
	}
}

/**
 * Simulates a sensor
 *
 * This function generates a temperature value between 19°C and 31°C.
 * As long as the temperature is less than or equal to 30°C, the temperature value is increased by 1°C each time
 * the function is called. Then it is reduced by 1°C until it is less than 20°C.
 *
 */
UpdateTemperature()
{
	static Float:fDelta = 1.0;                  // Value added or subtracted to the temperature when the function is called

	if(fTemperature > 30.0)                     // If the upper limit(31°C) is reached ->
		fDelta = -1.0;                            // Sets the value which is added or subtracted to the temperature to "-1"

	if(fTemperature < 20.0)                     // If the lower limit(19°C) is reached ->
		fDelta = 1.0;                             // Sets the value which is added or subtracted to the temperature to "+1"

	fTemperature = fTemperature + fDelta;       // Calculates the new temperature

	#log("Temperature=%g", fTemperature);
}