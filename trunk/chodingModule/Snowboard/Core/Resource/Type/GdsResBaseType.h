#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"

class GdsBaseRes : public GdsObject{

public:
	GdsBaseRes()
	{
		SetName( OBJECT_RES_BASE );
	}

	virtual ~GdsBaseRes(){};

	virtual void	Clear() = 0;
	virtual HRESULT	Create() = 0;
	virtual HRESULT	Release() = 0;
	
};

#endif