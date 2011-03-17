
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
		ASSERT( !ms_singleton && L"하나만 맹거주세요~~" );
		ms_singleton = 0; 
	}

	virtual ~singleton()
	{
		ASSERT( ms_singleton && L"이미 없는데 왜 또 없앨려고 하나용~~"); 
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