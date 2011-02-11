#pragma once

#include "Base/CoreBase.h"

// 코어 팩토리
class CCoreFactory {

public:
	CCoreFactory();
	virtual ~CCoreFactory();

	static CCoreBase*		CreateCore( const tstring& coretype );
};