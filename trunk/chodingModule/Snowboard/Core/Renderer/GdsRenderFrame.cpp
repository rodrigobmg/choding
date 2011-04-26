#include "GdsRenderFrame.h"


GdsRenderFrame::~GdsRenderFrame()
{
	m_RenderFrame.clear();
	m_RenderStateList.clear();
}

void GdsRenderFrame::AttachRenderObject( GdsRenderObjectPtr pRenderObject , int iRenderStateGroupID )
{
	if ( iRenderStateGroupID < 0 )
	{
		iRenderStateGroupID = 0;
		ASSERT( 0 );
	}

	if ( m_RenderStateList.size() < iRenderStateGroupID )
	{
		ASSERT( 0 && "렌더스테이트인덱스보다 작은것을 참조하세요");
		iRenderStateGroupID = 0;
	}

	RENDEROBEJCT renderobj( iRenderStateGroupID , pRenderObject );
	m_RenderFrame.push_back( renderobj );
// 	if ( m_RenderFrame.empty() )
// 	{		
// 		m_RenderFrame.push_back( renderobj );
// 	}
// 	else
// 	{
// 		if ( iRenderStateGroupID < m_RenderFrame.front().first )
// 		{
// 			m_RenderFrame.push_front( renderobj );
// 		}
// 		else if ( iRenderStateGroupID > m_RenderFrame.end().first )
// 		{
// 			m_RenderFrame.push_back( renderobj );
// 		}
// 		else
// 		{
// 			std::lower_bound( renderobj)
// 		}
// 	}
}

void GdsRenderFrame::DetachRenderObject( GdsRenderObjectPtr pRenderObject )
{
	RENDER_CONTAINER::iterator it = m_RenderFrame.begin();
	for ( ; it != m_RenderFrame.end() ; ++it )
	{
		m_RenderFrame.erase( it );
	}
}

void GdsRenderFrame::vRender( LPDIRECT3DDEVICE9 device )
{ 	 
	RENDER_CONTAINER::iterator it = m_RenderFrame.begin();
	for ( ; it != m_RenderFrame.end() ; ++it )
	{
		RENDERSTATEGROUP::iterator it_state = m_RenderStateList.find( it->first );
		if ( it_state != m_RenderStateList.end() )
		{
			it_state->second->SetRenderState( device );
			it->second->vRender( device );
		}
	}
}

void GdsRenderFrame::AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID )
{
	m_RenderStateList.insert( std::pair<int , GdsRenderStateGroupPtr>(  iRenderStateGroupID , renderstategroup ) );
}