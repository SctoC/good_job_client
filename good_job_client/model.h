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
		//�̰߳�ȫ�ĵ���ģʽ
		static model m;
		return &m;
	}
	
};

