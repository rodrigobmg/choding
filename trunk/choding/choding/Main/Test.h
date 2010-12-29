
#include <stdio.h>
#include "../../../chodingModule/Framework/Singleton/CSingleton.h"


class Test : public CSingleton< Test >
{
public:

	Test(){};
	virtual ~Test(){};

	void Run()
	{
		printf( "Test::hello world\n" );
	}
};
