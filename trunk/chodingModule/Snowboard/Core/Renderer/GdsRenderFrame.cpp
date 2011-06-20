#include "GdsRenderFrame.h"
#include "Resource\GdsResMgr.h"


GdsRenderFrame::GdsRenderFrame()
: m_iTotalcountofRenderObject(0)
{
	SetName( OBJECT_RENDERFRAME );
}

GdsRenderFrame::~GdsRenderFrame()
{
	m_RenderFrame.clear();
	m_RenderStateList.clear();
}

void GdsRenderFrame::AttachRenderObject( GdsRenderObject* pRenderObject , int iRenderStateGroupID )
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
 	if ( m_RenderFrame.empty() )
 	{		
 		m_RenderFrame.push_back( renderobj );
 	}
 	else
 	{
 		if ( iRenderStateGroupID <= m_RenderFrame.front().first )
 		{
 			m_RenderFrame.push_front( renderobj );
 		}
 		else if ( iRenderStateGroupID >= m_RenderFrame.back().first )
 		{
 			m_RenderFrame.push_back( renderobj );
 		}
 		else
 		{
 			RENDER_CONTAINER::iterator i = lower_bound( m_RenderFrame.begin() , m_RenderFrame.end() , iRenderStateGroupID , DataCompare() );
			if ( i != m_RenderFrame.end() )
			{
				m_RenderFrame.insert( i , renderobj );
			}
			else
			{
				ASSERT( 0 && "렌더리스트 꼬임.ㅡㅡ;;" );
				return;
			}		
 		}
 	}
}

void GdsRenderFrame::vRender( LPDIRECT3DDEVICE9 device )
{ 	 
	int iPreRenderStateIndex = -1;
	RENDER_CONTAINER::iterator it = m_RenderFrame.begin();
	for ( ; it != m_RenderFrame.end() ; ++it )
	{
		int iRenderStateIndex = it->first;
		
		RENDERSTATEGROUP::iterator it_state = m_RenderStateList.find( iRenderStateIndex );
		if ( it_state != m_RenderStateList.end() )
		{
			if ( iPreRenderStateIndex != iRenderStateIndex )
			{
				it_state->second->SetRenderState( device );
				iPreRenderStateIndex = iRenderStateIndex;
			}

			it->second->vRender( device );
		}
	}
	m_iTotalcountofRenderObject = m_RenderFrame.size();

	it = m_RenderFrame.begin();
	RENDER_CONTAINER::iterator it_end = m_RenderFrame.end();
	for ( ; it != it_end ; ++it )
	{
		RESMGR.FreeRenderObject( it->second );
	}
	m_RenderFrame.clear();
}

void GdsRenderFrame::AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID )
{
	m_RenderStateList.insert( std::pair<int , GdsRenderStateGroupPtr>(  iRenderStateGroupID , renderstategroup ) );
}

GdsRenderObject* GdsRenderFrame::AllocRenderObject()
{
	return RESMGR.AllocRenderObject();
}