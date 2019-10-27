#include "VirtualKeyDriver.h"

BEGIN_MESSAGE_MAP(VirtualKeyDriver, CWinApp)
	ON_COMMAND(ID_CLOSE, &VirtualKeyDriver::OnClose)
END_MESSAGE_MAP()

VirtualKeyDriver::VirtualKeyDriver() noexcept {
	SetAppID(_T("lassombra.VirtualKey.0.1"));
}

VirtualKeyDriver app;


BOOL VirtualKeyDriver::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("VirtualKeyDriver"));

	CFrameWnd* pFrame = new MainForm();
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	BOOL result = pFrame->LoadFrame(IDR_MAIN_FORM, WS_OVERLAPPEDWINDOW, nullptr, nullptr);

	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	pFrame = new MainForm();
	if (!pFrame)
		return FALSE;
	return TRUE;
}

int VirtualKeyDriver::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void VirtualKeyDriver::OnClose() {
	m_pMainWnd->DestroyWindow();
}
