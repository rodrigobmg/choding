#include "stdafx.h"

TILE::TILE()
:m_pParent( NULL )
,m_pVB( GdsVertexBufferPtr( (GdsVertexBuffer*)NULL ) )
,m_iLOD(0)
,m_iCrackLOD(0)
,m_maxPos( 0.f , 0.f , 0.f )
,m_minPos( 0.f , 0.f , 0.f )
,m_cenPos( 0.f , 0.f , 0.f )
,m_fRadius( 0.f )
{
	for ( int i=0 ;i < 4 ; i++)
	{
		m_pChild[i] = NULL;
	}
}

TILE::~TILE()
{
	for ( int i=0 ;i < 4 ; i++)
	{
		SAFE_DELETE( m_pChild[i] );
	}	
}

