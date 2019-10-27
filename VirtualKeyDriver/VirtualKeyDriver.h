#pragma once
#include "framework.h"
class VirtualKeyDriver : public CWinApp
{
public:
	VirtualKeyDriver() noexcept;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

extern VirtualKeyDriver app;