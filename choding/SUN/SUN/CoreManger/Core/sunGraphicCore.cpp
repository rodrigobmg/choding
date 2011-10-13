#include "Precompiled.h"
#include "sunGraphicCore.h"

sunGraphicCore::sunGraphicCore() :
m_pRenderer(NULL)
{

	m_RenderList_Vector.reserve(1024);
}


sunGraphicCore::~sunGraphicCore()
{

}

int sunGraphicCore::InitializeCore()
{
	//랜더러를 생성한다.

	assert( NULL == m_pRenderer );

	m_pRenderer = new sunRendererDX9;

	m_pRenderer->InitializeRenderer();

	return true;
}

void sunGraphicCore::DestoryCore()
{
	assert( NULL != m_pRenderer);


	m_pRenderer->DestroyRenderer();
	Safe_Delete(m_pRenderer);
	
	ClearRenderList();

}


void sunGraphicCore::UpdateCore()
{
	BeginRender();
	Render();
	EndRender();

}

sunGraphicCore* sunGraphicCore::GetCore()
{
	assert(g_pCoreManager);

	sunGraphicCore* pGraphicCore = static_cast<sunGraphicCore*>(g_pCoreManager->GetCore(CORENAME_GRAPHIC));
	
	assert(pGraphicCore);

	return pGraphicCore;
}

sunRenderer* sunGraphicCore::GetRenderer()
{
	assert(m_pRenderer);
	return m_pRenderer;
}


void sunGraphicCore::RegistRenderList( const NodeList& SceneList )
{
	BOOST_FOREACH( sunNodePtr spNode, SceneList)
	{
		assert(spNode);

		if( IsKindOf(sunRenderNode , spNode) )
		{
			sunRenderNodePtr spRenderNode = SmartPointerCast( sunRenderNode, spNode);
			if(spRenderNode->GetVisible())
				m_RenderList_Vector.push_back( spRenderNode );
		}
		

		RegistRenderList( spNode->GetChildList() );
	}

}


void sunGraphicCore::BeginRender()
{
	assert(m_pRenderer);
	m_pRenderer->BeginRender();
}

void sunGraphicCore::Render()
{
	assert(m_pRenderer);
	m_pRenderer->RenderObject( m_RenderList_Vector );
}

void sunGraphicCore::EndRender()
{
	assert(m_pRenderer);
	m_pRenderer->EndRender();
}

void sunGraphicCore::ClearRenderList()
{
	m_RenderList_Vector.clear();
}
