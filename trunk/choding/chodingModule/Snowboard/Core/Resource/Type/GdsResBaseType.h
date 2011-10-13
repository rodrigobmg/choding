#ifndef _GDS_H_RESBASETYPE_
#define _GDS_H_RESBASETYPE_

#include "../../Base/GdsObject.h"
#include "../../Property/GdsPropertyState.h"

class GdsResBase : public GdsObject
{
protected:

	GdsPropertyStatePtr	m_PropertyState;
	tstring				m_strPath;

	virtual void		vClear() = 0;
	virtual HRESULT		vRelease() = 0;
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device ) = 0;

public:
	GdsResBase();
	virtual ~GdsResBase();

	void				Clear();
	HRESULT				Create( const TCHAR* path , LPDIRECT3DDEVICE9 device );
	HRESULT				ReCreate( LPDIRECT3DDEVICE9 device );
	HRESULT				LoadResource(LPDIRECT3DDEVICE9 device);

	GdsPropertyStatePtr	GetPropertyState(){ return m_PropertyState; }
	void				SetPropertyState( GdsPropertyStatePtr pPropertyState ){ m_PropertyState = pPropertyState; }
	
};


typedef boost::shared_ptr< GdsResBase >		GdsResBasePtr;

#endif