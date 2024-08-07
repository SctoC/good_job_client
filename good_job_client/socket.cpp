#include "stdafx.h"
#include "socket.h"

Socket::Socket() :socketfd(INVALID_SOCKET), _stop(false) {}
Socket::~Socket() {
	stop();
	send_thread.join();
	close();
	WSACleanup();
}
bool Socket::init(std::string& ipAddress, int& port)
{
	WinSock_init();
	creat();
	setServerIp(ipAddress, port);
	connect();
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
	if (socketfd == INVALID_SOCKET) {
		socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socketfd == INVALID_SOCKET) {
			std::cerr << "�����׽���ʧ�ܣ��������: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}
	}
	return true;
}

bool Socket::setServerIp(std::string ipAddress, int port)
{
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); // �˿ں�
	if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
	{
		std::cerr << "��Ч�ĵ�ַ���ַ��֧��" << std::endl;
		return false;
	}// IP ��ַ
	return true;
}

bool Socket::connect() {
	// ���ӷ�����
	if (::connect(socketfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "���ӵ�������ʧ�ܣ��������: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}
void Socket::close()
{
	if (socketfd != INVALID_SOCKET)
	{
		closesocket(socketfd);
		socketfd = INVALID_SOCKET;
	}
}

void Socket::start()
{
	send_thread = std::thread(&Socket::send_threadfuntion, this);
}
void Socket::stop()
{
	_stop = true;
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
			int result = send(socketfd, m_strSendBuf.c_str(), static_cast<int>(m_strSendBuf.size()), 0);
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
