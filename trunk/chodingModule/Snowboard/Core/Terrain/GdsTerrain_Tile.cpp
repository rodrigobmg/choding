#include "GdsTerrain_Tile.h"

TILE::TILE()
{
	m_pParent = NULL;
	m_pVB		= GdsVertexBufferPtr( (GdsVertexBuffer*)NULL );
	m_iLOD = 0;
	m_iMaxLOD = 3;						
	m_maxPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
	m_minPos = D3DXVECTOR3( 0.f , 0.f , 0.f );
	m_cenPos = D3DXVECTOR3( 0.f , 0.f , 0.f );

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

