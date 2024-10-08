#pragma once
#include "json.h" // 包含 JsonCpp 库的头文件
#include "socket.h"
#include "SendRequestTread.h"
#include "buddyInfo.h"
#include "budyChatDlg.h"
#include "GroupChatDlg.h"
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
	void initDb();

	void sendLoninRequest(CString& account, CString& pwd);
	void sendBuddyMessageRequest(CString& buddyAccount, CString& content, CString& time);
	void sendGroupMessageRequest(CString& GroupId, CString& content, CString& time);

	void setMaindlgHwnd(HWND mainDlgHwnd);

	bool getBuddyChatDialog(UINT account);
	bool getGroupChatDialog(UINT groupId);

	const char* unicodeToUtf_8(const wchar_t* wbuf);

	CString getBuddyNameByAccount(UINT account);
	CString getGroupNameByGroupId(UINT GroupId);

	void addBuddyChatDialog(UINT nUTalkUin, BuddyChatDialog* temp);
	void addGroupChatDialog(UINT nUTalkUin, GroupChatDialog* temp);

	std::wstring stringToWstring(const std::string& str);
	void setBuddyIfo(Json::Value& root);
	void setGroupIfo(Json::Value& root);
	void setAccount(CString _account);
	CString getAccount();
	std::map<UINT, buddyInfo>* getBuddyIfo();
	std::map<UINT, groupInfo>* getGroupsIfo();
	groupInfo* getGroupIfoByGroupId(UINT GroupId);
	void deleteBuddyDlgByAccount(UINT account);
	void deleteGroupDlgByGroupID(UINT groupID);
	void submitChatDlg(Json::Value& root);
	void submitGroupChatDlg(Json::Value& root);

	CString get_current_account();

	void saveBuddyMessage(CString& is_sent,CString& buddy_account, CString& buddy_name, CString&  text, CString& time);
	void saveBuddyMessage(std::string& is_sent, std::string& buddy_account, CString& buddy_name, std::string& text, std::string& time);
	void saveGroupMessage(CString& groupId, CString& Content, CString& time);
	void saveGroupMessage(std::string& group_id, std::string& send_account, std::string& content, std::string& time);
private:
	CString current_account;
	Socket           socket;
	SendRequestTread sendRequestThread;
	std::map<UINT, BuddyChatDialog*> map_account_chatDlg;
	std::map<UINT, GroupChatDialog*> map_groupID_chatDlg;
	std::map<UINT, buddyInfo> map_account_buddy;
	std::map<UINT, groupInfo> map_groupId_group;

	std::string dbPath;

};

