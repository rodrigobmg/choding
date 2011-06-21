#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"

class GdsRendererBase : public GdsObject
{	

public:
	GdsRendererBase();
	virtual ~GdsRendererBase(){};

	void			Render( float fAcuumTime );
protected:

	virtual void	vRenderFrame( float fAccumtime ) = 0;
};

#endif