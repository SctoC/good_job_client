#include "stdafx.h"
#include "MainDlg.h"
#include "ApplicationModel.h"
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);


	AppModel->setMaindlgHwnd(m_hWnd);

	CString account;
	CString pwd;
	LogIn_Dialog dlg(account, pwd);
	if (dlg.DoModal() == IDOK)
	{
		Init();
		show_tab(false);
	   AppModel->sendLoninRequest(account,pwd);
	}
	else
	{
		CloseDialog(IDOK);
		return 0;
	}
	
	
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CMainDlg::Init()
{
	tab_Init();
	
}

void CMainDlg::tab_Init()
{

	CRect rcClient;
	GetClientRect(&rcClient);

	int nWidth = (rcClient.Width() - 2) / 3;
	int nRemainder = (rcClient.Width() - 2) % 3;

	int nIndex = m_TabCtrl.AddItem(301, STCI_STYLE_DROPDOWN);
	m_TabCtrl.SetItemSize(nIndex, nWidth + nRemainder, 48, nWidth + nRemainder - 19, 19);
	m_TabCtrl.SetItemToolTipText(nIndex, _T("最近联系人"));
	m_TabCtrl.SetItemIconPic(nIndex, _T("MainTabCtrl\\icon_last_normal.png"), _T("MainTabCtrl\\icon_last_selected.png"));

	nIndex = m_TabCtrl.AddItem(302, STCI_STYLE_DROPDOWN);
	m_TabCtrl.SetItemSize(nIndex, nWidth, 48, nWidth - 19, 19);
	m_TabCtrl.SetItemToolTipText(nIndex, _T("联系人"));
	m_TabCtrl.SetItemIconPic(nIndex, _T("MainTabCtrl\\icon_contacts_normal.png"), _T("MainTabCtrl\\icon_contacts_selected.png"));

	nIndex = m_TabCtrl.AddItem(303, STCI_STYLE_DROPDOWN);
	m_TabCtrl.SetItemSize(nIndex, nWidth, 48, nWidth - 19, 19);
	m_TabCtrl.SetItemToolTipText(nIndex, _T("群/讨论组"));
	m_TabCtrl.SetItemIconPic(nIndex, _T("MainTabCtrl\\icon_group_normal.png"), _T("MainTabCtrl\\icon_group_selected.png"));


	m_TabCtrl.SetBgPic(_T("MainTabCtrl\\main_tab_bkg.png"), CRect(5, 1, 5, 1));
	m_TabCtrl.SetItemsBgPic(NULL, _T("MainTabCtrl\\main_tab_highlight.png"), _T("MainTabCtrl\\main_tab_check.png"), CRect(5, 1, 5, 1));
	m_TabCtrl.SetItemsArrowPic(_T("MainTabCtrl\\main_tabbtn_highlight.png"), _T("MainTabCtrl\\main_tabbtn_down.png"));

	CRect rcTabCtrl(0, 154, rcClient.right, 154 + 48);
	m_TabCtrl.Create(m_hWnd, rcTabCtrl, NULL, WS_CHILD | WS_VISIBLE, NULL, ID_TABCTRL_MAIN, NULL);

	m_TabCtrl.SetCurSel(1);
}

void CMainDlg::show_tab(bool isMain)
{   
	int IsMain = isMain ? SW_SHOW : SW_HIDE;
	m_TabCtrl.ShowWindow(IsMain);

	int IsLoging = isMain ? SW_HIDE:SW_SHOW;
	
	GetDlgItem(IDC_STATIC).ShowWindow(IsLoging);
	Invalidate();
}


void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}