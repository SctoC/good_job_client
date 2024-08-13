#pragma once
#include "json.h" // 包含 JsonCpp 库的头文件
#include "socket.h"
#include "SendRequestTread.h"
#include "buddyInfo.h"
#include "budyChatDlg.h"
#define AppModel ApplicationModel::getInstance()

class ApplicationModel
{
private:
	ApplicationModel();
	
public:
	ApplicationModel(const ApplicationModel& ) = delete;
	ApplicationModel& operator=(const ApplicationModel& )=delete;
	
	static ApplicationModel* getInstance();

	bool initSocket(std::string& ipAddress, int& port);

	void sendLoninRequest(CString& account, CString& pwd);
	void sendBuddyMessageRequest(CString& buddyAccount, CString& content, CString& time);

	void setMaindlgHwnd(HWND mainDlgHwnd);
	bool getBuddyChatDialog(UINT account);
	const char* unicodeToUtf_8(const wchar_t* wbuf);
	CString getBuddyNameByAccount(UINT account);
	void addBuddyChatDialog(UINT nUTalkUin, BuddyChatDialog* temp);
	std::wstring stringToWstring(const std::string& str);
	void setBuddyIfo(Json::Value& root);
	void setAccount(CString _account);
	CString getAccount();
	std::map<UINT, buddyInfo>* getBuddyIfo();
	void deleteBuddyDlgByAccount(UINT account);

private:
	CString current_account;
	Socket           socket;
	SendRequestTread sendRequestThread;
	std::map<UINT, BuddyChatDialog*> map_account_chatDlg;
	std::map<UINT, buddyInfo> map_account_buddy;

};

