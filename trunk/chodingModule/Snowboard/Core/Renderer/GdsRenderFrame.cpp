#include "GdsRenderFrame.h"


GdsRenderFrame::GdsRenderFrame()
: m_iTotalcountofRenderObject(0)
{
	SetName( OBJECT_RENDERFRAME );
}

GdsRenderFrame::~GdsRenderFrame()
{
	m_RenderFrame.clear();
	m_FrontRenderFrame.clear();
	m_RenderStateList.clear();
	m_RenderToken.clear();
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

void GdsRenderFrame::swap_buffer()
{
	m_FrontRenderFrame.swap( m_RenderFrame );
}

void GdsRenderFrame::vRender( LPDIRECT3DDEVICE9 device )
{ 	 
	swap_buffer();

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
	m_iTotalcountofRenderObject = m_FrontRenderFrame.size();

	it = m_FrontRenderFrame.begin();
	RENDER_CONTAINER::iterator it_end = m_FrontRenderFrame.end();
	for ( ; it != it_end ; ++it )
	{
		FreeRenderObject( it->second );
	}
	m_FrontRenderFrame.clear();
}

void GdsRenderFrame::AddRenderStateGroup( GdsRenderStateGroupPtr renderstategroup , int iRenderStateGroupID )
{
	m_RenderStateList.insert( std::pair<int , GdsRenderStateGroupPtr>(  iRenderStateGroupID , renderstategroup ) );
}

GdsRenderObject* GdsRenderFrame::AllocRenderObject()
{	
	GdsRenderObject* pRenderObject = new GdsRenderObject;
	if ( pRenderObject == NULL )
		return NULL;

	m_RenderToken.push_back( pRenderObject );

	GdsRenderObject* pTemp = m_RenderToken.at( m_RenderToken.size()-1 );
	ASSERT( pTemp == pRenderObject );
	return pRenderObject;
	//	LOG_CYAN_F( "Alloc RenderObject address[0x%08x] Total Count = %d" , p , m_listRenderToken.size() );
}

void GdsRenderFrame::FreeRenderObject( GdsRenderObject* p )
{
	RENDERTOKEN_LIST::iterator it = m_RenderToken.begin();
	RENDERTOKEN_LIST::iterator list_end = m_RenderToken.end();
	for ( ; it != list_end ; ++it )
	{
		if ( p == *it )
		{
			SAFE_DELETE( *it );
			m_RenderToken.erase( it );
			p = NULL;
			break;
		}
	}	
}
