#pragma once
#include<string>

#define LogInQuest 1
#define LogInAck 1

#define sendMessageQuest 2
#define buddyMessage 2

class Request
{
public:
	Request(unsigned int _type):type(_type){}
	unsigned int type;
};
class LogIn_Request :public Request
{
public:
	LogIn_Request(unsigned int _type, std::string _account, std::string _pwd) :Request(_type), account(_account), pwd{ _pwd }{}
	std::string account;
	std::string pwd;
};

class sendMessage_Request :public Request
{
public:
	sendMessage_Request(unsigned int _type, std::string _send_account, std::string _receive_account, std::string _content, std::string _time) :Request(_type), send_account(_send_account), receive_account{ _receive_account }, content(_content), time(_time){}
	
	std::string send_account;
	std::string receive_account;
	std::string content;
	std::string time;
    
};
