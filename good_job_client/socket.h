#pragma once
#include <winsock2.h>   // ���� Winsock API
#include <ws2tcpip.h>   // �ṩ IP ��ַ��صĺ���
#pragma comment(lib, "ws2_32.lib")

#include<iostream>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
//�����Ż��ɵ���ģʽ
class Socket
{
public:
	Socket();
	~Socket();
	//socket�Ĺ̶���������ʼ���⡢����socket�����÷�������ַ����ʼ���ӡ��ر����ӡ������
	bool init();
	bool creat();
	bool setServerIp(std::string ipAddress, int port);
	bool connect();
	void close();

	//��ʼ�̷߳��ͺͽ����̵߳�����
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

