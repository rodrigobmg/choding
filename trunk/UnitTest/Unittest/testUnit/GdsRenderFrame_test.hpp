#pragma once
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderFrame.h"
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderObject.h"

TEST( GdsRenderFrameTest ,  GdsRenderFrameTest )
{
	GdsRenderFramePtr renderframe = GdsRenderFramePtr( new GdsRenderFrame );
	GdsRenderObjectPtr renderobject = GdsRenderObjectPtr( new GdsRenderObject );

	renderframe->AttachRenderObject( renderobject , 0);
//	renderframe->SetRenderState( D3DRS_LIGHTING, FALSE );
}

