#pragma once

#include <atlstr.h>  // CString 的头文件

class buddyInfo
{
public:
	buddyInfo() {}
	buddyInfo(CString _name):name(_name) {}
	CString getName()
	{
		return name;
	}
private:
	CString name;
};
class buddyInfoWithAccount: public buddyInfo
{
public:
	buddyInfoWithAccount() {}
	buddyInfoWithAccount(CString _account, CString _name) :account(_account), buddyInfo(_name){}
private:
	CString account;
};
class groupInfo
{
public:
	groupInfo() {}
	groupInfo(CString _name) :name(_name) {}
	CString getName()
	{
		return name;
	}
	void members_push(buddyInfoWithAccount& member)
	{
		members.push_back(member);
	}
private:
	CString name;
	std::vector<buddyInfoWithAccount> members;
};
