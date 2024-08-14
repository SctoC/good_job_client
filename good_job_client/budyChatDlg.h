
#pragma once
#include "stdafx.h"
#include <windows.h> // 包含 Windows API 函数和数据结构
#include <atlbase.h>
#include <atlwin.h>
//#include <atlctrls.h>
#include <richedit.h>
#include "resource.h"
#include <locale>
#include <codecvt>
// 对话框类
class BuddyChatDialog : public CDialogImpl<BuddyChatDialog>
{
public:
    enum { IDD = IDD_buddyChatDlg
    };

    BuddyChatDialog(UINT& _account, CString& _name);
    BuddyChatDialog();
    // 控件成员变量

    BEGIN_MSG_MAP_EX(BuddyChatDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
        // 添加其他消息处理程序
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    // 按钮点击事件处理程序
    LRESULT OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    void CenterWindow(HWND hWnd);

    void AppendText(const CString& text);
    CString GetCurrentTimeFormatted();
    void appandBuddyMessage(std::string& send_account, std::string& message);
public:
    UINT account;
    CString name;
    CEdit m_editBox;
    CStatic m_staticText;
    CButton m_button;
    CRichEditCtrl m_richEditBox;
};
