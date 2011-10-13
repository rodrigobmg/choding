#include "stdafx.h"

GdsRendererBase::GdsRendererBase()
{ 
}

void GdsRendererBase::Render( float fAcuumTime )
{ 
	RenderFrame( fAcuumTime );
	RENDERER.SetRenderingEnable( true );
}
