#pragma once
#include "ReceiveAckThread.h"
#include "userMessage.h"
#include "message.h"
#include "ApplicationModel.h"

ReceiveAckThread::ReceiveAckThread() :_stop(false)
{
	start();
}
ReceiveAckThread::~ReceiveAckThread()
{
	stop();
	receAckThread.join();
}
void ReceiveAckThread::start()
{
	receAckThread = std::thread(&ReceiveAckThread::receive_threadfuntion, this);
}
void ReceiveAckThread::stop()
{
	_stop = true;
}
void ReceiveAckThread::AddAck(std::string ackData)
{
	mtx.lock();
	receiveQue.push(ackData);
	mtx.unlock();
}
void ReceiveAckThread::receive_threadfuntion()
{
	//������������
	while (!_stop)
	{
		bool is_unlock;
		mtx.lock();
		is_unlock = true;
		//�˴������Ż�Ϊ��������
		if (!receiveQue.empty())
		{
			std::string jsonData=receiveQue.front();
			receiveQue.pop();
			mtx.unlock();
			is_unlock = false;
			HandleAck(jsonData);
		}
		if (is_unlock)
		{
			mtx.unlock();
		}
	}
}
void ReceiveAckThread::HandleAck(std::string& jsonData )
{
	// ���� JSON ����
	Json::Value root;
	Json::CharReaderBuilder reader;
	std::string errs;

	int type = 0;
	std::istringstream ss(jsonData);
	if (Json::parseFromStream(reader, ss, &root, &errs)) {
		// ���ʽ������ JSON ����
		type = root["type"].asInt();
	}
	//������Կ����ö�̬
	switch (type)
	{
	case LogInAck:
		HandleLogInAck(root);
		break;
	case buddyMessage:
		HandleBuddyMessage(root);
		break;
	default:
		break;
	}
}
void ReceiveAckThread::HandleLogInAck(Json::Value& root)
{
	bool* isSuccess = new bool(root["isSuccess"].asBool());
	if (*isSuccess)
	{
		AppModel->setBuddyIfo(root["buddys"]);
		AppModel->setGroupIfo(root["groups"]);
	}
	Sleep(1000);
    PostMessage(_mainDlgHwnd, WM_USER_LOGIN_ACK, 0, reinterpret_cast<LPARAM>(isSuccess));
}
void ReceiveAckThread::HandleBuddyMessage(Json::Value& root)
{
	AppModel->submitChatDlg(root);
}
