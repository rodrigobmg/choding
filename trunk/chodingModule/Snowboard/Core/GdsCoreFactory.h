#ifndef _GDS_H_COREFACTORY_
#define _GDS_H_COREFACTORY_

#include "Base/GdsObject.h"

// 코어 팩토리
class GdsCoreFactory {

public:
	GdsCoreFactory();
	virtual ~GdsCoreFactory();

	static GdsObjectPtr	CreateCore( const tstring& coretype );
};

#endif