#ifndef _GDS_H_RENDERERBASE_
#define _GDS_H_RENDERERBASE_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"
#include "../Camera/GdsCamera.h"

class GdsRendererBase : public GdsObject{	


public:

	GdsRendererBase(): m_IsCurRendering( true )
	{ 
		SetName( OBJECT_RENDERBASE );
	}
	virtual ~GdsRendererBase(){};

	bool EnableRendering(){ return m_IsCurRendering; }

	void RenderFrame( float fAcuumTime )
	{ 
		if ( m_IsCurRendering == true )
		{
			m_IsCurRendering = false;
			vRenderFrame( fAcuumTime ); 
			m_IsCurRendering = true;
		}
	}

protected:

	bool		m_IsCurRendering;
	virtual void vRenderFrame( float fAccumtime ) = 0;
};

#endif