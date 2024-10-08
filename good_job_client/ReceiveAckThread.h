#pragma once
#include <windows.h>
#include<queue>
#include<atomic>
#include<mutex>
#include<thread>
#include<string>
#include "json.h" // 包含 JsonCpp 库的头文件


class ReceiveAckThread
{

public:
	
	ReceiveAckThread();
	~ReceiveAckThread();

	void start();

	void setMaindlgHwnd(HWND mainDlgHwnd)
	{
		_mainDlgHwnd = mainDlgHwnd;
	}

	void stop();

	void AddAck(std::string ackData);

	void receive_threadfuntion();

	void HandleAck(std::string& jsonData);

	void HandleLogInAck(Json::Value& root);

	void HandleBuddyMessage(Json::Value& root);

	void HandleGroupMessage(Json::Value& root);

	std::thread receAckThread;

	//可以优化成智能指针
	std::queue<std::string> receiveQue;
	std::atomic<bool> _stop;
	std::mutex mtx;
	HWND  _mainDlgHwnd;
	
};