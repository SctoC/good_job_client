#pragma once


class model
{
private:
	model() {}
public:
	model(const model& ) = delete;
	model& operator=(const model& )=delete;
	
	static model* getInstance()
	{
		//�̰߳�ȫ�ĵ���ģʽ
		static model* m= new model();
		return m;
	}
	
};

