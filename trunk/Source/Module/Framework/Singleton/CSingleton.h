#ifndef _H_SINGLETON_
#define _H_SINGLETON_

#include <cassert>

template < typename T > class CSingleton
{
	static T* ms_Singleton;

public:
	CSingleton(void)
	{
		assert( !ms_Singleton && L"�ϳ��� �Ͱ��ּ���~~" );
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	}

	~CSingleton()
	{
		assert( ms_Singleton && L"�̹� ���µ� �� �� ���ٷ��� �ϳ���~~"); 
		ms_Singleton = NULL; 
	}

	static void CreateInstance()
	{
		ms_Singleton = new T;
	}

	static void DestroyInstance()
	{
		delete ms_Singleton;
		ms_Singleton = NULL;
	}

	static T& GetSingleton(void)
	{
		assert( ms_Singleton );
		return *ms_Singleton;
	}
	static T* GetSingletonPtr(void)
	{
		return ms_Singleton;
	}
};

template<typename T> 
T* Singleton< T >::ms_Singleton = NULL;

#endif