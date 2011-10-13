#include "stdafx.h"


GdsRenderFrame::GdsRenderFrame()
: m_iTotalcountofRenderObject(0)
{
}

GdsRenderFrame::~GdsRenderFrame()
{
	m_RenderFrame.clear();
	m_FrontRenderFrame.clear();
	m_RenderStateList.clear();
}

void GdsRenderFrame::Swap_buffer()
{
	m_FrontRenderFrame.swap( m_RenderFrame );
}

void GdsRenderFrame::vRender( LPDIRECT3DDEVICE9 device )
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

void GdsRenderFrame::AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID )
{
	m_RenderStateList.insert( std::pair<int , GdsRenderStateGroupPtr>(  iRenderStateGroupID , renderstategroup ) );
}

void GdsRenderFrame::ClearBackFrameBuffer()
{
	RENDER_CONTAINER::iterator it = m_RenderFrame.begin();
	RENDER_CONTAINER::iterator it_end = m_RenderFrame.end();
	for ( ; it != it_end ; ++it )
	{
		SAFE_DELETE( it->second );
	}
	m_RenderFrame.clear();
}

GdsRenderObject* GdsRenderFrame::AllocRenderObject( int iRenderStateGroupID )
{	
	GdsRenderObject* pRenderObject = new GdsRenderObject;
	if ( pRenderObject == NULL )
		return NULL;
	
	AttachRenderObject( pRenderObject , iRenderStateGroupID );
	return pRenderObject;
}


void GdsRenderFrame::AttachRenderObject( GdsRenderObject* pRenderObject , int iRenderStateGroupID )
{
	if ( iRenderStateGroupID < 0 )
	{
		iRenderStateGroupID = 0;
		ASSERT( 0 );
	}

	if ( (int)m_RenderStateList.size() < iRenderStateGroupID )
	{
		ASSERT( 0 && "����������Ʈ�ε������� �������� �����ϼ���");
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
				ASSERT( 0 && "��������Ʈ ����.�Ѥ�;;" );
				return;
			}		
		}
	}
}