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
 *
 * Prints the information for identifying the rapidM2M hardware and the implemented API level to the 
 * development console.
 * 
 * Compatible with every rapidM2M hardware.
 *
 * @version 20190816  
 */

/* Application entry point */
main()
{
  new aId[TrM2M_Id];                        // Temporary memory for the HW module information
  
  rM2M_GetId(aId);							// Retrieves information to identify the rapidM2M hardware

  // Prints "Module identification", "Module type", "Hardware major version" and "Hardware minor version" to the console
  #log("Id:%s Module:'%s' HW:%d.%d", aId.string, aId.module, aId.hwmajor, aId.hwminor);
  
  // Prints the serial number of the device to the console
  #log("SN:%02X%02X%02X%02X%02X%02X%02X%02X", aId.sn{0}, aId.sn{1}, aId.sn{2}, aId.sn{3}, aId.sn{4}, aId.sn{5}, aId.sn{6}, aId.sn{7});
  
  // Prints the "Firmware major version", "Firmware minor version" and the "Site title (context)" to the console 
  #log("FW:%d.%d Context:'%s'", aId.fwmajor, aId.fwminor, aId.ctx);
  
  // Retrieves the implemented API level of the script engine and prints it to the console
  #log("APIlevel = %d", getapilevel());
}

