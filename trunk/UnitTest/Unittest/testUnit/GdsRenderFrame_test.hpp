#pragma once
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderFrame.h"
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRenderObject.h"

TEST( GdsRenderFrameTest ,  GdsRenderFrameTest )
{
	GdsRenderFramePtr renderframe = GdsRenderFramePtr( new GdsRenderFrame );
	GdsRenderObjectPtr renderobject = GdsRenderObjectPtr( new GdsRenderObject );
	GdsRenderStateGroupPtr	stategroup = GdsRenderStateGroupPtr( new GdsRenderStateGroup );
	for ( int i=0 ; i < 10 ; ++i )
	{
		renderframe->AddRenderStateGroup( stategroup , i );
	}
	
	for ( int i = 0 ; i< 100 ; i++ )
	{
		renderframe->AttachRenderObject( renderobject , 5);
		renderframe->AttachRenderObject( renderobject , 2);
		renderframe->AttachRenderObject( renderobject , 3);
		renderframe->AttachRenderObject( renderobject , 3);
		renderframe->AttachRenderObject( renderobject , 3);
		renderframe->AttachRenderObject( renderobject , 3);
		renderframe->AttachRenderObject( renderobject , 1);
		renderframe->AttachRenderObject( renderobject , 10);
	}
	
	//renderframe->DetachRenderObject( renderobject );
}

