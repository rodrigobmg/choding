#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"
#include "../Camera/GdsCamera.h"

class GdsRendererBase : public GdsObject{	


public:

	GdsRendererBase()
	{ 
		SetName( OBJECT_RENDERBASE );
	}
	virtual ~GdsRendererBase(){};

	void RenderFrame(){ vRenderFrame(); }

protected:

	virtual void vRenderFrame() = 0;
};

#endif