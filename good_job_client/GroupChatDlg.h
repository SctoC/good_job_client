
#pragma once
#include "stdafx.h"
#include <windows.h> // ���� Windows API ���������ݽṹ
#include <atlbase.h>
#include <atlwin.h>
//#include <atlctrls.h>
#include <richedit.h>
#include "resource.h"
#include <locale>
#include <codecvt>
// �Ի�����
class GroupChatDialog : public CDialogImpl<GroupChatDialog>
{
public:
    enum {
        IDD = IDD_GROUPCHATDLG
    };

    GroupChatDialog(UINT& _account, CString& _name);
    GroupChatDialog();
    // �ؼ���Ա����

    BEGIN_MSG_MAP_EX(GroupChatDialog)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_HANDLER(IDC_BUTTON1_group, BN_CLICKED, OnBnClickedButton1)
        // ���������Ϣ�������
        END_MSG_MAP()

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    // ��ť����¼��������
    LRESULT OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    void CenterWindow(HWND hWnd);

    void AppendText(const CString& text);
    CString GetCurrentTimeFormatted();
    void appandGroupMessage(std::string& send_account, std::string& message);
    void InitMemberList();
public:
    UINT account;
    CString name;
    CEdit m_editBox;
    CStatic m_staticText;
    CStatic m_staticText_userName;
    CButton m_button;
    CRichEditCtrl m_richEditBox;
    CListBox m_listBox;
};
