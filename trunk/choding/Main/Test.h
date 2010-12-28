#pragma once
#include "../../Module/Framework/Singleton/CSingleton.h"

class CTest : public CSingleton< CTest >
{
public:
	CTest(void);
	~CTest(void);
};

