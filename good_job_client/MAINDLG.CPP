#include "stdafx.h"
#include "MainDlg.h"
#include "GroupChatDlg.h"

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

	//CMyDialog_test dlg_test;
	//dlg_test.DoModal();

	LogIn_Dialog dlg(account, pwd);
	if (dlg.DoModal() == IDOK)
	{
		Init();
		show_tab(false);
		AppModel->setAccount(account);
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
	::PostQuitMessage(0);
	return 0;
}

LRESULT CMainDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();  // 销毁对话框
	return 0;
}

LRESULT CMainDlg::OnLoginResult(UINT, WPARAM, LPARAM lParam, BOOL&)
{
	bool* success = reinterpret_cast<bool*>(lParam);
	if (*success)
	{
		updataBuddyList();
		updataGroupList();
		show_tab(true);
	}
	delete success;
	return 0;
}

void CMainDlg::Init()
{
	tab_Init();
	buddyList_Init();
	groupList_Init();
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

void CMainDlg::buddyList_Init()
{
		m_BuddyListCtrl.SetMargin(CRect(2, 0, 2, 0));
		m_BuddyListCtrl.SetBgPic(_T("BuddyList\\Bg.png"));
		m_BuddyListCtrl.SetBuddyTeamHotBgPic(_T("BuddyList\\BuddyTeamHotBg.png"));
		m_BuddyListCtrl.SetBuddyItemHotBgPic(_T("BuddyList\\BuddyItemHotBg.png"), CRect(1, 1, 1, 1));
		m_BuddyListCtrl.SetBuddyItemSelBgPic(_T("BuddyList\\BuddyItemSelBg.png"), CRect(1, 1, 1, 1));
		m_BuddyListCtrl.SetHeadFramePic(_T("BuddyList\\Padding4Select.png"), CRect(6, 6, 6, 6));
		m_BuddyListCtrl.SetNormalArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTexture.png"));
		m_BuddyListCtrl.SetHotArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTextureHighlight.png"));
		m_BuddyListCtrl.SetSelArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTextureHighlight.png"));
		m_BuddyListCtrl.SetNormalExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTexture.png"));
		m_BuddyListCtrl.SetHotExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTextureHighlight.png"));
		m_BuddyListCtrl.SetSelExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTextureHighlight.png"));
		m_BuddyListCtrl.SetStdGGHeadPic(_T("BuddyList\\trad_boy.png"));
		m_BuddyListCtrl.SetStdMMHeadPic(_T("BuddyList\\trad_girl.png"));
		m_BuddyListCtrl.SetStyle(BLC_STANDARD_ICON_STYLE);
		m_BuddyListCtrl.SetShowBigIconInSel(TRUE);
		m_BuddyListCtrl.SetBuddyTeamHeight(24);
		m_BuddyListCtrl.SetBuddyItemHeightInBigIcon(54);
		m_BuddyListCtrl.SetBuddyItemHeightInSmallIcon(28);
		m_BuddyListCtrl.SetBuddyItemHeightInStandardIcon(26);
		m_BuddyListCtrl.SetBuddyItemPadding(1);

		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcListCtrl(0, 200, rcClient.Width(), 200 + rcClient.Height() - 236);
		m_BuddyListCtrl.Create(m_hWnd, rcListCtrl, NULL, WS_CHILD | WS_VISIBLE, NULL, ID_LISTCTRL_BUDDY);

}

void CMainDlg::groupList_Init()
{
	m_GroupListCtrl.SetMargin(CRect(2, 0, 2, 0));
	m_GroupListCtrl.SetBgPic(_T("BuddyList\\Bg.png"));
	m_GroupListCtrl.SetBuddyTeamHotBgPic(_T("BuddyList\\BuddyTeamHotBg.png"));
	m_GroupListCtrl.SetBuddyItemHotBgPic(_T("BuddyList\\BuddyItemHotBg.png"), CRect(1, 1, 1, 1));
	m_GroupListCtrl.SetBuddyItemSelBgPic(_T("BuddyList\\BuddyItemSelBg.png"), CRect(1, 1, 1, 1));
	m_GroupListCtrl.SetHeadFramePic(_T("BuddyList\\Padding4Select.png"), CRect(6, 6, 6, 6));
	m_GroupListCtrl.SetNormalArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTexture.png"));
	m_GroupListCtrl.SetHotArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTextureHighlight.png"));
	m_GroupListCtrl.SetSelArrowPic(_T("BuddyList\\MainPanel_FolderNode_collapseTextureHighlight.png"));
	m_GroupListCtrl.SetNormalExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTexture.png"));
	m_GroupListCtrl.SetHotExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTextureHighlight.png"));
	m_GroupListCtrl.SetSelExpArrowPic(_T("BuddyList\\MainPanel_FolderNode_expandTextureHighlight.png"));
	m_GroupListCtrl.SetStdGGHeadPic(_T("BuddyList\\trad_boy.png"));
	m_GroupListCtrl.SetStdMMHeadPic(_T("BuddyList\\trad_girl.png"));
	m_GroupListCtrl.SetStyle(BLC_STANDARD_ICON_STYLE);
	m_GroupListCtrl.SetShowBigIconInSel(TRUE);
	m_GroupListCtrl.SetBuddyTeamHeight(24);
	m_GroupListCtrl.SetBuddyItemHeightInBigIcon(54);
	m_GroupListCtrl.SetBuddyItemHeightInSmallIcon(28);
	m_GroupListCtrl.SetBuddyItemHeightInStandardIcon(26);
	m_GroupListCtrl.SetBuddyItemPadding(1);

	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcListCtrl(0, 200, rcClient.Width(), 200 + rcClient.Height() - 236);
	m_GroupListCtrl.Create(m_hWnd, rcListCtrl, NULL, WS_CHILD | WS_VISIBLE, NULL, ID_LISTCTRL_GROUP);
	m_GroupListCtrl.ShowWindow(SW_HIDE);
}

void CMainDlg::show_tab(bool isMain)
{   
	int IsMain = isMain ? SW_SHOW : SW_HIDE;
	m_TabCtrl.ShowWindow(IsMain);
	m_BuddyListCtrl.ShowWindow(IsMain);
	

	int IsLoging = isMain ? SW_HIDE:SW_SHOW;
	
	GetDlgItem(IDC_STATIC).ShowWindow(IsLoging);
	Invalidate();
}
void CMainDlg::updataBuddyList()
{
	int nTeamIndex = m_BuddyListCtrl.AddBuddyTeam(0);
	m_BuddyListCtrl.SetBuddyTeamName(nTeamIndex, _T("我的好友"));
	/*	m_BuddyListCtrl.SetBuddyTeamCurCnt(nTeamIndex, nOnlineBuddyCount);
		m_BuddyListCtrl.SetBuddyTeamExpand(nTeamIndex, aryTeamExpandStatus[i]);*/

	std::map<UINT, buddyInfo>* buddyIfos=AppModel->getBuddyIfo();

	for (auto& buddy : (*buddyIfos))
	{
		int nIndex = m_BuddyListCtrl.AddBuddyItem(nTeamIndex, buddy.first);
		m_BuddyListCtrl.SetBuddyItemMarkName(nTeamIndex, nIndex, buddy.second.getName());
	}

	Invalidate();
}
void CMainDlg::updataGroupList()
{
	int nTeamIndex = m_GroupListCtrl.AddBuddyTeam(0);
	m_GroupListCtrl.SetBuddyTeamName(nTeamIndex, _T("我的群聊"));
	/*	m_BuddyListCtrl.SetBuddyTeamCurCnt(nTeamIndex, nOnlineBuddyCount);
		m_BuddyListCtrl.SetBuddyTeamExpand(nTeamIndex, aryTeamExpandStatus[i]);*/

	std::map<UINT, groupInfo>* groupIfos = AppModel->getGroupsIfo();

	for (auto& group : (*groupIfos))
	{
		int nIndex = m_GroupListCtrl.AddBuddyItem(nTeamIndex, group.first);
		m_GroupListCtrl.SetBuddyItemMarkName(nTeamIndex, nIndex, group.second.getName());
	}

	Invalidate();
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBuddyListDblClk(LPNMHDR pnmh)
{
	int nTeamIndex, nIndex;
	m_BuddyListCtrl.GetCurSelIndex(nTeamIndex, nIndex);

	if (nTeamIndex != -1 && nIndex != -1)
	{
		UINT account = m_BuddyListCtrl.GetBuddyItemID(nTeamIndex, nIndex);
		if (!AppModel->getBuddyChatDialog(account))
		{
			BuddyChatDialog* temp = new BuddyChatDialog(account, AppModel->getBuddyNameByAccount(account));
			// 创建对话框窗口（非模态）
			if (temp->Create(nullptr) == NULL) {
				// 如果创建失败，删除对话框指针
				delete temp;
				return 0;
			}
			// 显示对话框
			temp->ShowWindow(SW_SHOW);
			AppModel->addBuddyChatDialog(account, temp);
		}
	}
	return 0;
}

LRESULT CMainDlg::OnGroupListDblClk(LPNMHDR pnmh)

{
	int nTeamIndex, nIndex;
	m_GroupListCtrl.GetCurSelIndex(nTeamIndex, nIndex);

	if (nTeamIndex != -1 && nIndex != -1)
	{
		UINT GroupId = m_GroupListCtrl.GetBuddyItemID(nTeamIndex, nIndex);
		if (!AppModel->getGroupChatDialog(GroupId))
		{
			GroupChatDialog* temp = new GroupChatDialog(GroupId, AppModel->getGroupNameByGroupId(GroupId));
			// 创建对话框窗口（非模态）
			if (temp->Create(nullptr) == NULL) {
				// 如果创建失败，删除对话框指针
				delete temp;
				return 0;
			}
			// 显示对话框
			temp->ShowWindow(SW_SHOW);
			AppModel->addGroupChatDialog(GroupId, temp);
		}
	}
	return 0;
}

LRESULT CMainDlg::OnTabCtrlSelChange(LPNMHDR pnmh)
{
	int nCurSel = m_TabCtrl.GetCurSel();
	switch (nCurSel)
	{
	case 0:
		//m_RecentListCtrl.ShowWindow(SW_SHOW);
		m_BuddyListCtrl.ShowWindow(SW_HIDE);
		m_GroupListCtrl.ShowWindow(SW_HIDE);
		//m_RecentListCtrl.SetBuddyTeamExpand(0, TRUE);
		//m_RecentListCtrl.SetFocus();
		break;

	case 1:
		//m_RecentListCtrl.ShowWindow(SW_HIDE);
		m_BuddyListCtrl.ShowWindow(SW_SHOW);
		m_GroupListCtrl.ShowWindow(SW_HIDE);
		m_BuddyListCtrl.SetFocus();
		break;

	case 2:
		//m_RecentListCtrl.ShowWindow(SW_HIDE);
		m_BuddyListCtrl.ShowWindow(SW_HIDE);
		m_GroupListCtrl.ShowWindow(SW_SHOW);
		m_GroupListCtrl.SetFocus();
		break;
	}


	return 0;
}