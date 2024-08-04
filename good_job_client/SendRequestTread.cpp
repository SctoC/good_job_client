
#include "SendRequestTread.h"
#include "json.h" // 包含 JsonCpp 库的头文件

SendRequestTread::SendRequestTread(Socket* _sendSocket):_stop(false),sendSocket(_sendSocket)
{

	start();
}
SendRequestTread::~SendRequestTread()
{
	stop();
	SendRequestThread.join();
	//释放没处理的请求
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
	//存在死锁问题
	while (!_stop)
	{
		Request* temp = nullptr;
		bool is_unlock;
		mtx.lock();
		is_unlock = true;
		//此处可以优化为条件变量
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
	//这里可以考虑用多态
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
	//构建json对象
	Json::Value root;
	root["type"] = LogInQuest;
	root["account"] = Req->account;
	root["password"] = Req->pwd;

	// 将 JSON 对象序列化为字符串
	Json::StreamWriterBuilder writer;
	std::string sendbuf1 = Json::writeString(writer, root);

    //加入信息头长度
	unsigned int size = sendbuf1.size();
	//const char* message_len = (const char*)&size;
	const char* message_len = reinterpret_cast<const char*>(&size);

	std::string sendbuf(message_len, sizeof(size));
	sendbuf += sendbuf1;


	sendSocket->add_sendbuf(sendbuf);
}
