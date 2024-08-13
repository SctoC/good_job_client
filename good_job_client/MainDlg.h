// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "LogIn_dlg.h"
#include "SkinTabCtrl.h"
#include "BuddyListCtrl.h"
#include "userMessage.h"
#include "budyChatDlg.h"
#include "ApplicationModel.h"
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);


	virtual BOOL OnIdle();


	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_USER_LOGIN_ACK, OnLoginResult)
		NOTIFY_HANDLER_EX(ID_LISTCTRL_BUDDY, NM_DBLCLK, OnBuddyListDblClk)		 //˫�������б��еĺ���
	END_MSG_MAP() 

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnLoginResult(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		bool* success = reinterpret_cast<bool*>(lParam);
		if(*success)
			show_tab(true);
		delete success;
		return 0;
	}

	void Init();
	void tab_Init();
	void buddyList_Init();

	void show_tab(bool isMain);

	void CloseDialog(int nVal);

	LRESULT CMainDlg::OnBuddyListDblClk(LPNMHDR pnmh)
	{
		int nTeamIndex, nIndex;
		m_BuddyListCtrl.GetCurSelIndex(nTeamIndex, nIndex);

		if (nTeamIndex != -1 && nIndex != -1)
		{
			UINT account = m_BuddyListCtrl.GetBuddyItemID(nTeamIndex, nIndex);
			if (!AppModel->getBuddyChatDialog(account))
			{
				BuddyChatDialog* temp = new BuddyChatDialog(account,AppModel->getBuddyNameByAccount(account));
				AppModel->addBuddyChatDialog(account, temp);
			}
		}
		return 0;
	}
	 CSkinTabCtrl		m_TabCtrl;
	 CBuddyListCtrl		m_BuddyListCtrl;
};
