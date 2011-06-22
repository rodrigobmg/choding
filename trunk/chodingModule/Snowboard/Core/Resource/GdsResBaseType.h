#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../Base/GdsObject.h"

class GdsResBase : public GdsObject
{
public:	
	GdsResBase();
	virtual ~GdsResBase();

protected:

	tstring				m_strPath;

	virtual void		vClear() = 0;
	virtual HRESULT		vRelease() = 0;
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device ) = 0;

public:

	void				Clear();
	HRESULT				Create( const TCHAR* path , LPDIRECT3DDEVICE9 device );
	HRESULT				ReCreate( LPDIRECT3DDEVICE9 device );
	HRESULT				LoadResource(LPDIRECT3DDEVICE9 device);
	
};


typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif