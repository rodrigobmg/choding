#include "stdafx.h"

GdsVertexBuffer::GdsVertexBuffer()
:m_Vertex_Maxcount(0)
, m_FVF( GDSVERTEX::FVF )
, m_iStartVertexIndex(0)
, m_VertexSize(0)
, m_iEndVertexIndex(0) 
{

}

GdsVertexBuffer::~GdsVertexBuffer()
{
	vRelease();
}

void GdsVertexBuffer::vClear()
{
	m_pBuffer.clear();
	m_Vertex_Maxcount = 0;
}

HRESULT GdsVertexBuffer::vRelease()
{
	Free();
	vClear();
	return S_OK;
}

HRESULT GdsVertexBuffer::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	return S_OK;
}

void GdsVertexBuffer::Alloc()
{	
	if ( m_pVB != NULL && m_Vertex_Maxcount > 0 )
	{
		RESMGR.AllocVertexBuffer( m_pVB , m_Vertex_Maxcount* sizeof(GDSVERTEX) );
		if ( m_pVB == NULL )
			return;

		VOID* pI;
		if( FAILED( m_pVB->Lock( 0 , m_Vertex_Maxcount*sizeof(GDSVERTEX), (void**)&pI, 0 ) ) )
			return;

		GDSVERTEX* p = (GDSVERTEX*)pI;
		for ( size_t i=0 ; i < m_Vertex_Maxcount ; i++ )
		{			
			*p++ = m_pBuffer.at(i);
		}
		m_pVB->Unlock();

		m_VertexSize = sizeof(GDSVERTEX);		
		m_iEndVertexIndex = m_Vertex_Maxcount;
	}
}

void GdsVertexBuffer::Update()
{
	VOID* pI;
	if( FAILED( m_pVB->Lock( 0 , m_Vertex_Maxcount*sizeof(GDSVERTEX), (void**)&pI, 0 ) ) )
		return;

	GDSVERTEX* p = (GDSVERTEX*)pI;
	for ( size_t i=0 ; i < m_Vertex_Maxcount ; i++ )
	{			
		*p++ = m_pBuffer.at(i);
	}
	m_pVB->Unlock();
}

void GdsVertexBuffer::Free()
{
	if ( m_pVB )
	{
		RESMGR.FreeVertexBuffer( m_pVB );
		m_pBuffer.clear();
	}
}

void GdsVertexBuffer::AddVertex( GDSVERTEX& vertex )
{
	m_Vertex_Maxcount++;
	m_pBuffer.push_back( vertex );
}

void GdsVertexBuffer::GetVertexFromIndex(int index , GDSVERTEX& vertex )
{
	if( index < m_pBuffer.size() && index >= 0 )
	{
		vertex = m_pBuffer.at(index);
	}
}

void GdsVertexBuffer::SetVertexToIndex( int index , GDSVERTEX& vertex )
{
	if( index >= 0 && index < m_pBuffer.size() )
	{
		GDSVERTEX& p = m_pBuffer.at(index);
		p = vertex;
	}
}