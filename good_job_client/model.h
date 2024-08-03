#pragma once


class model
{
private:
	model() {}
public:
	model(const model& ) = delete;
	model& operator=(const model& )=delete;
	
1	static model* getInstance()
	{
		//线程安全的单例模式
		static model m;
		return &m;
	}
	
};

