#include "GdsTerrain_Triangle.h"

TRIANGLE::TRIANGLE()
{
	m_iLodlv = dir = 0;
	m_pRight = m_pLeft = NULL;
}

TRIANGLE::~TRIANGLE()
{
	SAFE_DELETE( m_pLeft );
	SAFE_DELETE( m_pRight );
}

void TRIANGLE::calcDir( D3DXVECTOR3& center , D3DXVECTOR3& corner , int& dir )
{
	if( center.z == corner.z )
	{
		if ( center.x > corner.x )
		{
			dir = EAST;
		}
		else
		{
			dir = WEST;
		}
	}
	else if ( center.x == corner.x )
	{
		if ( center.z > corner.z )
		{
			dir = NORTH;
		}
		else
		{
			dir = SOUTH;
		}
	}
}

void TRIANGLE::split( TRIANGLE* tri , int idir , int lodlv , int iLodLimit )
{
	lodlv++;
	if ( lodlv > iLodLimit )
		return;

	tri->center = ( tri->p1 + tri->p2 ) * 0.5;
	tri->m_pLeft = new TRIANGLE;
	tri->m_pRight = new TRIANGLE;
	tri->m_iLodlv = lodlv;

	tri->m_pLeft->corner	= tri->center;
	tri->m_pLeft->p1		= tri->p1;
	tri->m_pLeft->p2		= tri->corner;

	tri->m_pRight->corner	= tri->center;
	tri->m_pRight->p1		= tri->corner;
	tri->m_pRight->p2		= tri->p2;

	if ( lodlv > 0  )
	{
		calcDir( (tri->m_pLeft->p1 + tri->m_pLeft->p2 ) *0.5 , tri->m_pLeft->corner , tri->m_pLeft->dir );
		calcDir( (tri->m_pRight->p1 + tri->m_pRight->p2 )*0.5 , tri->m_pRight->corner , tri->m_pRight->dir );
	}

	split( tri->m_pLeft , 0 , tri->m_iLodlv , iLodLimit );
	split( tri->m_pRight , 0 , tri->m_iLodlv , iLodLimit );
}

void TRIANGLE::genIndexTemplet( std::vector< D3DXVECTOR3 >& vecList , int ilodlv , int icrackDir )
{
	if ( ilodlv < m_iLodlv )
		return;

	if ( ilodlv == m_iLodlv && m_pLeft != NULL && m_pRight != NULL )
	{
		if ( icrackDir == m_pLeft->dir && icrackDir != NONE 
			&& m_pLeft->m_pLeft != NULL && m_pLeft->m_pRight != NULL 
			)
		{
			vecList.push_back( m_pLeft->m_pLeft->p1 );
			vecList.push_back( m_pLeft->m_pLeft->corner );
			vecList.push_back( m_pLeft->m_pLeft->p2 );

			vecList.push_back( m_pLeft->m_pRight->p1 );
			vecList.push_back( m_pLeft->m_pRight->corner );
			vecList.push_back( m_pLeft->m_pRight->p2 );

			vecList.push_back( m_pRight->p1 );
			vecList.push_back( m_pRight->corner );
			vecList.push_back( m_pRight->p2 );
		}
		else if ( icrackDir == m_pRight->dir && icrackDir != NONE 
			&& m_pRight->m_pLeft != NULL && m_pRight->m_pRight != NULL 
			)
		{
			vecList.push_back( m_pRight->m_pLeft->p1 );
			vecList.push_back( m_pRight->m_pLeft->corner );
			vecList.push_back( m_pRight->m_pLeft->p2 );

			vecList.push_back( m_pRight->m_pRight->p1 );
			vecList.push_back( m_pRight->m_pRight->corner );
			vecList.push_back( m_pRight->m_pRight->p2 );				

			vecList.push_back( m_pLeft->p1 );
			vecList.push_back( m_pLeft->corner );
			vecList.push_back( m_pLeft->p2 );
		}
		else
		{
			vecList.push_back( m_pLeft->p1 );
			vecList.push_back( m_pLeft->corner );
			vecList.push_back( m_pLeft->p2 );

			vecList.push_back( m_pRight->p1 );
			vecList.push_back( m_pRight->corner );
			vecList.push_back( m_pRight->p2 );
		}			
	}		

	if ( m_pLeft ) m_pLeft->genIndexTemplet( vecList , ilodlv , icrackDir );
	if ( m_pRight )	m_pRight->genIndexTemplet( vecList , ilodlv , icrackDir );		
}


void TRIANGLE::GetVertex( GDSVERTEX* tile , int x , int z , GDSVERTEX& vertex )
{
	if ( x >= 33 )
		x = 33-1;
	if ( z >= 33 )
		z = 33-1;
	if ( x < 0 )
		x = 0;
	if ( z < 0 )
		z = 0;

	vertex = tile[ z*(33) + x ];
}

void TRIANGLE::GetIndex( D3DXVECTOR3& vertex , int& index )
{
	index = (vertex.z)*(33) + vertex.x;
}