
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"
#include "..\..\..\chodingModule\Snowboard\Core\Terrain\GdsTerrain.h"
#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsIndexBuffer.h"
#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsVertexBuffer.h"

class GdsTerrainTest : public ::testing::Test
{
public:
	GdsTerrainTest()
	{
	}

	virtual ~GdsTerrainTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		//¼Ò¸êÀÚ
	}

	virtual void TestBody()
	{		
	}
};

struct TRIANGLE
{
	// direction LEFT:0 , NEAR:1 , RIGHT:2 , FAR:3
	//		  /| p2
	//center / |
	//		/  |
	//   p1/---|  corner
	//  order to triangle index p1 , corner , p2

	//int p1 , corner , p2 , center; // index of vertex
	enum DIR
	{
		NONE = 0,
		WEST,
		SOUTH,
		EAST,
		NORTH,
	};

	D3DXVECTOR3 p1 , corner , p2 , center;
	int dir,m_iLodlv;
	TRIANGLE*	m_pLeft;
	TRIANGLE*	m_pRight;

	TRIANGLE()
	{
		m_iLodlv = dir = 0;
		m_pRight = m_pLeft = NULL;
	}
	~TRIANGLE()
	{
		SAFE_DELETE( m_pLeft );
		SAFE_DELETE( m_pRight );
	}

	void calcDir( D3DXVECTOR3& center , D3DXVECTOR3& corner , int& dir )
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
	
	void split( TRIANGLE* tri , int idir , int lodlv )
	{
		lodlv++;
		if ( lodlv > 4 )
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

		split( tri->m_pLeft , 0 , tri->m_iLodlv );
		split( tri->m_pRight , 0 , tri->m_iLodlv );
	}

	void genIndex( std::vector< D3DXVECTOR3 >& vecList , int ilodlv , int icrackDir )
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

		if ( m_pLeft ) m_pLeft->genIndex( vecList , ilodlv , icrackDir );
		if ( m_pRight )	m_pRight->genIndex( vecList , ilodlv , icrackDir );		
	}

};

void GetVertex( GDSVERTEX* tile , int x , int z , GDSVERTEX& vertex )
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

void GetIndex( GDSVERTEX& vertex , int& index )
{
	index = (vertex.p.z)*(33) + vertex.p.x;
}

TEST_F( GdsTerrainTest,  MakeHeightMap )
{	 
	GdsNodePtr pNode = GdsNodePtr( new GdsNode );
	TERRAIN.MakeHeightMap();		

	GDSVERTEX map[33*33];
	int xheight , zheight;
	xheight = zheight = 33;

	GdsVertexBufferPtr vertexBuffer = GdsVertexBufferPtr( new GdsVertexBuffer );
	for (int z = 0 ; z < zheight ; z++ )
	{
		for (int x=0; x< xheight ; x++ )
		{
			map[z*xheight + x ].p = D3DXVECTOR3( x , 0 , z );
			vertexBuffer->AddVertex( map[z*xheight + x ] );
		}		
	}

	GDSVERTEX testVertex;
	for ( int x=0 ; x < xheight ; x++ )
	{
		for ( int z=0 ; z < zheight ; z++ )
		{
			GetVertex( map , x , z , testVertex );
			EXPECT_EQ( x , (int)testVertex.p.x );
			EXPECT_EQ( z , (int)testVertex.p.z );
			int index=0;
			GetIndex( testVertex , index );
			EXPECT_EQ( z*xheight+x , index );
		}
	}
	

	EXPECT_EQ( xheight*zheight , vertexBuffer->GetVertexMaxCount() );
	vertexBuffer->Alloc();


	TRIANGLE* pRootTri1 = new TRIANGLE;	

	pRootTri1->m_pLeft = new TRIANGLE;
	pRootTri1->m_pLeft->p1 = map[0].p;
	pRootTri1->m_pLeft->corner = map[ 0 * (zheight) + (xheight-1) ].p;
	pRootTri1->m_pLeft->p2 = map[(zheight-1)*(zheight) + xheight-1 ].p;

	pRootTri1->m_pRight = new TRIANGLE;
	pRootTri1->m_pRight->p1 = map[ (zheight-1)*(zheight) + xheight-1 ].p;
	pRootTri1->m_pRight->corner = map[ (zheight)*(xheight-1) + 0 ].p;
	pRootTri1->m_pRight->p2 = map[0].p;

	pRootTri1->split( pRootTri1->m_pLeft , 0 , pRootTri1->m_iLodlv );
	pRootTri1->split( pRootTri1->m_pRight , 0 , pRootTri1->m_iLodlv );

	std::vector< D3DXVECTOR3 > vecList0;
	pRootTri1->genIndex( vecList0 , 0 , 0 );

	std::vector< D3DXVECTOR3 > vecList1;
	pRootTri1->genIndex( vecList1 , 1 , TRIANGLE::EAST );

	std::vector< D3DXVECTOR3 > vecList2;
	pRootTri1->genIndex( vecList2 , 2 , TRIANGLE::EAST );

	std::vector< D3DXVECTOR3 > vecList3;
	pRootTri1->genIndex( vecList3 , 3 , 0 );

	std::vector< D3DXVECTOR3 > vecList4;
	pRootTri1->genIndex( vecList4 , 4 , 0 );

	std::vector< D3DXVECTOR3 > vecList5;
	pRootTri1->genIndex( vecList5 , 5 , 0 );

	std::vector< D3DXVECTOR3 > vecList6;
	pRootTri1->genIndex( vecList6 , 6 , 0 );	

	SAFE_DELETE( pRootTri1 );
}
