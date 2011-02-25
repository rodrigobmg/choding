#include "GdsCoreFactory.h"

#include "Renderer/GdsRendererDX9.h"
#include "Resource/GdsResMgr.h"

GdsCoreFactory::GdsCoreFactory()
{
}

GdsCoreFactory::~GdsCoreFactory()
{
}

GdsObjectPtr	GdsCoreFactory::CreateCore( const tstring& coretype )
{
	if ( coretype == CORE_RESOURCE )
	{
		return GdsResMgrPtr( new GdsResMgr );	
	}
	else if ( coretype == CORE_RENDERER )
	{
		return GdsRendererDX9Ptr( new GdsRendererDX9 );
	}
	else if ( coretype == CORE_CAMERA )
	{
		return GdsObjectPtr( (GdsObject*)NULL );
	}
	else
	{
		assert( 0 && "정의되지 않은 코어는 생성될수 없습니다." );
		return GdsObjectPtr( (GdsObject*)NULL );
	}
}