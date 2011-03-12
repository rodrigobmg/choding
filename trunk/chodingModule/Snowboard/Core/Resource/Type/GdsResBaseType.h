#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"

class GdsResBase : public GdsObject
{
protected:

	tstring				m_strPath;

public:
	GdsResBase()
	{
		SetName( OBJECT_RES_BASE );
	}

	virtual ~GdsResBase(){};

	virtual void	vClear() = 0;
	virtual HRESULT	vCreate() = 0;
	virtual HRESULT	vRelease() = 0;
	virtual	HRESULT	vReCreate( LPDIRECT3DDEVICE9 device ) = 0;
	virtual HRESULT vLoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device ) = 0;
};

typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif