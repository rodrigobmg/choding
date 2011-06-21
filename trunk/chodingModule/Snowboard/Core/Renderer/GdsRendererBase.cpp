#include "GdsRendererBase.h"
#include "GdsRendererManager.h"

GdsRendererBase::GdsRendererBase()
{ 
	SetName( OBJECT_RENDERBASE );
}

void GdsRendererBase::Render( float fAcuumTime )
{ 
	RenderFrame( fAcuumTime );
	RENDERER.SetRenderingEnable( true );
}
