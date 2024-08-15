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
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		NOTIFY_HANDLER_EX(ID_LISTCTRL_BUDDY, NM_DBLCLK, OnBuddyListDblClk)		 //双击好友列表中的好友
		NOTIFY_HANDLER_EX(ID_LISTCTRL_GROUP, NM_DBLCLK, OnGroupListDblClk)		 //双击好友列表中的好友
		NOTIFY_HANDLER_EX(ID_TABCTRL_MAIN, TCN_SELCHANGE, OnTabCtrlSelChange)    //Tab选择改变
	END_MSG_MAP() 

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLoginResult(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	
	void Init();
	void tab_Init();
	void buddyList_Init();
	void groupList_Init();
	void show_tab(bool isMain);

	void CloseDialog(int nVal);

	LRESULT OnBuddyListDblClk(LPNMHDR pnmh);
	LRESULT OnGroupListDblClk(LPNMHDR pnmh);

	LRESULT OnTabCtrlSelChange(LPNMHDR pnmh);

	void updataBuddyList();
	void updataGroupList();
private:
	 CSkinTabCtrl		m_TabCtrl;
	 CBuddyListCtrl		m_BuddyListCtrl;
	 CBuddyListCtrl     m_GroupListCtrl;
};
