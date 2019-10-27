#pragma once
#include "framework.h"
#include "MainForm.h"
#include "resource.h"

class VirtualKeyDriver : public CWinApp
{
public:
	VirtualKeyDriver() noexcept;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

extern VirtualKeyDriver app;