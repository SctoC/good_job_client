#include "stdafx.h"
#include "LogIn_dlg.h"
#include "resource.h"
LRESULT LogIn_Dialog ::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 在此初始化对话框控件
    ModifyStyleEx(WS_EX_APPWINDOW, 0);

    CenterWindow(m_hWnd);
    ::SetForegroundWindow(m_hWnd);
    ::SetFocus(m_hWnd);

    InitUi();
    return TRUE;
}
void LogIn_Dialog::InitUi()
{

    HDC hDlgBgDC = GetDC();

    BTN_LogIn.SetButtonType(SKIN_PUSH_BUTTON);
    BTN_LogIn.SetTransparent(TRUE, hDlgBgDC);
    BTN_LogIn.SetBgPic(_T("Button\\login_btn_normal.png"), _T("Button\\login_btn_highlight.png"), _T("Button\\login_btn_down.png"), _T("Button\\login_btn_focus.png"));
    BTN_LogIn.SetTextColor(RGB(255, 255, 255));
    BTN_LogIn.SetTextBoldStyle(TRUE);
    BTN_LogIn.SubclassWindow(GetDlgItem(ID_LogIn));

    ReleaseDC(hDlgBgDC);


    ETD_Pwd.SubclassWindow(GetDlgItem(IDC_EDT_PWD));

    ETD_Accout.SubclassWindow(GetDlgItem(IDC_EDIT_Account));

}
void LogIn_Dialog::CenterWindow(HWND hWnd)
{
    RECT rcDlg;
    RECT rcOwner;
    HWND hWndParent = GetParent();

    // 获取对话框的大小
    GetWindowRect(&rcDlg);
    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;

    if (hWndParent)
    {
        // 获取父窗口的大小
        ::GetWindowRect(hWndParent, &rcOwner);
    }
    else
    {
        // 获取屏幕大小
        rcOwner.left = 0;
        rcOwner.top = 0;
        rcOwner.right = GetSystemMetrics(SM_CXSCREEN);
        rcOwner.bottom = GetSystemMetrics(SM_CYSCREEN);
    }

    // 计算新的对话框位置
    int newLeft = rcOwner.left + (rcOwner.right - rcOwner.left - dlgWidth) / 2;
    int newTop = rcOwner.top + (rcOwner.bottom - rcOwner.top - dlgHeight) / 2;

    // 设置对话框的位置
    SetWindowPos(NULL, newLeft, newTop, dlgWidth, dlgHeight, SWP_NOZORDER | SWP_NOSIZE);
}

LRESULT LogIn_Dialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT LogIn_Dialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}


void LogIn_Dialog::OnBTN_LogIn(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    //输入验证，待写。
    ETD_Accout.GetWindowText(Account);
    ETD_Pwd.GetWindowText(Pwd);
    EndDialog(IDOK);
}