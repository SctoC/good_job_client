#pragma once
#include "stdafx.h"
#include "socket.h"
#include "json.h" // 包含 JsonCpp 库的头文件
#include <winsock2.h>   // 包含 Winsock API
#include <ws2tcpip.h>   // 提供 IP 地址相关的函数
#include "ApplicationModel.h" 

Socket::Socket() :socketfd(nullptr), _stop(false){}


Socket::~Socket() {
	stop();
	send_thread.join();
	receive_thread.join();
	close();
	WSACleanup();
	if(socketfd)
	  delete socketfd;
}
bool Socket::init(std::string& ipAddress, int& port)
{
	WinSock_init();
	creat();
	
	connect(ipAddress, port);
	start();
	return true;
}
bool Socket::WinSock_init()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "Winsock 初始化失败，错误代码: " << result << std::endl;
		return false;
	}
	return true;
}

bool Socket::creat()
{
	if (socketfd == nullptr) {
		socketfd = new SOCKET();
		(*socketfd) = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ((*socketfd) == INVALID_SOCKET) {
			std::cerr << "创建套接字失败，错误代码: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}
	}
	return true;
}



bool Socket::connect(std::string& ipAddress, int& port) {
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); // 端口号
	if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
	{
		std::cerr << "无效的地址或地址不支持" << std::endl;
		return false;
	}// IP 地址
	// 连接服务器
	if (::connect(*socketfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "连接到服务器失败，错误代码: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}
void Socket::close()
{
	if ((*socketfd) != INVALID_SOCKET)
	{
		closesocket(*socketfd);
	}
}

void Socket::start()
{
	send_thread = std::thread(&Socket::send_threadfuntion, this);
	receive_thread = std::thread(&Socket::receive_funtion, this);
}
void Socket::stop()
{
	_stop = true;
}
void Socket::receive_funtion()
{
	char buffer[BUFFER_SIZE];//接收缓冲区
	int bytes_received;//接收到的字节数
	int toBeByteNum = 4;//待接收字节数
	bool toBeIsDataLen = true;//待接收的是数据长度，还是数据

	u_long mode = 1; // 1 表示非阻塞模式
	if (ioctlsocket(*socketfd, FIONBIO, &mode) != NO_ERROR) {
		std::cerr << "Failed to set non-blocking mode\n";
		return;
	}

	while (!_stop)
	{
		bytes_received = recv(*socketfd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// 没有数据可用，继续循环
				continue;
			}
			else {
				std::cerr << "Recv failed with error: " << WSAGetLastError() << "\n";
				break;
			}
		}
		else if (bytes_received == 0) {
			std::cout << "Connection closed by peer\n";
			break;
		}

		else {
			//将本次接收的数据加入到接收队列
			std::string temp(buffer, bytes_received);
			m_strReceiveBuf += temp;

			while (m_strReceiveBuf.size() >= toBeByteNum)//当接收队列的字节数大于等于待接收的字节数，处理数据
			{
				if (toBeIsDataLen)//此时接收的是数据长度
				{
					std::memcpy(&toBeByteNum, m_strReceiveBuf.c_str(), 4);//提取数据长度
					m_strReceiveBuf.erase(0, 4); // 从位置 0 开始删除 n 个字符
					toBeIsDataLen = false;//接下来接收数据
				}
				else//此时接收的是数据
				{
					std::string jsonData = m_strReceiveBuf.substr(0, toBeByteNum);
					m_strReceiveBuf.erase(0, toBeByteNum);
					//接下来接收数据长度
					toBeIsDataLen = true;
					toBeIsDataLen = 4;
					//将数据发送给接收应答消息线程处理
					revAckThread.AddAck(jsonData);
				}

			}

		}

	}
}
void Socket::send_threadfuntion()
{
	while (!_stop)
	{
		send_mtx.lock();
		if(m_strSendBuf.empty()) {
		}
		else
		{
			int result = send(*socketfd, m_strSendBuf.c_str(), static_cast<int>(m_strSendBuf.size()), 0);
			//发送错误处理，待加
			//清除缓冲区。
			if (result > 0)
				m_strSendBuf = m_strSendBuf.substr(result);
		}
		send_mtx.unlock();
	}
}


void Socket::add_sendbuf(std::string& sendbuf)
{
	send_mtx.lock();
	m_strSendBuf += sendbuf;
	send_mtx.unlock();
}

