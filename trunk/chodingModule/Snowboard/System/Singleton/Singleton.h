
#ifndef _SNOWBOARD_UTILITY_SINGLETON_
#define _SNOWBOARD_UTILITY_SINGLETON_

#include <ASSERT.h>

template < typename T > 
class singleton 
{
	static T* ms_singleton;

public:
	singleton()
	{
		ASSERT( !ms_singleton && L"�ϳ��� �Ͱ��ּ���~~" );
		ms_singleton = 0; 
	}

	virtual ~singleton()
	{
		ASSERT( ms_singleton && L"�̹� ���µ� �� �� ���ٷ��� �ϳ���~~"); 
		ms_singleton = 0; 
	}

	static void createSingleton()
	{
		ms_singleton = new T;
	}

	static void destroySingleton()
	{
		delete ms_singleton;
		ms_singleton = NULL;
	}

	static T& getInstance()
	{
		ASSERT( ms_singleton );
		return *ms_singleton;
	}
	static T* getInstancePtr()
	{
		return ms_singleton;
	}
};

template<typename T> 
T* singleton< T >::ms_singleton = 0;

#endif // _SNOWBOARD_UTILITY_SINGLETON_