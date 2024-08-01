#pragma once
#include <windows.h> // 包含 Windows API 函数和数据结构
#include <atlbase.h> // 包含 ATL 基础类和宏
#include <atlwin.h>  // 包含 ATL 窗口类和函数
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
