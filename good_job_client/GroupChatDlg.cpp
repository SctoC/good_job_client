
#pragma once
#include "stdafx.h"
#include "ApplicationModel.h"
#include <chrono>
#include <iomanip>
#include "GroupChatDlg.h"
// �Ի�����

GroupChatDialog::GroupChatDialog(UINT& _account, CString& _name) :account(_account), name(_name) {};
GroupChatDialog::GroupChatDialog() {};
// �ؼ���Ա����

LRESULT GroupChatDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // �ڴ˳�ʼ���Ի���ؼ�
    ModifyStyleEx(WS_EX_APPWINDOW, 0);

    CenterWindow(m_hWnd);
    ::SetForegroundWindow(m_hWnd);
    ::SetFocus(m_hWnd);

    // �󶨿ؼ�����Ա����
    m_editBox.Attach(GetDlgItem(IDC_EDIT_group));
    m_staticText.Attach(GetDlgItem(IDC_STATIC_group));
    m_button.Attach(GetDlgItem(IDC_BUTTON1_group));
    m_richEditBox.Attach(GetDlgItem(IDC_RICHEDIT21_group));
    m_listBox.Attach(GetDlgItem(IDC_LISTBOX_group));

    m_richEditBox.SetReadOnly(TRUE);
    m_staticText.SetWindowText(name);


    InitMemberList();
    /*m_richEditBox.EnableWindow(FALSE);*/
    return TRUE;
}


void GroupChatDialog::InitMemberList()
{
    groupInfo* temp= AppModel->getGroupIfoByGroupId(account);

    std::vector<buddyInfoWithAccount>* buddysIfo= temp->get_members();

    for (int i = 0; i < buddysIfo->size(); i++)
    {
        CString text = (*buddysIfo)[i].getName() + _T("(") + (*buddysIfo)[i].getaccount() + _T(")");
        m_listBox.AddString(text);
    }
}

LRESULT GroupChatDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    AppModel->deleteGroupDlgByGroupID(account);
    // ����ؼ��ĸ���
    m_editBox.Detach();
    m_staticText.Detach();
    m_button.Detach();
    m_richEditBox.Detach();
    m_listBox.Detach();

    bHandled = TRUE; // ��ʾ��Ϣ�ѱ�����
    return 0;

}
// ��ť����¼��������
LRESULT GroupChatDialog::OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    // ��ȡ�ؼ����ı�
    CString text;
    m_editBox.GetWindowText(text);
    m_editBox.SetWindowText(_T(""));

    AppendText(text);


    // Convert UINT to CString
    CString account_s;
    account_s.Format(_T("%u"), account);
    AppModel->sendBuddyMessageRequest(account_s, text, GetCurrentTimeFormatted());

    // ����ť����¼�
    return 0;
}
void GroupChatDialog::CenterWindow(HWND hWnd)
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

void GroupChatDialog::AppendText(const CString& text)
{
    // ��ȡ�ؼ����ݵ�ĩβλ��
    LONG length = m_richEditBox.GetWindowTextLength();

    // ����ѡ��Χ��ĩβ
    m_richEditBox.SetSel(length, length);

    CString newText = +_T("��:") + text + _T(" ") + GetCurrentTimeFormatted() + _T("\r\n");

    m_richEditBox.ReplaceSel(newText);
    m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
    m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
}

CString GroupChatDialog::GetCurrentTimeFormatted() {
    // ��ȡ��ǰϵͳʱ��
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // ��time_tת��Ϊtm�ṹ
    struct tm buf;
    localtime_s(&buf, &in_time_t);

    // ʹ��ostringstream����ʽ��ʱ��
    std::ostringstream ss;
    ss << std::put_time(&buf, "%H:%M:%S");

    // ����һ��CString
    CString formattedTime(ss.str().c_str());
    return formattedTime;
}
LRESULT  GroupChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();  // ���ٶԻ���
    return 0;
}

void GroupChatDialog::appandBuddyMessage(std::string& send_account, std::string& message)
{
    // ��ȡ�ؼ����ݵ�ĩβλ��
    LONG length = m_richEditBox.GetWindowTextLength();

    // ����ѡ��Χ��ĩβ
    m_richEditBox.SetSel(length, length);

    CString name = AppModel->getBuddyNameByAccount(std::stoul(send_account));
    CString content(AppModel->stringToWstring(message).c_str());
    CString newText = name + content + _T("\r\n");

    m_richEditBox.ReplaceSel(newText);
    m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
    m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);

}
