
#pragma once
#include "stdafx.h"
#include "budyChatDlg.h"
#include "ApplicationModel.h"
#include <chrono>
#include <iomanip>
// 对话框类

   BuddyChatDialog::BuddyChatDialog(UINT& _account, CString& _name) :account(_account), name(_name) {};
    BuddyChatDialog::BuddyChatDialog() {};
    // 控件成员变量

    LRESULT BuddyChatDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 在此初始化对话框控件
        ModifyStyleEx(WS_EX_APPWINDOW, 0);

        CenterWindow(m_hWnd);
        ::SetForegroundWindow(m_hWnd);
        ::SetFocus(m_hWnd);
        // 绑定控件到成员变量
        m_editBox.Attach(GetDlgItem(IDC_EDIT2));
        m_staticText.Attach(GetDlgItem(IDC_STATIC));
        m_button.Attach(GetDlgItem(IDC_BUTTON1));
        m_richEditBox.Attach(GetDlgItem(IDC_RICHEDIT21));


        m_richEditBox.SetReadOnly(TRUE);
        m_staticText.SetWindowText(name);
        /*m_richEditBox.EnableWindow(FALSE);*/
        return TRUE;
    }
    LRESULT BuddyChatDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        AppModel->deleteBuddyDlgByAccount(account);
        // 解除控件的附加
        m_editBox.Detach();
        m_staticText.Detach();
        m_button.Detach();
        m_richEditBox.Detach();
        
        bHandled = TRUE; // 表示消息已被处理
        return 0;
 
    }
    // 按钮点击事件处理程序
    LRESULT BuddyChatDialog::OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        // 获取控件的文本
        CString text;
        m_editBox.GetWindowText(text);
        m_editBox.SetWindowText(_T(""));

        AppendText(text);
        // 处理按钮点击事件
        return 0;
    }
    void BuddyChatDialog::CenterWindow(HWND hWnd)
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

    void BuddyChatDialog::AppendText(const CString& text)
    {
        // 获取控件内容的末尾位置
        LONG length = m_richEditBox.GetWindowTextLength();

        // 设置选择范围到末尾
        m_richEditBox.SetSel(length, length);

        CString newText = + _T("我:")+text + _T(" ") + GetCurrentTimeFormatted() + _T("\r\n");

        m_richEditBox.ReplaceSel(newText);
        m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
        m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
    }
    CString BuddyChatDialog::GetCurrentTimeFormatted() {
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
    LRESULT  BuddyChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        DestroyWindow();  // 销毁对话框
        return 0;
    }


