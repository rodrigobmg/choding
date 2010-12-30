#ifndef _CHODING_SINGLETON_
#define _CHODING_SINGLETON_
#include "choding_stdafx.h"
#include "../BaseObject/CObject.h"

template < typename T > 
class CSingleton : public CObject
{
	static T* ms_Singleton;

public:
	CSingleton()
	{
		assert( !ms_Singleton && L"�ϳ��� �Ͱ��ּ���~~" );
 //		int32_t offset = (int32_t)(T*)1 - (int32_t)(CSingleton <T>*)(T*)1;
// 		ms_Singleton = (T*)((int32_t)this + offset);
		ms_Singleton = 0; 
	}

	virtual ~CSingleton()
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