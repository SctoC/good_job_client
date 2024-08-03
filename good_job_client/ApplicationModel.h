#pragma once
#include "SendRequestTread.h"
#include "socket.h"
#define AppModel ApplicationModel::getInstance()
class ApplicationModel
{
private:
	ApplicationModel():sendRequestThread(&socket){
	}
public:
	ApplicationModel(const ApplicationModel& ) = delete;
	ApplicationModel& operator=(const ApplicationModel& )=delete;
	
	static ApplicationModel* getInstance()
	{
		//线程安全的单例模式
		static ApplicationModel m;
		return &m;
	}
	void sendLoninRequest(CString& account, CString& pwd)
	{
		const wchar_t* account_wbuf = static_cast<const wchar_t*>(account);
		const char* account_ubuf=unicodeToUtf_8(account_wbuf);
		const wchar_t* pwd_wbuf = static_cast<const wchar_t*>(pwd);
		const char* pwd_ubuf = unicodeToUtf_8(pwd);

		LogIn_Request* t = new LogIn_Request(LogInQuest,account_ubuf, pwd_ubuf);//发送线程处理完后，会释放资源，但是改成智能指针更好。
		sendRequestThread.AddRequest(static_cast<Request*>(t));
	}

	const char* unicodeToUtf_8(const wchar_t* wbuf)
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
	Socket           socket;
	SendRequestTread sendRequestThread;

};

