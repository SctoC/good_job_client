#pragma once
#include "stdafx.h"
#include "ApplicationModel.h"

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
	
	void ApplicationModel::addBuddyChatDialog(UINT nUTalkUin, BuddyChatDialog* temp)
	{
		map_account_chatDlg[nUTalkUin] = temp;
	}

	void ApplicationModel::setBuddyIfo(Json::Value& root)
	{
		for (const auto& buddy : root) {
			int account = std::stoi(buddy["account"].asString());
			
			CString name(stringToWstring(buddy["name"].asString()).c_str());
			map_account_buddy[account] = buddyInfo(name);
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

	void ApplicationModel::deleteBuddyDlgByAccount(UINT account)
	{
		if(map_account_chatDlg.find(account)!= map_account_chatDlg.end())
			map_account_chatDlg.erase(account);
	}


