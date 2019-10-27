#include "MainForm.h"

MainForm::MainForm() noexcept
{
}

MainForm::~MainForm()
{
}

//IMPLEMENT_DYNAMIC(MainForm, CFrameWnd)

BEGIN_MESSAGE_MAP(MainForm, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int MainForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	status.Create(_T("Initializing"), WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(0, 0, 500, 20), this);

	return 0;
}

BOOL MainForm::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
	cs.cy = 80;
	cs.cx = 500;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

BOOL MainForm::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let children handle messages
	if (status.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
		return TRUE;
	}

	//return to super class
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#ifdef _DEBUG
void MainForm::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void MainForm::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG