#pragma once
#include <windows.h> // ���� Windows API ���������ݽṹ
#include <atlbase.h> // ���� ATL ������ͺ�
#include <atlwin.h>  // ���� ATL ������ͺ���
#include "SkinButton.h"
#include "SkinEdit.h"
class LogIn_Dialog : public CDialogImpl<LogIn_Dialog>
{
public:
    LogIn_Dialog(CString& _Account,CString& _Pwd):Account(_Account), Pwd(_Pwd){}
    enum { IDD = IDD_DIALOG_LogIn
    };
    BEGIN_MSG_MAP_EX(LogIn_Dialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(ID_LogIn, OnBTN_LogIn)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    void CenterWindow(HWND hWnd = NULL);
    LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    void OnBTN_LogIn(UINT uNotifyCode, int nID, CWindow wndCtl);
    void InitUi();

public:

    CString& Account;
    CString& Pwd;
    CSkinButton  BTN_LogIn;
    CSkinEdit	 ETD_Accout;
    CSkinEdit	 ETD_Pwd;
};
