#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"

class GdsRendererBase : public GdsObject{

public:
	GdsRendererBase(){ SetName( OBJECT_RENDERBASE ); }
	virtual ~GdsRendererBase(){};

	void			Update( float fElapsedTime ){ vUpdate(fElapsedTime); }

protected:

	virtual void	vUpdate( float fElapsedTime ) = 0;
};

#endif