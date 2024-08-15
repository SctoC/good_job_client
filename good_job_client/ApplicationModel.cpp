#pragma once
#include "stdafx.h"
#include "ApplicationModel.h"
#include <filesystem>
#include "SQLiteWrapper.h"
ApplicationModel::ApplicationModel() :sendRequestThread(&socket) {}
	
    ApplicationModel* ApplicationModel::getInstance()
	{
		//线程安全的单例模式
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

		LogIn_Request* t = new LogIn_Request(LogInQuest, account_ubuf, pwd_ubuf);//发送线程处理完后，会释放资源，但是改成智能指针更好。
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

		sendMessage_Request* t = new sendMessage_Request(sendMessageQuest, current_account_ubuf, buddyAccount_ubuf, content_ubuf, time_ubuf);//发送线程处理完后，会释放资源，但是改成智能指针更好。
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

			sendGroupMessage_Request* t = new sendGroupMessage_Request(sendGroupMessageQuest, current_account_ubuf, GroupId_ubuf, content_ubuf, time_ubuf);//发送线程处理完后，会释放资源，但是改成智能指针更好。
			
			sendRequestThread.AddRequest(static_cast<Request*>(t));

	}
	void ApplicationModel::setMaindlgHwnd(HWND mainDlgHwnd)
	{
		socket.setMaindlgHwnd(mainDlgHwnd);
	}
	bool ApplicationModel::getBuddyChatDialog(UINT account)//判断是否有输入好友的对话框
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
		// 创建子目录 'a'
		const wchar_t* account_wbuf = static_cast<const wchar_t*>(current_account);
		const char* account_ubuf = unicodeToUtf_8(account_wbuf);
		std::filesystem::path dir = "./"+ std::string(account_ubuf);
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}
		// 指定数据库路径并创建 SQLiteWrapper 对象
		dbPath = dir.string() + "/chat_history.db";
		SQLiteWrapper db(dbPath);
		//创建好友聊天记录表
		std::string sql =
			"CREATE TABLE IF NOT EXISTS ChatHistory ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, " // 自增的唯一标识符
			"is_sent BOOLEAN NOT NULL, "             // 收/发送标志 (1: 发送, 0: 接收)
			"friend_account VARCHAR(255) NOT NULL, " // 好友账号
			"friend_name VARCHAR(255) NOT NULL, "    // 好友姓名
			"message TEXT NOT NULL, "                // 聊天内容（无限长）
			"timestamp VARCHAR(20) NOT NULL"         // 时间，使用 VARCHAR 存储，长度 20 字符
			");";

		//创建群聊天记录表
		std::string sql1 = R"(
        CREATE TABLE IF NOT EXISTS GroupChatRecords (
            GroupID VARCHAR(50) NOT NULL,         -- 群ID，使用VARCHAR类型，最大长度为50个字符
            Sender VARCHAR(255) NOT NULL,         -- 发送者，最大长度为255个字符
            Content TEXT NOT NULL,                -- 内容，TEXT类型允许存储较长的文本
            Time VARCHAR(50) NOT NULL             -- 时间，使用字符串类型存储，长度50字符（例如：2024.07.21 12:34:56）
        );
    )";
		db.execute(sql); // 假设你的 SQLiteWrapper 类中有一个 execute 函数来执行 SQL 语句
		db.execute(sql1); // 假设你的 SQLiteWrapper 类中有一个 execute 函数来执行 SQL 语句
	}

	const char* ApplicationModel::unicodeToUtf_8(const wchar_t* wbuf)
	{
		//计算需要多大的缓冲区
		//分配足够大的缓冲区来保存转换后的结构，
		//开始转换
		//return。

		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, nullptr, 0, nullptr, nullptr);

		// 分配缓冲区
		char* utf8Str = new char[bufferSize];

		// 执行转换
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
	//group["groupId"] = row[0];//群聊ID
	//group["groupName"] = row[1];//群聊昵称
	//group["members"] = buddys;//群成员
	//   buddy["account"] = row[0];//群成员账号
	//   buddy["Name"] = row[1];//群成员昵称
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

		// 定义 SQL 插入语句，使用 ? 占位符作为参数
		std::string sql = R"(
        INSERT INTO ChatHistory (is_sent, friend_account, friend_name, message, timestamp)
        VALUES (?, ?, ?, ?, ?);
    )";
		// 获取系统当前时间
		SYSTEMTIME st;
		GetLocalTime(&st);

		// 格式化日期为 "YYYY.MM.DD" 形式
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// 转换为字符串
		std::string date_str = oss.str();
		date_str += " ";
		date_str += std::string(CT2A(time));

		// 将 CString 转换为 std::string
		std::vector<std::string> params;
		params.push_back(std::string(CT2A(is_sent)));  // is_sent 固定为 1，表示发送
		params.push_back(std::string(CT2A(buddy_account)));
		params.push_back(std::string(CT2A(buddy_name)));
		params.push_back(std::string(CT2A(text)));
		params.push_back(date_str);

		// 执行插入操作
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}

	void ApplicationModel::saveBuddyMessage(std::string& is_sent, std::string& buddy_account, CString& buddy_name, std::string& text, std::string& time) {

		SQLiteWrapper db(dbPath);

		// 定义 SQL 插入语句，使用 ? 占位符作为参数
		std::string sql = R"(
        INSERT INTO ChatHistory (is_sent, friend_account, friend_name, message, timestamp)
        VALUES (?, ?, ?, ?, ?);
    )";
		// 获取系统当前时间
		SYSTEMTIME st;
		GetLocalTime(&st);

		// 格式化日期为 "YYYY.MM.DD" 形式
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// 转换为字符串
		std::string date_str = oss.str();
		date_str += " ";
		date_str += time;

		// 将 CString 转换为 std::string
		std::vector<std::string> params;
		params.push_back(is_sent);  // is_sent 固定为 1，表示发送
		params.push_back(buddy_account);
		params.push_back(std::string(CT2A(buddy_name)));
		params.push_back(text);
		params.push_back(date_str);

		// 执行插入操作
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}

	void ApplicationModel::saveGroupMessage(CString& groupId, CString& Content, CString& time) {

		SQLiteWrapper db(dbPath);

		// 定义 SQL 插入语句，使用 ? 占位符作为参数
		std::string sql = R"(
        INSERT INTO GroupChatRecords (GroupID, Sender, Content, Time)
        VALUES (?, ?, ?, ?);
    )";
		// 获取系统当前时间
		SYSTEMTIME st;
		GetLocalTime(&st);

		// 格式化日期为 "YYYY.MM.DD" 形式
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// 转换为字符串
		std::string date_str = oss.str();
		date_str += " ";
		date_str += std::string(CT2A(time));

		// 将 CString 转换为 std::string
		std::vector<std::string> params;
		params.push_back(std::string(CT2A(groupId)));  // is_sent 固定为 1，表示发送
		params.push_back(std::string(CT2A(current_account)));
		params.push_back(std::string(CT2A(Content)));
		params.push_back(date_str);

		// 执行插入操作
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

		// 定义 SQL 插入语句，使用 ? 占位符作为参数
		std::string sql = R"(
        INSERT INTO GroupChatRecords (GroupID, Sender, Content, Time)
        VALUES (?, ?, ?, ?);
    )";
		// 获取系统当前时间
		SYSTEMTIME st;
		GetLocalTime(&st);

		// 格式化日期为 "YYYY.MM.DD" 形式
		std::ostringstream oss;
		oss << st.wYear << '.'
			<< std::setw(2) << std::setfill('0') << st.wMonth << '.'
			<< std::setw(2) << std::setfill('0') << st.wDay;

		// 转换为字符串
		std::string date_str = oss.str();
		date_str += " ";
		date_str += time;

		// 将 CString 转换为 std::string
		std::vector<std::string> params;
		params.push_back(group_id); // is_sent 固定为 1，表示发送
		params.push_back(send_account);
		params.push_back(content);
		params.push_back(date_str);

		// 执行插入操作
		if (!db.executeP(sql, params)) {
			std::cerr << "Failed to insert data." << std::endl;
		}
		else {
			std::cout << "Data inserted successfully." << std::endl;
		}
	}