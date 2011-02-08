#pragma once

#include "../../Base/CoreBase.h"

class CBaseRes : public CCoreBase{

private:

public:
	CBaseRes()
	{
		SetName( OBJECT_RES_BASE );
	}

	virtual ~CBaseRes();

	virtual void	Clear() = 0;
	virtual HRESULT	Create() = 0;
	virtual HRESULT	Release() = 0;
	virtual	HRESULT	Init() = 0;
	
};