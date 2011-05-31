#include "GdsIndexBuffer.h"
#include "..\GdsResMgr.h"

GdsIndexBuffer::GdsIndexBuffer()
: m_Index_Maxcount(0)
, m_pIB(NULL)
, m_ePrimitiveType( D3DPT_TRIANGLELIST )
{
	vClear();
}

GdsIndexBuffer::~GdsIndexBuffer()
{	
	vRelease();
}


HRESULT GdsIndexBuffer::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	return S_OK;
}

HRESULT GdsIndexBuffer::vRelease()
{
	if (m_pIB )
	{
		RESMGR.FreeIndexBuffer( m_pIB );
	}
	m_pBuffer.clear();
	
	return S_OK;
	
}

void GdsIndexBuffer::vClear()
{
	m_pBuffer.clear();	
}

void GdsIndexBuffer::Alloc()
{
	if ( !IsBufferEmpty() && m_Index_Maxcount > 0 )
	{
		RESMGR.AllocIndexBuffer( m_pIB , m_Index_Maxcount*sizeof( GDSINDEX ) );

		if ( m_pIB == NULL )
			return;

		VOID* pI;
		if( FAILED( m_pIB->Lock( 0, m_Index_Maxcount*sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
			return;
		
		GDSINDEX* p = (GDSINDEX*)pI;
		for ( size_t i=0 ; i<m_Index_Maxcount ; i++ )
		{			
			*p++ = m_pBuffer.at(i);
		}
		m_pIB->Unlock();

		m_pBuffer.clear();	
		m_iStartIndex = 0;
		m_iEndIndex = m_Index_Maxcount;
	}
}

void GdsIndexBuffer::Free()
{
	if ( m_pIB )
	{
		RESMGR.FreeIndexBuffer(m_pIB);
	}
}

void GdsIndexBuffer::AddIndex( GDSINDEX& index )
{
	m_Index_Maxcount++;
	m_pBuffer.push_back( index );
}
