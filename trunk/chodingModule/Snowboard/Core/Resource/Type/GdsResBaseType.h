#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"

class GdsResBase : public GdsObject{

public:
	GdsResBase()
	{
		SetName( OBJECT_RES_BASE );
	}

	virtual ~GdsResBase(){};

	virtual void	Clear() = 0;
	virtual HRESULT	Create() = 0;
	virtual HRESULT	Release() = 0;
	
};

typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif