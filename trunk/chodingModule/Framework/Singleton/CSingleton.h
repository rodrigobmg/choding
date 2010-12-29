#ifndef _H_SINGLETON_
#define _H_SINGLETON_

#include <cassert>

template < typename T > class CSingleton
{
	static T* ms_Singleton;

public:
	CSingleton()
	{
		assert( !ms_Singleton && L"�ϳ��� �Ͱ��ּ���~~" );
		int offset = (int)(T*)1 - (int)(CSingleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	}

	~CSingleton()
	{
		assert( ms_Singleton && L"�̹� ���µ� �� �� ���ٷ��� �ϳ���~~"); 
		ms_Singleton = 0; 
	}

	static void CreateSingleton()
	{
		ms_Singleton = new T;
	}

	static void DestroySingleton()
	{
		delete ms_Singleton;
		ms_Singleton = NULL;
	}

	static T& GetInstance()
	{
		assert( ms_Singleton );
		return *ms_Singleton;
	}
	static T* GetInstancePtr()
	{
		return ms_Singleton;
	}
};

template<typename T> 
T* CSingleton< T >::ms_Singleton = 0;

#endif