#pragma once
#include <vector>
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the OUTPUTVJOYDRIVER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// OUTPUTVJOYDRIVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DRIVER_EXPORTS
#define DRIVER_API __declspec(dllexport)
#else
#define DRIVER_API __declspec(dllimport)
#endif

// available buttons and dpads to press/release.
struct Capabilities {
	BYTE Buttons;
	BYTE DPads;
};

// direction of output dpads.
// not used by all drivers
enum DPad:BYTE{
	Up,
	Down,
	Left,
	Right,
	None
};

// OutputDriver object
struct OutputDriver {
	// called once after activation to get capabilities.  Assumed unchanged after that
	const Capabilities& (__cdecl *getCaps)();
	// called to activate the driver.  The driver is expected after this call returns
	// to accept calls to all other functions assuming that they don't violate the 
	// capabilities
	// The driver can at this time allocate system resources it needs.
	BOOL(__cdecl* activate)();
	// called to set button states to pressed.  buttons is a vector of button ids
	// newly pressed since the last action.
	// Assumed to be idempotent
	BOOL (__cdecl *buttonsDown) (std::vector<BYTE> buttons);
	// called to set button states to released.  buttons is a vector of button ids
	// newly released since the last action.
	// Assumed to be idempotent
	BOOL (__cdecl *buttonsUp) (std::vector<BYTE> buttons);
	// called to set the direction of a dpad identified by dPad and with the provided direction
	// Assumed to be idempotent
	BOOL (__cdecl *dPadSet)(BYTE dPad, DPad direction);
	// Called to deactivate the driver.  The driver may use this to free any system
	// resources it is holding (such as virtual device drivers) and is expected
	// to fail (return FALSE) any calls except getCaps and activate.
	BOOL(__cdecl* deactivate)();
	// The user friendly name of this driver used in configuration options
	const wchar_t* name;
};

// Input Driver handles
struct InputActions {
	// function to pass a button down action
	// function is semi-idempotent in that it will
	// always fire the action on the output driver, but the
	// output driver is expected to be idempotent.
	void(__cdecl* setDown)(BYTE button);
	// function to pass a button up action
	// function is semi-idempotent in that it will
	// always fire the action on the output driver, but the
	// output driver is expected to be idempotent.
	void(__cdecl* setUp)(BYTE button);
	// function to set a dpad state.
	// function is semi-idempotent in that it will always
	// fire the action on the output driver, but the output
	// driver is epxected to be idempotent.
	void(__cdecl* setDPad)(BYTE dPad, DPad direction);
};

// InputDriver object
struct InputDriver {
	// called whenever capabilities change.  Not expected
	// to immediately update UI (discretion of plugin)
	// but expected to immediately stop sending instructions
	// oustide of new capabilities.
	// If input driver is acting in a multi-threaded fashion, must be thread safe.
	void(__cdecl* setCapabilities)(const Capabilities& caps);
	// called to initialize input driver.  Can be used to set up required
	// system resources and activate input mechanisms.
	BOOL(__cdecl* activate)(const InputActions&);
	// called to deactivate the input driver.  Should immediately stop accepting input
	// and should immediately stop sending actions.
	// Should be used to clean up system resources allocated in activation
	BOOL(__cdecl* deactivate)();
	// The user friendly name of this driver used in configuration options
	const wchar_t* name;
};