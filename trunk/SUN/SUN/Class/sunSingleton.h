#ifndef _SUN_CLASS_SINGLETON_H_
#define _SUN_CLASS_SINGLETON_H_

template<class T>
class sunSingleton
{
public:
	sunSingleton()
	{
		assert(!ms_pSingleton);
		ms_pSingleton = static_cast<T*>( this );
	}

	~sunSingleton()
	{
		assert( ms_pSingleton);
		ms_pSingleton = NULL;
	}

public:
	static T& getSingleton()
	{
		assert(ms_pSingleton);
		return (*ms_pSingleton);
	}

	static T* getSingletonPtr()
	{
		return ms_pSingleton;
	}


private:
	static T* ms_pSingleton;		//½Ì±ÛÅæ Æ÷ÀÎÆ®

};


#endif