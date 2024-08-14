
#pragma once
#include "stdafx.h"
#include "ApplicationModel.h"
#include <chrono>
#include <iomanip>
#include "GroupChatDlg.h"
// 对话框类

GroupChatDialog::GroupChatDialog(UINT& _account, CString& _name) :account(_account), name(_name) {};
GroupChatDialog::GroupChatDialog() {};
// 控件成员变量

LRESULT GroupChatDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 在此初始化对话框控件
    ModifyStyleEx(WS_EX_APPWINDOW, 0);

    CenterWindow(m_hWnd);
    ::SetForegroundWindow(m_hWnd);
    ::SetFocus(m_hWnd);

    // 绑定控件到成员变量
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
    // 解除控件的附加
    m_editBox.Detach();
    m_staticText.Detach();
    m_button.Detach();
    m_richEditBox.Detach();
    m_listBox.Detach();

    bHandled = TRUE; // 表示消息已被处理
    return 0;

}
// 按钮点击事件处理程序
LRESULT GroupChatDialog::OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    // 获取控件的文本
    CString text;
    m_editBox.GetWindowText(text);
    m_editBox.SetWindowText(_T(""));

    AppendText(text);


    // Convert UINT to CString
    CString account_s;
    account_s.Format(_T("%u"), account);
    AppModel->sendBuddyMessageRequest(account_s, text, GetCurrentTimeFormatted());

    // 处理按钮点击事件
    return 0;
}
void GroupChatDialog::CenterWindow(HWND hWnd)
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

void GroupChatDialog::AppendText(const CString& text)
{
    // 获取控件内容的末尾位置
    LONG length = m_richEditBox.GetWindowTextLength();

    // 设置选择范围到末尾
    m_richEditBox.SetSel(length, length);

    CString newText = +_T("我:") + text + _T(" ") + GetCurrentTimeFormatted() + _T("\r\n");

    m_richEditBox.ReplaceSel(newText);
    m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
    m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
}

CString GroupChatDialog::GetCurrentTimeFormatted() {
    // 获取当前系统时间
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // 将time_t转换为tm结构
    struct tm buf;
    localtime_s(&buf, &in_time_t);

    // 使用ostringstream来格式化时间
    std::ostringstream ss;
    ss << std::put_time(&buf, "%H:%M:%S");

    // 创建一个CString
    CString formattedTime(ss.str().c_str());
    return formattedTime;
}
LRESULT  GroupChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();  // 销毁对话框
    return 0;
}

void GroupChatDialog::appandBuddyMessage(std::string& send_account, std::string& message)
{
    // 获取控件内容的末尾位置
    LONG length = m_richEditBox.GetWindowTextLength();

    // 设置选择范围到末尾
    m_richEditBox.SetSel(length, length);

    CString name = AppModel->getBuddyNameByAccount(std::stoul(send_account));
    CString content(AppModel->stringToWstring(message).c_str());
    CString newText = name + content + _T("\r\n");

    m_richEditBox.ReplaceSel(newText);
    m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
    m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);

}
