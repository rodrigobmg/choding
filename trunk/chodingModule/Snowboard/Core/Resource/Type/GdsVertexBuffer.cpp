#include "GdsVertexBuffer.h"
#include "..\GdsResMgr.h"

GdsVertexBuffer::GdsVertexBuffer()
:m_Vertex_Maxcount(0)
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

		m_pBuffer.clear();
	}
}

void GdsVertexBuffer::Free()
{
	if ( m_pVB )
	{
		RESMGR.FreeVertexBuffer( m_pVB );
	}
}

void GdsVertexBuffer::AddVertex( GDSVERTEX& vertex )
{
	m_Vertex_Maxcount++;
	m_pBuffer.push_back( vertex );
}