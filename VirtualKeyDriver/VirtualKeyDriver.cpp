#include "pch.h"
#include "VirtualKeyDriver.h"

VirtualKeyDriver app;

VirtualKeyDriver::VirtualKeyDriver() noexcept {
	SetAppID(_T("lassombra.VirtualKey.0.1"));
}

BOOL VirtualKeyDriver::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("VirtualKeyDriver"));

	// add an actual window

	return TRUE;
}

int VirtualKeyDriver::ExitInstance()
{
	return CWinApp::ExitInstance();
}