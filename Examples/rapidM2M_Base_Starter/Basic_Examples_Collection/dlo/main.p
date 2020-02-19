/* =============================================================================
	APP/DLO - DEVICE LOGIC's MAIN MODULE
============================================================================= */
//	!!! Have a look at the DLO documentation !!!
//
//  The examples included in the basic example collection are designed to
//	demonstrate how to use the rapidM2M Device API. In addition to the basic
//	handling you also find best practice examples. With the increasing number
//	at the beginning of the name, the complexity of the example increases as
//	well.
//
//	To change the active example proceed as follows:
//	1) Switch to the project overview and clone the Basic Example Collection
//	   project
//	2) Open the Data Descriptor file
//	3) Remove the "/*" mark before the "#define" of the example you want to
//	   activate (e.g. "#define System_Values_0		(30)" )
//
//	Note: The explanations of the examples can be found in the Data Descriptor
//	      file.
/* ========================================================================== */

// Simple rapidM2M "Hello World"
#ifdef Common_0_Main
	#include "00_Common_0_Main/Step_0"
#endif

// Extended rapidM2M "Hello World" Example
#ifdef Common_1_Timer
	#include "00_Common_1_Timer/Step_0"
#endif

// Simple "Get Module Info" Example
#ifdef Common_2_Get_Module_Info
	#include "00_Common_2_Get_Module_Info/Step_0"
#endif

// Simple "Named Array" Example
#ifdef Common_3_NamedArray
	#include "00_Common_3_NamedArray/Step_0"
#endif

// Simple "Pack/Unpack" Example
#ifdef Common_4_Pack
	#include "00_Common_4_Pack/Step_0"
#endif

// Simple "Data Types" Example
#ifdef Common_5_Data_Types
	#include "00_Common_5_Data_Types/Step_0"
#endif

// Simple "Array" Example
#ifdef Common_6_Array
	#include "00_Common_6_Array/Step_0"
#endif

// Simple "Conditional" Example
#ifdef Common_7_Conditional
	#include "00_Common_7_Conditional/Step_0"
#endif

// Simple "Loop" Example
#ifdef Common_8_Loop
	#include "00_Common_8_Loop/Step_0"
#endif

//Simple "Button" Example
#ifdef Switch_0
	#include "10_Switch/Step_0"
#endif

//Extended "Button" Example
#ifdef Switch_1
	#include "10_Switch/Step_1"
#endif

//Simple "LED" Example
#ifdef Led_0
	#include "11_Led/Step_0"
#endif

//Extended "LED" Example
#ifdef Led_1
	#include "11_Led/Step_1"
#endif





// Simple "System Values" Example
#ifdef System_Values_0
	#include "30_System_Values/Step_0"
#endif

//Extended "System Values" Example
#ifdef System_Values_1
	#include "30_System_Values/Step_1"
#endif

//Extended "System Values" Example
#ifdef System_Values_2a
	#include "30_System_Values/Step_2a"
#endif

//Extended "System Values" Example
#ifdef System_Values_2b
	#include "30_System_Values/Step_2b"

	#define UPLINK_LIB_USED	(1)
#endif

//Extended "System Values" Example
#ifdef System_Values_3
	#include "30_System_Values/Step_3"

	#define UPLINK_LIB_USED	(1)
#endif


//Simple "Alarm" Example
#ifdef Alarm_0
	#include "61_Alarm/Step_0"
#endif

//Extended "Alarm" Example
#ifdef Alarm_1
	#include "61_Alarm/Step_1"
#endif





//Simple "GNSS NMEA" Example
#ifdef GNSS_NMEA_system
	#include "80_GNSS_NMEA_system/Step_0"

	// #define UPLINK_LIB_USED	(1) todo
#endif

#ifndef UPLINK_LIB_USED
	/* The following function is not used for this example. However, it must be defined because the
	   "~uplink" library required for some other examples is included in the Basic Example Project.          */
	public onUplinkEvent( ev, param) {}
#endif