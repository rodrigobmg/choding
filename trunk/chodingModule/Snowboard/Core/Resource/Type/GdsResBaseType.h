#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"
#include "../../Property/GdsPropertyState.h"

class GdsResBase : public GdsObject
{
protected:

	tstring				m_strPath;
	GdsPropertyStatePtr	m_PropertyState;

	virtual void	vClear() = 0;
	virtual HRESULT	vCreate() = 0;
	virtual HRESULT	vRelease() = 0;
	virtual	HRESULT	vReCreate( LPDIRECT3DDEVICE9 device ) = 0;
	virtual HRESULT vLoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device ) = 0;

public:
	GdsResBase()
	{
		SetName( OBJECT_RES_BASE );
		m_PropertyState = GdsPropertyStatePtr( new GdsPropertyState );
	}

	virtual ~GdsResBase(){};

	GdsPropertyStatePtr	GetPropertyState(){ return m_PropertyState; }
	void			Clear(){ vClear(); }
	HRESULT			Create(){ return vCreate(); }
	HRESULT			Release(){ return vRelease(); }
	HRESULT			ReCreate( LPDIRECT3DDEVICE9 device ) { return vReCreate( device ); }
	HRESULT			LoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device ){ return vLoadResource(path,device); }
	
};

typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif