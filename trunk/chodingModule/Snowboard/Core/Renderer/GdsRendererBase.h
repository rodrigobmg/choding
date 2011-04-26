#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"
#include "../Camera/GdsCamera.h"

class GdsRendererBase : public GdsObject{	

	GdsCameraPtr	m_Camera;

public:

	GdsRendererBase()
	{ 
		SetName( OBJECT_RENDERBASE );
		m_Camera = GdsCameraPtr( (GdsCamera*)NULL );
	}
	virtual ~GdsRendererBase(){};

	void			Update( float fElapsedTime ){ vUpdate(fElapsedTime); }
	void			SetCamera( GdsCameraPtr camera ){ m_Camera = camera; }
	GdsCameraPtr	GetCamera(){ return m_Camera; }

protected:

	virtual void	vUpdate( float fElapsedTime ) = 0;
};

#endif