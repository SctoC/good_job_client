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

	CString name;
};