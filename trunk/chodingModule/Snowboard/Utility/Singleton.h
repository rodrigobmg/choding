#pragma once

template < typename T > 
class CSingleton
{
	static T* ms_Singleton;

public:
	CSingleton()
	{
		assert( !ms_Singleton && L"하나만 맹거주세요~~" );
 //		int32_t offset = (int32_t)(T*)1 - (int32_t)(CSingleton <T>*)(T*)1;
// 		ms_Singleton = (T*)((int32_t)this + offset);
		ms_Singleton = 0; 
	}

	virtual ~CSingleton()
	{
		assert( ms_Singleton && L"이미 없는데 왜 또 없앨려고 하나용~~"); 
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

	static T& getInstance()
	{
		assert( ms_Singleton );
		return *ms_Singleton;
	}
	static T* getInstancePtr()
	{
		return ms_Singleton;
	}
};

template<typename T> 
T* CSingleton< T >::ms_Singleton = 0;

#endif