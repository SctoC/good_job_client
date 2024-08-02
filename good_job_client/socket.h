#pragma once
#include <winsock2.h>   // 包含 Winsock API
#include <ws2tcpip.h>   // 提供 IP 地址相关的函数
#pragma comment(lib, "ws2_32.lib")

#include<iostream>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
//可以优化成单例模式
class Socket
{
public:
	Socket();
	~Socket();
	//socket的固定操作：初始化库、创建socket、设置服务器地址、开始连接、关闭连接、清理库
	bool init();
	bool creat();
	bool setServerIp(std::string ipAddress, int port);
	bool connect();
	void close();

	//开始线程发送和接收线程的运行
	void start();
	void stop();


	void send_threadfuntion();
	void add_sendbuf(std::string& sendbuf);


	void rece_funtion();


	SOCKET socketfd;
	struct sockaddr_in serverAddr;
	std::string m_strSendBuf;
	std::thread send_thread;
	std::mutex send_mtx;
	std::atomic<bool> _stop;
};

