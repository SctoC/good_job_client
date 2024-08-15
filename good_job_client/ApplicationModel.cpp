#pragma once
#include "stdafx.h"
#include "ApplicationModel.h"
#include <filesystem>
#include "SQLiteWrapper.h"
ApplicationModel::ApplicationModel() :sendRequestThread(&socket) {}
	
    ApplicationModel* ApplicationModel::getInstance()
	{
		//�̰߳�ȫ�ĵ���ģʽ
		static ApplicationModel m;
		return &m;
	}
	bool ApplicationModel::initSocket(std::string& ipAddress, int& port)
	{
		return socket.init(ipAddress, port);
	}
	void ApplicationModel::sendLoninRequest(CString& account, CString& pwd)
	{
		const wchar_t* account_wbuf = static_cast<const wchar_t*>(account);
		const char* account_ubuf = unicodeToUtf_8(account_wbuf);
		const wchar_t* pwd_wbuf = static_cast<const wchar_t*>(pwd);
		const char* pwd_ubuf = unicodeToUtf_8(pwd);

		LogIn_Request* t = new LogIn_Request(LogInQuest, account_ubuf, pwd_ubuf);//�����̴߳�����󣬻��ͷ���Դ�����Ǹĳ�����ָ����á�
		sendRequestThread.AddRequest(static_cast<Request*>(t));
	}
	void ApplicationModel::sendBuddyMessageRequest(CString& buddyAccount, CString& content, CString& time)
	{

		const wchar_t* current_account_wbuf = static_cast<const wchar_t*>(current_account);
		const char* current_account_ubuf = unicodeToUtf_8(current_account_wbuf);

		const wchar_t* buddyAccount_wbuf = static_cast<const wchar_t*>(buddyAccount);
		const char* buddyAccount_ubuf = unicodeToUtf_8(buddyAccount_wbuf);

		const wchar_t* content_wbuf = static_cast<const wchar_t*>(content);
		const char* content_ubuf = unicodeToUtf_8(content_wbuf);

		const wchar_t* time_wbuf = static_cast<const wchar_t*>(time);
		const char* time_ubuf = unicodeToUtf_8(time_wbuf);

		sendMessage_Request* t = new sendMessage_Request(sendMessageQuest, current_account_ubuf, buddyAccount_ubuf, content_ubuf, time_ubuf);//�����̴߳�����󣬻��ͷ���Դ�����Ǹĳ�����ָ����á�
		sendRequestThread.AddRequest(static_cast<Request*>(t));
	}
	void ApplicationModel::sendGroupMessageRequest(CString& GroupId, CString& content, CString& time)
	{
			const wchar_t* current_account_wbuf = static_cast<const wchar_t*>(current_account);
			const char* current_account_ubuf = unicodeToUtf_8(current_account_wbuf);

			const wchar_t* GroupId_wbuf = static_cast<const wchar_t*>(GroupId);
			const char* GroupId_ubuf = unicodeToUtf_8(GroupId_wbuf);

			const wchar_t* content_wbuf = static_cast<const wchar_t*>(content);
			const char* content_ubuf = unicodeToUtf_8(content_wbuf);

			const wchar_t* time_wbuf = static_cast<const wchar_t*>(time);
			const char* time_ubuf = unicodeToUtf_8(time_wbuf);

			sendGroupMessage_Request* t = new sendGroupMessage_Request(sendGroupMessageQuest, current_account_ubuf, GroupId_ubuf, content_ubuf, time_ubuf);//�����̴߳�����󣬻��ͷ���Դ�����Ǹĳ�����ָ����á�
			
			sendRequestThread.AddRequest(static_cast<Request*>(t));

	}
	void ApplicationModel::setMaindlgHwnd(HWND mainDlgHwnd)
	{
		socket.setMaindlgHwnd(mainDlgHwnd);
	}
	bool ApplicationModel::getBuddyChatDialog(UINT account)//�ж��Ƿ���������ѵĶԻ���
	{
		if (map_account_chatDlg.find(account) != map_account_chatDlg.end())
			return true;
		return false;
	}
	bool ApplicationModel::getGroupChatDialog(UINT groupId)
	{
		if (map_groupID_chatDlg.find(groupId) != map_groupID_chatDlg.end())
			return true;
		return false;
	}

	void ApplicationModel::initDb()
	{
		// ������Ŀ¼ 'a'
		const wchar_t* account_wbuf = static_cast<const wchar_t*>(current_account);
		const char* account_ubuf = unicodeToUtf_8(account_wbuf);
		std::filesystem::path dir = "./"+ std::string(account_ubuf);
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}
		// ָ�����ݿ�·�������� SQLiteWrapper ����
		dbPath = dir.string() + "/chat_history.db";
		SQLiteWrapper db(dbPath);
		//�������������¼��
		std::string sql =
			"CREATE TABLE IF NOT EXISTS ChatHistory ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, " // ������Ψһ��ʶ��
			"is_sent BOOLEAN NOT NULL, "             // ��/���ͱ�־ (1: ����, 0: ����)
			"friend_account VARCHAR(255) NOT NULL, " // �����˺�
			"friend_name VARCHAR(255) NOT NULL, "    // ��������
			"message TEXT NOT NULL, "                // �������ݣ����޳���
			"timestamp VARCHAR(20) NOT NULL"         // ʱ�䣬ʹ�� VARCHAR �洢������ 20 �ַ�
			");";

		//����Ⱥ�����¼��
		std::string sql1 = R"(
        CREATE TABLE IF NOT EXISTS GroupChatRecords (
            GroupID VARCHAR(50) NOT NULL,         -- ȺID��ʹ��VARCHAR���ͣ���󳤶�Ϊ50���ַ�
            Sender VARCHAR(255) NOT NULL,         -- �����ߣ���󳤶�Ϊ255���ַ�
            Content TEXT NOT NULL,                -- ���ݣ�TEXT��������洢�ϳ����ı�
            Time VARCHAR(50) NOT NULL             -- ʱ�䣬ʹ���ַ������ʹ洢������50�ַ������磺2024.07.21 12:34:56��
        );
    )";
		db.execute(sql); // ������� SQLiteWrapper ������һ�� execute ������ִ�� SQL ���
		db.execute(sql1); // ������� SQLiteWrapper ������һ�� execute ������ִ�� SQL ���
	}

	const char* ApplicationModel::unicodeToUtf_8(const wchar_t* wbuf)
	{
		//������Ҫ���Ļ�����
		//�����㹻��Ļ�����������ת����Ľṹ��
		//��ʼת��
		//return��

		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, nullptr, 0, nullptr, nullptr);

		// ���仺����
		char* utf8Str = new char[bufferSize];

		// ִ��ת��
		int result = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, utf8Str, bufferSize, nullptr, nullptr);
		return utf8Str;
	}
	std::wstring ApplicationModel::stringToWstring(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
		std::wstring wideString(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wideString[0], size_needed);
		return wideString;
	}
	CString ApplicationModel::getBuddyNameByAccount(UINT account)
	{
		return map_account_buddy[account].getName();
	}

	CString ApplicationModel::getGroupNameByGroupId(UINT GroupId)
	{
		return map_groupId_group[GroupId].getName();
	}
	
	void ApplicationModel::addBuddyChatDialog(UINT nUTalkUin, BuddyChatDialog* temp)
	{
		map_account_chatDlg[nUTalkUin] = temp;
	}
	void ApplicationModel::addGroupChatDialog(UINT nUTalkUin, GroupChatDialog* temp)
	{
		map_groupID_chatDlg[nUTalkUin] = temp;
	}
	
	void ApplicationModel::setBuddyIfo(Json::Value& root)
	{
		for (const auto& buddy : root) {
			UINT account = std::stoi(buddy["account"].asString());
			
			CString name(stringToWstring(buddy["name"].asString()).c_str());
			map_account_buddy[account] = buddyInfo(name);
		}
	}
	//group["groupId"] = row[0];//Ⱥ��ID
	//group["groupName"] = row[1];//Ⱥ���ǳ�
	//group["members"] = buddys;//Ⱥ��Ա
	//   buddy["account"] = row[0];//Ⱥ��Ա�˺�
	//   buddy["Name"] = row[1];//Ⱥ��Ա�ǳ�
	void ApplicationModel::setGroupIfo(Json::Value& root)
	{
		for (const auto& group : root) {
			UINT groupId = std::stoi(group["groupId"].asString());
			CString groupName(stringToWstring(group["groupName"].asString()).c_str());
			groupInfo group_temp(groupName);

			for (const auto& member : group["members"]) {
				CString account(stringToWstring(member["account"].asString()).c_str());
				CString Name(stringToWstring(member["Name"].asString()).c_str());
				group_temp.members_push(buddyInfoWithAccount(account, Name));
			}

			map_groupId_group[groupId] = group_temp;
		}
	}

	void ApplicationModel::setAccount(CString _account)
	{
		current_account = _account;
	}
	CString ApplicationModel::getAccount()
	{
		return current_account;
	}

	std::map<UINT, buddyInfo>* ApplicationModel::getBuddyIfo()
	{
			return &map_account_buddy;
	}
	std::map<UINT, groupInfo>* ApplicationModel::getGroupsIfo()
	{
		return &map_groupId_group;
	}
	groupInfo* ApplicationModel::getGroupIfoByGroupId(UINT GroupId)
	{
		return &map_groupId_group[GroupId];
	}

	void ApplicationModel::deleteBuddyDlgByAccount(UINT account)
	{
		if(map_account_chatDlg.find(account)!= map_account_chatDlg.end())
			map_account_chatDlg.erase(account);
	}
	void ApplicationModel::deleteGroupDlgByGroupID(UINT groupID)
	{
		if (map_groupID_chatDlg.find(groupID) != map_groupID_chatDlg.end())
			map_groupID_chatDlg.erase(groupID);
	}
	void ApplicationModel::submitChatDlg(Json::Value& root)
	{
		std::string send_account = root["send_account"].asString();
		std::string content = root["content"].asString();
		std::string time = root["time"].asString();

		std::string is_sent = "0";
		AppModel->saveBuddyMessage(is_sent, send_account, AppModel->getBuddyNameByAccount(std::stoul(send_account)), content, time);
		
		std::string message =":" + content + " " + time;

		auto it = map_account_chatDlg.find(std::stoul(send_account));
		if ( it!= map_account_chatDlg.end())
		{
			it->second->appandBuddyMessage(send_account, message);
		}
	}

	void ApplicationModel::submitGroupChatDlg(Json::Value& root)
	{
		std::string send_account = root["send_account"].asString();
		std::string content = root["content"].asString();
		std::string time = root["time"].asString();
		std::string group_id= root["group_id"].asString();
	
		std::string message = ":" + content + " " + time;

		AppModel->saveGroupMessage(group_id, send_account,content,time);


		auto it = map_groupID_chatDlg.find(std::stoul(group_id));
		if (it != map_groupID_chatDlg.end())
		{
			it->second->appandGroupMessage(send_account, message);
		}
	}

	CString ApplicationModel::get_current_account()
	{
		return current_account;
	}

	void ApplicationModel::saveBuddyMessage(CString& is_sent,CString& buddy_account, CString& buddy_name, CString& text, CString& time) {

		SQLiteWrapper db(dbPath); 

		// ���� SQL ������䣬ʹ�� ? ռλ����Ϊ����
		std::string sql = R"(
        INSERT INTO ChatHistory (is_sent, friend_account, friend_name, message, timestamp)
        VALUES (?, ?, ?, ?, ?);
    )";
		// ��ȡϵͳ��ǰʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);

		// ��ʽ������Ϊ "YYYY.MM.DD" ��ʽ
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// ת��Ϊ�ַ���
		std::string date_str = oss.str();
		date_str += " ";
		date_str += std::string(CT2A(time));

		// �� CString ת��Ϊ std::string
		std::vector<std::string> params;
		params.push_back(std::string(CT2A(is_sent)));  // is_sent �̶�Ϊ 1����ʾ����
		params.push_back(std::string(CT2A(buddy_account)));
		params.push_back(std::string(CT2A(buddy_name)));
		params.push_back(std::string(CT2A(text)));
		params.push_back(date_str);

		// ִ�в������
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}

	void ApplicationModel::saveBuddyMessage(std::string& is_sent, std::string& buddy_account, CString& buddy_name, std::string& text, std::string& time) {

		SQLiteWrapper db(dbPath);

		// ���� SQL ������䣬ʹ�� ? ռλ����Ϊ����
		std::string sql = R"(
        INSERT INTO ChatHistory (is_sent, friend_account, friend_name, message, timestamp)
        VALUES (?, ?, ?, ?, ?);
    )";
		// ��ȡϵͳ��ǰʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);

		// ��ʽ������Ϊ "YYYY.MM.DD" ��ʽ
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// ת��Ϊ�ַ���
		std::string date_str = oss.str();
		date_str += " ";
		date_str += time;

		// �� CString ת��Ϊ std::string
		std::vector<std::string> params;
		params.push_back(is_sent);  // is_sent �̶�Ϊ 1����ʾ����
		params.push_back(buddy_account);
		params.push_back(std::string(CT2A(buddy_name)));
		params.push_back(text);
		params.push_back(date_str);

		// ִ�в������
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}

	void ApplicationModel::saveGroupMessage(CString& groupId, CString& Content, CString& time) {

		SQLiteWrapper db(dbPath);

		// ���� SQL ������䣬ʹ�� ? ռλ����Ϊ����
		std::string sql = R"(
        INSERT INTO GroupChatRecords (GroupID, Sender, Content, Time)
        VALUES (?, ?, ?, ?);
    )";
		// ��ȡϵͳ��ǰʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);

		// ��ʽ������Ϊ "YYYY.MM.DD" ��ʽ
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// ת��Ϊ�ַ���
		std::string date_str = oss.str();
		date_str += " ";
		date_str += std::string(CT2A(time));

		// �� CString ת��Ϊ std::string
		std::vector<std::string> params;
		params.push_back(std::string(CT2A(groupId)));  // is_sent �̶�Ϊ 1����ʾ����
		params.push_back(std::string(CT2A(current_account)));
		params.push_back(std::string(CT2A(Content)));
		params.push_back(date_str);

		// ִ�в������
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}

	void ApplicationModel::saveGroupMessage(std::string& group_id, std::string& send_account, std::string& content, std::string& time)
	{

		SQLiteWrapper db(dbPath);

		// ���� SQL ������䣬ʹ�� ? ռλ����Ϊ����
		std::string sql = R"(
        INSERT INTO GroupChatRecords (GroupID, Sender, Content, Time)
        VALUES (?, ?, ?, ?);
    )";
		// ��ȡϵͳ��ǰʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);

		// ��ʽ������Ϊ "YYYY.MM.DD" ��ʽ
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// ת��Ϊ�ַ���
		std::string date_str = oss.str();
		date_str += " ";
		date_str += time;

		// �� CString ת��Ϊ std::string
		std::vector<std::string> params;
		params.push_back(group_id); // is_sent �̶�Ϊ 1����ʾ����
		params.push_back(send_account);
		params.push_back(content);
		params.push_back(date_str);

		// ִ�в������
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}