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
		assert( 0 && "���ǵ��� ���� �ھ�� �����ɼ� �����ϴ�." );
		return NULL;
	}
}