#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"

class GdsRendererBase : public GdsObject
{	

public:	
	virtual ~GdsRendererBase(){};

	void				Render( float fAcuumTime );
	virtual	void		Clear() = 0;
	virtual bool		Create( HWND hWnd ) = 0;
	virtual HRESULT		Release() = 0;

protected:	
	GdsRendererBase();
	virtual void		RenderFrame( float fAccumtime ) = 0;
};

#endif