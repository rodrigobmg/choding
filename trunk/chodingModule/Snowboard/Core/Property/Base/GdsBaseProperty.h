#ifndef _GDS_CORE_PROPERTY_BASE_H_
#define _GDS_CORE_PROPERTY_BASE_H_

#include "../../Base/GdsObject.h"

class GdsBaseProperty : public GdsObject
{
public:
	GdsBaseProperty(void);
	virtual ~GdsBaseProperty(void);

	void				Clear(){ vClear(); }
	void				Update( float fElapsedTime ){ vUpdate( fElapsedTime ); }
	void				Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }


protected:
	
	virtual	void		vClear(){};
	virtual	void		vUpdate( float fElapsedTime ){ ASSERT( fElapsedTime); }
	virtual void		vRender( LPDIRECT3DDEVICE9 device ){ ASSERT( device); }

};

#endif