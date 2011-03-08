#include "GdsCoreFactory.h"

#include "Renderer/GdsRendererDX9.h"
#include "Resource/GdsResMgr.h"
#include "Camera/GdsCameraManagerDX9.h"

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
	else if ( coretype == CORE_CAMMGR_DX9 )
	{
		return GdsCameraManagerDX9Ptr( new GdsCameraManagerDX9 );
	}
	else
	{
		assert( 0 && L"���ǵ��� ���� �ھ�� �����ɼ� �����ϴ�." );
		return GdsObjectPtr( (GdsObject*)NULL );
	}
}