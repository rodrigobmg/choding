#ifndef _CHODING_SINGLETON_
#define _CHODING_SINGLETON_


#include <stdio.h>
#include "../../../chodingModule/Framework/Singleton/CSingleton.h"


class Test : public CSingleton< Test >
{
public:

	Test();
	virtual ~Test();


	void Run()
	{
		printf( "hello world\n" );
	}
};
#endif