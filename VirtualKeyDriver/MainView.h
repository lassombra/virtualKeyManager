#pragma once
#include "framework.h"

class MainView :
	public CWnd
{
public:
	MainView();
	virtual ~MainView();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

