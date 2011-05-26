
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"
#include "..\..\..\chodingModule\Snowboard\Core\Terrain\GdsTerrain.h"

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

	/*
	void checkorder( D3DXVECTOR3& corner , D3DXVECTOR3& p1 , D3DXVECTOR3& p2 )
		{
			if ( p1.x == p2.x )
			{
				if( p1.z < p2.z )
				{
					swap( p1 , p2 );
					return;
				}
			}
			else if ( p1.z == p2.z )
			{
				if( p1.x)
			}
		}*/
	
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
		
		if ( lodlv == 1 )
		{
			calcDir( (tri->m_pLeft->p1 + tri->m_pLeft->p2 ) *0.5 , tri->m_pLeft->corner , tri->m_pLeft->dir );
			calcDir( (tri->m_pRight->p1 + tri->m_pRight->p2 )*0.5 , tri->m_pRight->corner , tri->m_pRight->dir );
		}
		else if ( lodlv > 1 )
		{
			tri->m_pLeft->dir = tri->dir;
			tri->m_pRight->dir = tri->dir;
		}

		split( tri->m_pLeft , 0 , tri->m_iLodlv );
		split( tri->m_pRight , 0 , tri->m_iLodlv );
		/*
			switch( idir )
					{
					case 1: // LEFT
						break;
					case 2: // NEAR
						break;
					case 3: //RIGHT
						break;
					case 4: //FAR
						break;
					default:
						break;
			
		}*/
	}

	void genIndex( std::vector< D3DXVECTOR3 >& vecList , int ilodlv )
	{
		if ( ilodlv < m_iLodlv )
			return;

		if ( ilodlv == m_iLodlv && m_pLeft != NULL && m_pRight != NULL )
		{
			vecList.push_back( m_pLeft->p1 );
			vecList.push_back( m_pLeft->corner );
			vecList.push_back( m_pLeft->p2 );

			vecList.push_back( m_pRight->p1 );
			vecList.push_back( m_pRight->corner );
			vecList.push_back( m_pRight->p2 );
		}		

		if ( m_pLeft ) m_pLeft->genIndex( vecList , ilodlv );
		if ( m_pRight )	m_pRight->genIndex( vecList , ilodlv );		
	}

};
TEST_F( GdsTerrainTest,  MakeHeightMap )
{	 
	GdsNodePtr pNode = GdsNodePtr( new GdsNode );
	TERRAIN.MakeHeightMap();		

	D3DXVECTOR3 map[33*33];
	int xheight , zheight;
	xheight = zheight = 33;
	for (int z = 0 ; z < zheight ; z++ )
	{
		for (int x=0; x< xheight ; x++ )
		{
			map[z*xheight + x ] = D3DXVECTOR3( x , 0 , z );
		}		
	}


	TRIANGLE* pRootTri1 = new TRIANGLE;	

	pRootTri1->m_pLeft = new TRIANGLE;
	pRootTri1->m_pLeft->p1 = map[0];
	pRootTri1->m_pLeft->corner = map[ 0 * (zheight) + (xheight-1) ];
	pRootTri1->m_pLeft->p2 = map[(zheight-1)*(zheight) + xheight-1 ];

	pRootTri1->m_pRight = new TRIANGLE;
	pRootTri1->m_pRight->p1 = map[ (zheight-1)*(zheight) + xheight-1 ];
	pRootTri1->m_pRight->corner = map[ (zheight)*(xheight-1) + 0 ];
	pRootTri1->m_pRight->p2 = map[0];

	pRootTri1->split( pRootTri1->m_pLeft , 0 , pRootTri1->m_iLodlv );
	pRootTri1->split( pRootTri1->m_pRight , 0 , pRootTri1->m_iLodlv );

	std::vector< D3DXVECTOR3 > vecList0;
	pRootTri1->genIndex( vecList0 , 0 );

	std::vector< D3DXVECTOR3 > vecList1;
	pRootTri1->genIndex( vecList1 , 1 );

	std::vector< D3DXVECTOR3 > vecList2;
	pRootTri1->genIndex( vecList2 , 2 );

	std::vector< D3DXVECTOR3 > vecList3;
	pRootTri1->genIndex( vecList3 , 3 );

	std::vector< D3DXVECTOR3 > vecList4;
	pRootTri1->genIndex( vecList4 , 4 );

	std::vector< D3DXVECTOR3 > vecList5;
	pRootTri1->genIndex( vecList5 , 5 );

	std::vector< D3DXVECTOR3 > vecList6;
	pRootTri1->genIndex( vecList6 , 6 );
}
