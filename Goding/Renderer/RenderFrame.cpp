#include "stdafx.h"
#include "RenderFrame.h"

RenderFrame::RenderFrame()
: m_iTotalcountofRenderObject(0)
{
}

RenderFrame::~RenderFrame()
{
	m_RenderFrame.clear();
	m_FrontRenderFrame.clear();
	m_RenderStateList.clear();
}

void RenderFrame::Swap_buffer()
{
	m_FrontRenderFrame.swap( m_RenderFrame );
}

void RenderFrame::vRender( LPDIRECT3DDEVICE9 device )
{ 	 
	//swap_buffer();

	int iPreRenderStateIndex = -1;
	RENDER_CONTAINER::iterator it = m_FrontRenderFrame.begin();
	for ( ; it != m_FrontRenderFrame.end() ; ++it )
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
	m_iTotalcountofRenderObject = (int)m_FrontRenderFrame.size();

	it = m_FrontRenderFrame.begin();
	RENDER_CONTAINER::iterator it_end = m_FrontRenderFrame.end();
	for ( ; it != it_end ; ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_FrontRenderFrame.clear();
}

void RenderFrame::AddRenderStateGroup( RenderStateGroupPtr renderstategroup , int iRenderStateGroupID )
{
	m_RenderStateList.insert( std::pair<int , RenderStateGroupPtr>(  iRenderStateGroupID , renderstategroup ) );
}

void RenderFrame::ClearBackFrameBuffer()
{
	RENDER_CONTAINER::iterator it = m_RenderFrame.begin();
	RENDER_CONTAINER::iterator it_end = m_RenderFrame.end();
	for ( ; it != it_end ; ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_RenderFrame.clear();
}

RenderObject* RenderFrame::AllocRenderObject( int iRenderStateGroupID )
{	
	RenderObject* pRenderObject = new RenderObject;
	if ( pRenderObject == NULL )
		return NULL;
	
	AttachRenderObject( pRenderObject , iRenderStateGroupID );
	return pRenderObject;
}


void RenderFrame::AttachRenderObject( RenderObject* pRenderObject , int iRenderStateGroupID )
{
	if ( iRenderStateGroupID < 0 )
	{
		iRenderStateGroupID = 0;
		ASSERT( 0 );
	}

	if ( (int)m_RenderStateList.size() < iRenderStateGroupID )
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