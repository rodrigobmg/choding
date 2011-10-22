#include "StdAfx.h"
#include "RenderSystem.h"
#include "IRenderObject\Mesh\Staticmesh.h"
#include "IRenderObject\Mesh\SkinnedMesh.h"
#include "IRenderObject\Mesh\MorphingMesh.h"


RenderSystem::RenderSystem(void)
{
}


RenderSystem::~RenderSystem(void)
{
}

IRenderObject* RenderSystem::AllocRenderObject( IRenderObject::ENABLE_RENDER_TYPE type )
{
	IRenderObject* pRenderObject = NULL;
	switch( type )
	{
	case IRenderObject::ENABLE_RENDER_TYPE::STATIC_MESH:
		pRenderObject = new StaticMesh;
		break;

	case IRenderObject::ENABLE_RENDER_TYPE::SKINNED_MESH:
		pRenderObject = new SkinnedMesh;
		break;

	case IRenderObject::ENABLE_RENDER_TYPE::MORPHING_MESH:
		pRenderObject = new MorphingMesh;
	}

	if( pRenderObject != NULL )
	{
		m_RenderObjectContainer.push_back( pRenderObject );
	}

	return pRenderObject;
}

bool RenderSystem::ReleaseRenderObject()
{
	return true;
}

void RenderSystem::Update( float fAccumtime )
{

}

void RenderSystem::Render()
{
	typedef	std::vector< IRenderObject* >::iterator renderObjectIter;
	renderObjectIter itB = m_RenderObjectContainer.begin();
	renderObjectIter itE = m_RenderObjectContainer.end();
	for (renderObjectIter it = itB ; it != itE ; ++it )
	{
		(*it)->Draw(NULL,NULL);
	}
}