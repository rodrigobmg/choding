#include "CoreFactory.h"

#include "Renderer/RendererDX9.h"
#include "Resource/ResMgr.h"

CCoreFactory::CCoreFactory()
{
}

CCoreFactory::~CCoreFactory()
{
}

CSnow*	CCoreFactory::CreateCore( const tstring& coretype )
{
	if ( coretype == CORE_RESOURCE )
	{
		return new CResMrg;	
	}
	else if ( coretype == CORE_RENDERER )
	{
		return new CRendererDX9;
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