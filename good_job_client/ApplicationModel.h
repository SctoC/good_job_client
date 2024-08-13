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
	void setMaindlgHwnd(HWND mainDlgHwnd);
	bool getBuddyChatDialog(UINT account);
	const char* unicodeToUtf_8(const wchar_t* wbuf);
	CString getBuddyNameByAccount(UINT account);
	void addBuddyChatDialog(UINT nUTalkUin, BuddyChatDialog* temp);

	void setBuddyIfo(Json::Value& root);



private:
	Socket           socket;
	SendRequestTread sendRequestThread;
	std::map<UINT, BuddyChatDialog*> map_account_chatDlg;
	std::map<UINT, buddyInfo> map_account_buddy;

};

