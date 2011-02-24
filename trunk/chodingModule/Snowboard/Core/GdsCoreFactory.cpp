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
		assert( 0 && "���ǵ��� ���� �ھ�� �����ɼ� �����ϴ�." );
		return NULL;
	}
}