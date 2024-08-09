#pragma once
#include "ReceiveAckThread.h"


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
	case 1:
		HandleLogInAck(root);
		break;
	default:
		break;
	}
}
void ReceiveAckThread::HandleLogInAck(Json::Value& root)
{
	bool isSuccess = root["isSuccess"].asBool();
	if (isSuccess)
	{

	}
	else
	{

	}
}
