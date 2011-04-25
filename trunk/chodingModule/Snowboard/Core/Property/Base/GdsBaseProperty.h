#ifndef _GDS_CORE_PROPERTY_BASE_H_
#define _GDS_CORE_PROPERTY_BASE_H_

#include "../../Base/GdsObject.h"

class GdsBaseProperty : public GdsObject
{
protected:

	D3DXMATRIX		m_DXmatWorld;
	

	virtual	void		vClear(){};
	virtual	void		vUpdate( float fElapsedTime ){ ASSERT( fElapsedTime); }
	virtual void		vRender( LPDIRECT3DDEVICE9 device ){ ASSERT( device); }

public:
	GdsBaseProperty(void);
	virtual ~GdsBaseProperty(void);

	void				Clear(){ vClear(); }
	void				Update( float fElapsedTime ){ vUpdate( fElapsedTime ); }
	void				Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }

	void				SetMT( D3DXMATRIX& mt ){ m_DXmatWorld = mt; }
	D3DXMATRIX		GetMT(){ return m_DXmatWorld; }

	
};

#endif