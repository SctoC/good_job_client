#include "stdafx.h"
#include "LogIn_dlg.h"
#include "stdafx.h"
#include "resource.h"
LRESULT CMyDialog ::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // �ڴ˳�ʼ���Ի���ؼ�
    ModifyStyleEx(WS_EX_APPWINDOW, 0);

    CenterWindow(m_hWnd);
    ::SetForegroundWindow(m_hWnd);
    ::SetFocus(m_hWnd);

    InitUi();
    return TRUE;
}
void CMyDialog::InitUi()
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
void CMyDialog::CenterWindow(HWND hWnd)
{
    RECT rcDlg;
    RECT rcOwner;
    HWND hWndParent = GetParent();

    // ��ȡ�Ի���Ĵ�С
    GetWindowRect(&rcDlg);
    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;

    if (hWndParent)
    {
        // ��ȡ�����ڵĴ�С
        ::GetWindowRect(hWndParent, &rcOwner);
    }
    else
    {
        // ��ȡ��Ļ��С
        rcOwner.left = 0;
        rcOwner.top = 0;
        rcOwner.right = GetSystemMetrics(SM_CXSCREEN);
        rcOwner.bottom = GetSystemMetrics(SM_CYSCREEN);
    }

    // �����µĶԻ���λ��
    int newLeft = rcOwner.left + (rcOwner.right - rcOwner.left - dlgWidth) / 2;
    int newTop = rcOwner.top + (rcOwner.bottom - rcOwner.top - dlgHeight) / 2;

    // ���öԻ����λ��
    SetWindowPos(NULL, newLeft, newTop, dlgWidth, dlgHeight, SWP_NOZORDER | SWP_NOSIZE);
}

LRESULT CMyDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT CMyDialog::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}