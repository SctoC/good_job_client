
#include "SendRequestTread.h"
#include "json.h" // ���� JsonCpp ���ͷ�ļ�

SendRequestTread::SendRequestTread(Socket* _sendSocket):_stop(false),sendSocket(_sendSocket)
{

	start();
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
	OutputDebugString(L"Thread function started.\n");
	//������������
	while (!_stop)
	{
		Request* temp = nullptr;
		bool is_unlock;
		mtx.lock();
		is_unlock = true;
		//�˴������Ż�Ϊ��������
	    if(!SendQue.empty())
		{
			temp = SendQue.front();
			SendQue.pop();
			mtx.unlock();
			is_unlock = false;
			HandleRequest(temp);
			delete temp;
		}
		if (is_unlock)
		{
			mtx.unlock();
		}
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
	//����json����
	Json::Value root;
	root["type"] = LogInQuest;
	root["account"] = Req->account;
	root["password"] = Req->pwd;

	// �� JSON �������л�Ϊ�ַ���
	Json::StreamWriterBuilder writer;
	std::string sendbuf1 = Json::writeString(writer, root);

    //������Ϣͷ����
	unsigned int size = sendbuf1.size();
	//const char* message_len = (const char*)&size;
	const char* message_len = reinterpret_cast<const char*>(&size);

	std::string sendbuf(message_len, sizeof(size));
	sendbuf += sendbuf1;


	sendSocket->add_sendbuf(sendbuf);
}
