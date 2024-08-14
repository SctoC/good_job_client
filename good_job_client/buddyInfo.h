#pragma once

#include <atlstr.h>  // CString ��ͷ�ļ�

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
	CString getaccount()
	{
		return account;
	}
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
	std::vector<buddyInfoWithAccount>* get_members()
	{
		return &members;
	}
private:
	CString name;
	std::vector<buddyInfoWithAccount> members;
};
