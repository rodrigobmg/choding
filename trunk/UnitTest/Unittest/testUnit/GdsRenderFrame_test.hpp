#pragma once
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderFrame.h"
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderObject.h"

TEST( GdsRenderFrameTest ,  GdsRenderFrameTest )
{
	GdsRenderFramePtr renderframe = GdsRenderFramePtr( new GdsRenderFrame );
	GdsRenderObjectPtr renderobject = GdsRenderObjectPtr( new GdsRenderObject );

	renderframe->AddRenderObject( renderobject );
	renderframe->SetRenderState( D3DRS_LIGHTING, FALSE );
}

