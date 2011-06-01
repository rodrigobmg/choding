#ifndef _GDS_CORE_TERRAIN_TRIANGLE_H_
#define _GDS_CORE_TERRAIN_TRIANGLE_H_

#include "../../Framework/Snowboard_stdafx.h"
#include "../Resource/Type/GdsIndexBuffer.h"

struct TRIANGLE
{
	// direction LEFT:0 , NEAR:1 , RIGHT:2 , FAR:3
	//		  /| p2
	//center / |
	//		/  |
	//   p1/---|  corner
	//  order to triangle index p1 , corner , p2
	enum DIR
	{
		NONE = 0,
		WEST,
		SOUTH,
		EAST,
		NORTH,
		MAX,
	};

	D3DXVECTOR3 p1 , corner , p2 , center;
	int dir,m_iLodlv;
	TRIANGLE*	m_pLeft;
	TRIANGLE*	m_pRight;

	TRIANGLE();
	~TRIANGLE();

	void	checkVertexOrder( D3DXVECTOR3& p1 , D3DXVECTOR3& corner , D3DXVECTOR3& p2 );
	void	calcDir( D3DXVECTOR3& center , D3DXVECTOR3& corner , int& dir );
	void	split( TRIANGLE* tri , int lodlv , int iLodLimit );
	void	genIndexTemplet( GdsIndexBufferPtr pIB , int ilodlv , int icrackDir );
	void	GetVertex( GDSVERTEX* tile , int x , int z , GDSVERTEX& vertex );
	void	GetIndex( D3DXVECTOR3& vertex , WORD& index );
};

#endif
