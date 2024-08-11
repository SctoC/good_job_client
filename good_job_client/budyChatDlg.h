#pragma once
// MyDialog.h

#pragma once
//#include "stdafx.h"
#include <windows.h> // 包含 Windows API 函数和数据结构
#include <atlbase.h>
#include <atlwin.h>
//#include <atlctrls.h>
#include <richedit.h>
#include "resource.h"
// 对话框类
class CMyDialog_test : public CDialogImpl<CMyDialog_test>
{
public:
    enum { IDD = IDD_buddyChatDlg
    };

    // 控件成员变量
    CEdit m_editBox;
    CStatic m_staticText;
    CButton m_button;
    CRichEditCtrl m_richEditBox;

    BEGIN_MSG_MAP_EX(CMyDialog_test)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
        // 添加其他消息处理程序
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
        /*m_richEditBox.EnableWindow(FALSE);*/
        return TRUE;
    }
    LRESULT CMyDialog_test::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 解除控件的附加
        m_editBox.Detach();
        m_staticText.Detach();
        m_button.Detach();
        m_richEditBox.Detach();
        return 0;
    }
    // 按钮点击事件处理程序
    LRESULT OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        // 获取控件的文本
        CString text;
        m_editBox.GetWindowText(text);
        m_editBox.SetWindowText(_T(""));
        messages.push_back(text);
        AppendText(text);
        // 处理按钮点击事件
        return 0;
    }
    void CMyDialog_test::CenterWindow(HWND hWnd)
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
    void CMyDialog_test::SetChatContent()
    {
        // 清空现有内容
        m_richEditBox.SetWindowText(_T(""));

        // 设置文本格式
        CHARFORMAT2 cf;
        cf.cbSize = sizeof(CHARFORMAT2);
        cf.dwMask = CFM_BOLD | CFM_COLOR;
        cf.dwEffects = 0;
        cf.crTextColor = RGB(0, 0, 0); // 黑色文本
        m_richEditBox.SetDefaultCharFormat(cf);

        // 插入聊天内容
        for (const auto& message : messages)
        {
            // 插入文本并设置格式
            AppendText(message);
        }

        // 设置自动滚动到最底部
        m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
    }
    void AppendText(const CString& text)
    {
        // 获取控件内容的末尾位置
        LONG length = m_richEditBox.GetWindowTextLength();

        // 设置选择范围到末尾
        m_richEditBox.SetSel(length, length);
   
        CString newText = text + _T("\r\n");
            
        m_richEditBox.ReplaceSel(newText);
        m_richEditBox.LineScroll(m_richEditBox.GetLineCount());
        m_richEditBox.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, 0), 0);
    }

public:
    std::vector<CString> messages;
};
