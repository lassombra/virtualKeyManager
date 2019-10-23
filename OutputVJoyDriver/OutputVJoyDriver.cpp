// OutputVJoyDriver.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "OutputVJoyDriver.h"

Capabilities Caps = Capabilities();


const Capabilities& __cdecl getCaps() {
	return Caps;
}
BOOL __cdecl buttonsDown (std::vector<BYTE> buttons) {
	return FALSE;
}
BOOL __cdecl buttonsUp(std::vector<BYTE> buttons) {
	return FALSE;
}
BOOL __cdecl dPadSet(BYTE dPad, DPad direction) {
	return FALSE;
}
BOOL __cdecl activate() {
	return FALSE;
}
BOOL __cdecl deactivate() {
	return TRUE;
}

OutputDriver driver = OutputDriver();


extern "C" OUTPUTDRIVER_API OutputDriver* __cdecl getDriver(void) {
	Caps.Buttons = 128;
	Caps.DPads = 2;
	driver.name = L"VJoy Output Driver";
	driver.buttonsDown = &buttonsDown;
	driver.buttonsUp = &buttonsUp;
	driver.dPadSet = &dPadSet;
	driver.getCaps = &getCaps;
	driver.activate = &activate;
	driver.deactivate = &deactivate;
	return &driver;
}
extern "C" OUTPUTDRIVER_API void __cdecl releaseDriver(OutputDriver * driver) {}