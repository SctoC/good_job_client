#pragma once
#include "stdafx.h"
#include "socket.h"
#include "json.h" // ���� JsonCpp ���ͷ�ļ�
#include <winsock2.h>   // ���� Winsock API
#include <ws2tcpip.h>   // �ṩ IP ��ַ��صĺ���
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
		std::cerr << "Winsock ��ʼ��ʧ�ܣ��������: " << result << std::endl;
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
			std::cerr << "�����׽���ʧ�ܣ��������: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}
	}
	return true;
}



bool Socket::connect(std::string& ipAddress, int& port) {
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); // �˿ں�
	if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
	{
		std::cerr << "��Ч�ĵ�ַ���ַ��֧��" << std::endl;
		return false;
	}// IP ��ַ
	// ���ӷ�����
	if (::connect(*socketfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "���ӵ�������ʧ�ܣ��������: " << WSAGetLastError() << std::endl;
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
	char buffer[BUFFER_SIZE];//���ջ�����
	int bytes_received;//���յ����ֽ���
	int toBeByteNum = 4;//�������ֽ���
	bool toBeIsDataLen = true;//�����յ������ݳ��ȣ���������

	u_long mode = 1; // 1 ��ʾ������ģʽ
	if (ioctlsocket(*socketfd, FIONBIO, &mode) != NO_ERROR) {
		std::cerr << "Failed to set non-blocking mode\n";
		return;
	}

	while (!_stop)
	{
		bytes_received = recv(*socketfd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_received == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// û�����ݿ��ã�����ѭ��
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
			//�����ν��յ����ݼ��뵽���ն���
			std::string temp(buffer, bytes_received);
			m_strReceiveBuf += temp;

			while (m_strReceiveBuf.size() >= toBeByteNum)//�����ն��е��ֽ������ڵ��ڴ����յ��ֽ�������������
			{
				if (toBeIsDataLen)//��ʱ���յ������ݳ���
				{
					std::memcpy(&toBeByteNum, m_strReceiveBuf.c_str(), 4);//��ȡ���ݳ���
					m_strReceiveBuf.erase(0, 4); // ��λ�� 0 ��ʼɾ�� n ���ַ�
					toBeIsDataLen = false;//��������������
				}
				else//��ʱ���յ�������
				{
					std::string jsonData = m_strReceiveBuf.substr(0, toBeByteNum);
					m_strReceiveBuf.erase(0, toBeByteNum);
					//�������������ݳ���
					toBeIsDataLen = true;
					toBeIsDataLen = 4;
					//�����ݷ��͸�����Ӧ����Ϣ�̴߳���
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
			//���ʹ���������
			//�����������
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

