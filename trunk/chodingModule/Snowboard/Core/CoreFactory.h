#pragma once

#include "Base/CoreBase.h"

// �ھ� ���丮
class CCoreFactory {

public:
	CCoreFactory();
	virtual ~CCoreFactory();

	static CCoreBase*		CreateCore( const tstring& coretype );
};