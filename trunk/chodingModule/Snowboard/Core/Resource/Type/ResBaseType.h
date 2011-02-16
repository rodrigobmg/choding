#pragma once

#include "../../Base/Snow.h"

class CBaseRes : public CSnow{

private:

public:
	CBaseRes()
	{
		SetName( OBJECT_RES_BASE );
	}

	virtual ~CBaseRes(){};

	virtual void	Clear() = 0;
	virtual HRESULT	Create() = 0;
	virtual HRESULT	Release(){ DecRefCount(); return S_OK; }
	
};