#ifndef _SNOWBOARD_COREFACTORY_
#define _SNOWBOARD_COREFACTORY_

#include "Base\Snow.h"


// 코어 팩토리
class CCoreFactory {

public:
	CCoreFactory();
	virtual ~CCoreFactory();

	static CSnow*	CreateCore( const tstring& coretype );
};

#endif