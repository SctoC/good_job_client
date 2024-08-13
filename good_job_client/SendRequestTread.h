#pragma once

#include<queue>
#include<atomic>
#include<mutex>
#include<thread>
#include<string>
#include "message.h"
#include "socket.h"

class SendRequestTread
{

public:
	SendRequestTread(Socket* _sendSocket);
	~SendRequestTread();

	void start();

	void stop();

	void AddRequest(Request* req);

	void send_threadfuntion();

	void HandleRequest(Request* Req);

	void HandleLogInRequest(LogIn_Request* Req);
	void HandleSendMessageQuest(sendMessage_Request* Req);

	std::thread SendRequestThread;

	//可以优化成智能指针
	std::queue<Request*> SendQue;
	std::atomic<bool> _stop;
	std::mutex mtx;
	Socket* sendSocket;
};