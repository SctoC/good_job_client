#pragma once
#include<string>

#define LogInQuest 1
class Request
{
public:
	Request(unsigned int _type):type(_type){}
	unsigned int type;
};
class LogIn_Request :Request
{
public:
	LogIn_Request(unsigned int _type, std::string _account, std::string _pwd) :Request(_type), account(_account), pwd{ _pwd }{}
	std::string account;
	std::string pwd;
};

