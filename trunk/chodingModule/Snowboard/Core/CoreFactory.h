#ifndef _SNOWBOARD_COREFACTORY_
#define _SNOWBOARD_COREFACTORY_

#include "Base\Snow.h"


// �ھ� ���丮
class CCoreFactory {

public:
	CCoreFactory();
	virtual ~CCoreFactory();

	static CSnow*	CreateCore( const tstring& coretype );
};

#endif