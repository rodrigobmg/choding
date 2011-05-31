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

void TRIANGLE::split( TRIANGLE* tri , int lodlv , int iLodLimit )
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
		D3DXVECTOR3 center , corner;
		center = ( tri->m_pLeft->p1 + tri->m_pLeft->p2 )*0.5;
		corner = tri->m_pLeft->corner;
		calcDir( center , corner , tri->m_pLeft->dir );

		center = (tri->m_pRight->p1 + tri->m_pRight->p2 )*0.5;
		corner = tri->m_pRight->corner;
		calcDir( center , corner , tri->m_pRight->dir );
	}

	split( tri->m_pLeft , tri->m_iLodlv , iLodLimit );
	split( tri->m_pRight , tri->m_iLodlv , iLodLimit );
}

void TRIANGLE::genIndexTemplet( GdsIndexBufferPtr pIB , int ilodlv , int icrackDir )
{
	if ( ilodlv < m_iLodlv )
		return;

	if ( ilodlv == m_iLodlv && m_pLeft != NULL && m_pRight != NULL )
	{
		if ( icrackDir == m_pLeft->dir && icrackDir != NONE 
			&& m_pLeft->m_pLeft != NULL && m_pLeft->m_pRight != NULL 
			)
		{
 			GDSINDEX index;
 			GetIndex( m_pLeft->m_pLeft->p1 , index._0 );
 			GetIndex( m_pLeft->m_pLeft->corner , index._1 );
 			GetIndex( m_pLeft->m_pLeft->p2 , index._2 );
 			pIB->AddIndex( index );
 
 			GetIndex( m_pLeft->m_pRight->p1 , index._0 );
 			GetIndex( m_pLeft->m_pRight->corner , index._1 );
 			GetIndex( m_pLeft->m_pRight->p2 , index._2 );
 			pIB->AddIndex( index );
 
 			GetIndex( m_pRight->p1 , index._0 );
 			GetIndex( m_pRight->corner , index._1 );
 			GetIndex( m_pRight->p2 , index._2 );
 			pIB->AddIndex( index );

		}
		else if ( icrackDir == m_pRight->dir && icrackDir != NONE 
			&& m_pRight->m_pLeft != NULL && m_pRight->m_pRight != NULL 
			)
		{
 			GDSINDEX index;
 			GetIndex( m_pRight->m_pLeft->p1 , index._0 );
 			GetIndex( m_pRight->m_pLeft->corner , index._1 );
 			GetIndex( m_pRight->m_pLeft->p2 , index._2 );
 			pIB->AddIndex( index );
 
 			GetIndex( m_pRight->m_pRight->p1 , index._0 );
 			GetIndex( m_pRight->m_pRight->corner , index._1 );
 			GetIndex( m_pRight->m_pRight->p2 , index._2 );
 			pIB->AddIndex( index );
 
 			GetIndex( m_pLeft->p1 , index._0 );
 			GetIndex( m_pLeft->corner , index._1 );
 			GetIndex( m_pLeft->p2 , index._2 );
 			pIB->AddIndex( index );

		}
		else
		{
 			GDSINDEX index;
 			GetIndex( m_pLeft->p1 , index._0 );
 			GetIndex( m_pLeft->corner , index._1 );
 			GetIndex( m_pLeft->p2 , index._2 );
 			pIB->AddIndex( index );
 
 			GetIndex( m_pRight->p1 , index._0 );
 			GetIndex( m_pRight->corner , index._1 );
 			GetIndex( m_pRight->p2 , index._2 );
 			pIB->AddIndex( index );

		}			
	}		

	if ( m_pLeft ) m_pLeft->genIndexTemplet( pIB , ilodlv , icrackDir );
	if ( m_pRight )	m_pRight->genIndexTemplet( pIB , ilodlv , icrackDir );		
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

void TRIANGLE::GetIndex( D3DXVECTOR3& vertex , WORD& index )
{
	index = (vertex.z)*(33) + vertex.x;
}