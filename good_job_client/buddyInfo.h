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

	CString name;
};