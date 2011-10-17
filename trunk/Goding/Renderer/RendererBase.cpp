#include "stdafx.h"
#include "RendererBase.h"

RendererBase::RendererBase()
{ 
}

void RendererBase::Render( float fAcuumTime )
{ 
	RenderFrame( fAcuumTime );
	RENDERER.SetRenderingEnable( true );
}
