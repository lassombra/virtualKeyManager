#pragma once
#include "DriverPlugin.h"

//Loads a pointer to a driver structure.  This structure should be fully populated
//with all function handles at the time this is called.  The driver is permitted
//to maintain a pointer or reference to this driver as well, and to that end can
//change the function handles to reflect changing driver states.
//however, get caps will only be called once, during activation of the driver.
//It will only be called again if the driver is deactivated and reactivated.
//Get Caps is assumed to be idempotent.
extern "C" DRIVER_API InputDriver * __cdecl getInputDriver(void);
//Called to inform the module that the driver is no longer needed.  It will not
//be referenced again after this call is made.
//The call will only be made when the driver module is being unloaded.
extern "C" DRIVER_API void __cdecl releaseInputDriver(InputDriver * driver);