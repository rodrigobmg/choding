#include "GdsCoreFactory.h"

#include "Renderer/GdsRendererDX9.h"
#include "Resource/GdsResMgr.h"

GdsCoreFactory::GdsCoreFactory()
{
}

GdsCoreFactory::~GdsCoreFactory()
{
}

GdsObject*	GdsCoreFactory::CreateCore( const tstring& coretype )
{
	if ( coretype == CORE_RESOURCE )
	{
		return new GdsResMgr;	
	}
	else if ( coretype == CORE_RENDERER )
	{
		return new GdsRendererDX9;
	}
	else if ( coretype == CORE_CAMERA )
	{
		//return new CCama
		return NULL;
	}
	else
	{
		assert( 0 && "정의되지 않은 코어는 생성될수 없습니다." );
		return NULL;
	}
}