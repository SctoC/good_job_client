#include "SendRequestTread.h"


SendRequestTread::SendRequestTread(Socket* _sendSocket):_stop(false),sendSocket(_sendSocket)
{
}
SendRequestTread::~SendRequestTread()
{
	stop();
	SendRequestThread.join();
	//�ͷ�û���������
	while (!SendQue.empty())
	{
		Request* t = SendQue.front();
		delete t;
		SendQue.pop();
	}
}
void SendRequestTread::start()
{
	SendRequestThread = std::thread(&SendRequestTread::send_threadfuntion, this);
}
void SendRequestTread::stop()
{
	_stop = true;
}
void SendRequestTread::AddRequest(Request* req)
{
	mtx.lock();
	SendQue.push(req);
	mtx.unlock();
}
void SendRequestTread::send_threadfuntion()
{
	//������������
	while (!_stop)
	{
		Request* temp = nullptr;
		mtx.lock();
		//�˴������Ż�Ϊ��������
		while (SendQue.empty()) {
			mtx.unlock();
			if (_stop)
				return;
			mtx.unlock();
		}
		mtx.lock();
		temp = SendQue.front();
		SendQue.pop();
		mtx.unlock();

		HandleRequest(temp);
		delete temp;
	}
}
void SendRequestTread::HandleRequest(Request* Req)
{
	//������Կ����ö�̬
	switch (Req->type)
	{
	case LogInQuest:
		HandleLogInRequest((LogIn_Request*)Req);
		break;
	default:
		break;
	}
}
void SendRequestTread::HandleLogInRequest(LogIn_Request* Req)
{
	unsigned int size = Req->account.size() + Req->pwd.size();
	//const char* message_len = (const char*)&size;
	const char* message_len = reinterpret_cast<const char*>(&size);

	std::string sendbuf(message_len, sizeof(size));
	sendbuf += Req->account;
	sendbuf += Req->pwd;

	sendSocket->add_sendbuf(sendbuf);
}
