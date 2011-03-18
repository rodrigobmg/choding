#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"
#include "../../Property/GdsPropertyState.h"

class GdsResBase : public GdsObject
{
protected:

	tstring				m_strPath;
	GdsPropertyStatePtr	m_PropertyState;

	virtual void		vClear() = 0;
	virtual HRESULT		vRelease() = 0;
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device ) = 0;

public:
	GdsResBase()
	{
		SetName( OBJECT_RES_BASE );
		m_PropertyState = GdsPropertyStatePtr( new GdsPropertyState );
	}

	virtual ~GdsResBase(){};

	GdsPropertyStatePtr	GetPropertyState(){ return m_PropertyState; }

	void				Clear(){ vClear(); }
	HRESULT				Create( const TCHAR* path , LPDIRECT3DDEVICE9 device )
						{
							m_strPath = path;
							return vLoadResource( device );
						}

	HRESULT				ReCreate( LPDIRECT3DDEVICE9 device ) 
						{ 
							vRelease();
							return vLoadResource( device ); 
						}

	HRESULT				LoadResource(LPDIRECT3DDEVICE9 device){ return vLoadResource(device); }
	
};


typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif