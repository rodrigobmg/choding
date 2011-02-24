#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"

class GdsRendererBase : public GdsObject{

public:
	GdsRendererBase(){ SetName( OBJECT_RENDERBASE ); }
	virtual ~GdsRendererBase(){};
	
	virtual void Render( GdsNodePtr pNode ) = 0;	

};

#endif