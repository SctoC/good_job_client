#pragma once

#include "ReceiveAckThread.h"
#include<iostream>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")
//可以优化成单例模式

#define BUFFER_SIZE 1024
class Socket
{
public:
	Socket();
	~Socket();
	//socket的固定操作：初始化库、创建socket、设置服务器地址、开始连接、关闭连接、清理库
	bool init(std::string& ipAddress, int& port);
	bool WinSock_init();
	bool creat();
	bool connect(std::string& ipAddress, int& port);
	void close();

	//开始线程发送和接收线程的运行
	void start();
	void stop();

	void receive_funtion();
	void send_threadfuntion();
	void add_sendbuf(std::string& sendbuf);

	void setMaindlgHwnd(HWND mainDlgHwnd)
	{
		revAckThread.setMaindlgHwnd(mainDlgHwnd);
	}
	ReceiveAckThread revAckThread;
	SOCKET* socketfd;
	std::string m_strSendBuf;
	std::string m_strReceiveBuf;
	std::thread send_thread;
	std::thread receive_thread;
	std::mutex send_mtx;
	std::atomic<bool> _stop;
	
};

