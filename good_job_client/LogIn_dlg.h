#pragma once
#include <windows.h> // ���� Windows API ���������ݽṹ
#include <atlbase.h> // ���� ATL ������ͺ�
#include <atlwin.h>  // ���� ATL ������ͺ���
#include "SkinButton.h"
#include "SkinEdit.h"
class CMyDialog : public CDialogImpl<CMyDialog>
{
public:
    enum { IDD = IDD_DIALOG_LogIn
    };

    BEGIN_MSG_MAP(CMyDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    void CenterWindow(HWND hWnd = NULL);
    LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    void InitUi();

public:
    CSkinButton  BTN_LogIn;
    CSkinEdit	 ETD_Accout;
    CSkinEdit	 ETD_Pwd;
};
