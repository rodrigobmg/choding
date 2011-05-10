
#pragma once

#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\Octree\GdsOctree.h"
#include "..\..\..\chodingModule\Snowboard\Core\Resource\Type\GdsResTexture.h"
#include "..\..\..\chodingModule\Snowboard\Core\Resource\GdsResMgr.h"
#include "..\..\..\chodingModule\Snowboard\System\Logger\logger.h"
#include "..\..\..\chodingModule\Snowboard\Core\Renderer\GdsRendererDX9.h"

class GdsOctreeTest : public ::testing::Test
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB;
	LPDIRECT3DINDEXBUFFER9 g_pIB;
	int m_iNumFaces;
	int g_cxHeight , g_czHeight;

public:
	GdsOctreeTest()
	{
	}

	virtual ~GdsOctreeTest()
	{
	}

	virtual void SetUp()
	{
		//생성자
		GdsResTexturePtr texheight = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"height8.bmp") );
		GdsResTexturePtr texcolor = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"tile2.tga") );

		D3DSURFACE_DESC		ddsd;
		D3DLOCKED_RECT		d3drc;
		
		texheight->Get()->GetLevelDesc( 0, &ddsd );	/// 텍스처의 정보
		g_cxHeight = ddsd.Width;				/// 텍스처의 가로크기
		g_czHeight = ddsd.Height;				/// 텍스처의 세로크기
		
		LOG_WARNING_F( "Texture Size:[%d,%d]", g_cxHeight, g_czHeight );
		if( FAILED( RENDERER.GetDevice()->CreateVertexBuffer( ddsd.Width*ddsd.Height*sizeof( GDSVERTEX ),
			0, GDSVERTEX::FVF,
			D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
		{
			return;
		}

		/// 텍스처 메모리 락!
		texheight->Get()->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
		VOID* pVertices;
		/// 정점버퍼 락!
		if( FAILED( g_pVB->Lock( 0, g_cxHeight*g_czHeight*sizeof(GDSVERTEX), (void**)&pVertices, 0 ) ) )
			return;

		GDSVERTEX	v;
		GDSVERTEX*	pV = (GDSVERTEX*)pVertices;
		for( DWORD z = 0 ; z < g_czHeight ; z++ )
		{
			for( DWORD x = 0 ; x < g_cxHeight ; x++ )
			{
				v.p.x = (float)x-g_cxHeight/2.0f;		/// 정점의 x좌표(메시를 원점에 정렬)
				v.p.z = -((float)z-g_czHeight/2.0f);	/// 정점의 z좌표(메시를 원점에 정렬), z축이 모니터안쪽이므로 -를 곱한다.
				v.p.y = ((float)(*((LPDWORD)d3drc.pBits+x+z*(d3drc.Pitch/4))&0x000000ff))/10.0f;	/// DWORD이므로 pitch/4
				v.n.x = v.p.x;
				v.n.y = v.p.y;
				v.n.z = v.p.z;
				D3DXVec3Normalize( &v.n, &v.n );
				v.t.x = (float)x / (g_cxHeight-1);
				v.t.y = (float)z / (g_czHeight-1);
				*pV++ = v;
				//			g_pLog->Log( "[%f,%f,%f]", v.x, v.y, v.z );
			}
		}


		D3DXVECTOR3 minPos( 0,0,0 );
		D3DXVECTOR3 maxPos( 0,0,0 );
		pV = (GDSVERTEX*)pVertices;
		for( DWORD z = 0 ; z < g_czHeight*g_cxHeight ; z++ )
		{
			v =	*pV++;
			if ( minPos.x > v.p.x )
				minPos.x = v.p.x;

			if ( minPos.y > v.p.y )
				minPos.y = v.p.y;

			if ( minPos.z > v.p.z )
				minPos.z = v.p.z;

			if ( maxPos.x < v.p.x )
				maxPos.x = v.p.x;
			if ( maxPos.y < v.p.y )
				maxPos.y = v.p.y;
			if ( maxPos.z < v.p.z )
				maxPos.z = v.p.z;
		}

		pV = (GDSVERTEX*)pVertices;

		g_pVB->Unlock();
		texheight->Get()->UnlockRect( 0 );

		if( FAILED( RENDERER.GetDevice()->CreateIndexBuffer( (g_cxHeight-1)*(g_czHeight-1)*2 * sizeof(GDSINDEX), 0, 
			D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL ) ) )
		{
			return;
		}

		GDSINDEX	i;
		GDSINDEX*	pI;		

		if( FAILED( g_pIB->Lock( 0, (g_cxHeight-1)*(g_czHeight-1)*2 * sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
			return;

		GDSINDEX*	pStart = pI;
		for( DWORD z = 0 ; z < g_czHeight-1 ; z++ )
		{
			for( DWORD x = 0 ; x < g_cxHeight-1 ; x++ )
			{
				i._0 = (z*g_cxHeight+x);
				i._1 = (z*g_cxHeight+x+1);
				i._2 = ((z+1)*g_cxHeight+x);
				*pI++ = i;
				i._0 = ((z+1)*g_cxHeight+x);
				i._1 = (z*g_cxHeight+x+1);
				i._2 = ((z+1)*g_cxHeight+x+1);
				*pI++ = i;
			}
		}
		g_pIB->Unlock();

		m_iNumFaces = (g_cxHeight-1)*(g_czHeight-1)*2 ;

		GdsOctree ocTree( m_iNumFaces , minPos , maxPos , pV , pStart );
		ocTree.SetLimitedFacePerNode( 10 );
		ocTree.Build();


	}

	virtual void TearDown()
	{
		//소멸자
	}

	virtual void TestBody()
	{		
	}
};

 TEST_F( GdsOctreeTest,  OcTree )
 {	 
 }
