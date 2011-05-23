#include "GdsTerrain.h"
#include "Resource\GdsResMgr.h"
#include "..\..\System\Logger\logger.h"
#include "Renderer\GdsRendererDX9.h"
#include "Renderer\GdsRenderStateGroup.h"

GdsTerrain::GdsTerrain() 
: m_iVertexPerNode( 16 )
, m_pIB( NULL )
, m_ixheight( 0 )
, m_izheight( 0 )
, m_iMaxLOD( 6 )
, m_pRootTile(NULL)
{
	SetName( OBJECT_TERRAIN );	
	
	//set renderstate
	GdsRenderStateGroupPtr renderstate = GdsRenderStateGroupPtr( new GdsRenderStateGroup );
	renderstate->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0번 텍스처 스테이지의 확대 필터

	renderstate->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		/// 0번 텍스처 : 0번 텍스처 인덱스 사용
	renderstate->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	renderstate->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	renderstate->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	renderstate->SetRenderState( D3DRS_ZENABLE, TRUE );
	renderstate->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	renderstate->SetRenderState( D3DRS_LIGHTING, FALSE );

	RENDERER.GetRenderFrame()->AddRenderStateGroup( renderstate , TERRAIN_RENDERSTATE );
}

GdsTerrain::~GdsTerrain()
{
	vClear();	
}

void GdsTerrain::vClear()
{
	SAFE_DELETE( m_pRootTile );
}


void GdsTerrain::build( TILE* tile , GDSVERTEX* pVB )
{
	tile->m_cenPos = ( tile->m_minPos + tile->m_maxPos ) * 0.5;
	float fDist = tile->m_maxPos.x - tile->m_minPos.x;
	if ( abs( fDist ) > m_iVertexPerNode )
	{
		tile->m_pChild[0] = new TILE;
		tile->m_pChild[0]->m_minPos = tile->m_minPos;
		tile->m_pChild[0]->m_maxPos = tile->m_cenPos;		

		tile->m_pChild[1] = new TILE;
		tile->m_pChild[1]->m_minPos = D3DXVECTOR3( tile->m_cenPos.x , 0 , tile->m_minPos.z );
		tile->m_pChild[1]->m_maxPos = D3DXVECTOR3( tile->m_maxPos.x , 0 , tile->m_cenPos.z );

		tile->m_pChild[2] = new TILE;
		tile->m_pChild[2]->m_minPos = tile->m_cenPos;
		tile->m_pChild[2]->m_maxPos = tile->m_maxPos;

		tile->m_pChild[3] = new TILE;
		tile->m_pChild[3]->m_minPos = D3DXVECTOR3( tile->m_minPos.x , 0 , tile->m_cenPos.z );
		tile->m_pChild[3]->m_maxPos = D3DXVECTOR3( tile->m_cenPos.x , 0 , tile->m_maxPos.z );

		build( tile->m_pChild[0] , pVB );
		build( tile->m_pChild[1] , pVB );
		build( tile->m_pChild[2] , pVB );
		build( tile->m_pChild[3] , pVB );
	}
	else
	{
		tile->m_pVertex = new GDSVERTEX[ m_iVertexPerNode*m_iVertexPerNode ];
		int icount = 0;
		float minY = 0;
		float maxY = 0;
		for ( int x = static_cast<int>(tile->m_minPos.x) ; x < static_cast<int>(tile->m_maxPos.x) ; x++ )
		{
			for ( int z = static_cast<int>(tile->m_minPos.z) ; z < static_cast<int>(tile->m_maxPos.z) ; z++ )
			{
				tile->m_pVertex[icount] = pVB[ z*m_iVertexPerNode + x];
				if ( tile->m_pVertex[icount].p.y > maxY )
					maxY = tile->m_pVertex[icount].p.y;

				if ( tile->m_pVertex[icount].p.y < minY )
					minY = tile->m_pVertex[icount].p.y;
				
				icount++;				
			}		
		}		
		tile->m_minPos.y = minY;
		tile->m_maxPos.y = maxY;
		tile->m_RenderToken = GdsRenderObjectPtr( new GdsRenderObject );

		LPDIRECT3DVERTEXBUFFER9 pVB;
		if( FAILED( RENDERER.GetDevice()->CreateVertexBuffer( m_iVertexPerNode*m_iVertexPerNode*sizeof( GDSVERTEX ),
			0, GDSVERTEX::FVF,
			D3DPOOL_DEFAULT, &pVB, NULL ) ) )
		{
			return;
		}
		VOID* pVertices;
		if( FAILED( pVB->Lock( 0, m_iVertexPerNode*m_iVertexPerNode*sizeof(GDSVERTEX), (void**)&pVertices, 0 ) ) )
			return;

		memcpy( pVertices , tile->m_pVertex , m_iVertexPerNode*m_iVertexPerNode*sizeof(GDSVERTEX) );
		pVB->Unlock();

		tile->m_RenderToken->SetVertexBuffer( pVB );
		tile->m_RenderToken->SetVertexMaxCount( m_iVertexPerNode*m_iVertexPerNode );
		tile->m_RenderToken->SetVertexSize( sizeof( GDSVERTEX ) );
		tile->m_RenderToken->SetFVF( GDSVERTEX::FVF );
		tile->m_RenderToken->SetStartVertexIndex( 0 );
		tile->m_RenderToken->SetEndVertexIndex( m_iVertexPerNode*m_iVertexPerNode );

	}
}

bool GdsTerrain::MakeHeightMap( GdsNodePtr pNode )
{
	if ( m_pRootTile != NULL )
	{
		ASSERT( 0 && "중복 로딩" );
		return false;
	}

	GdsResTexturePtr texheight = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"map.bmp") );
	GdsResTexturePtr texcolor = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"tile2.tga") );

	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;
	int cxHeight , czHeight;

	texheight->Get()->GetLevelDesc( 0, &ddsd );	/// 텍스처의 정보
	cxHeight = ddsd.Width;				/// 텍스처의 가로크기
	czHeight = ddsd.Height;				/// 텍스처의 세로크기

	int icheckx = cxHeight % m_iVertexPerNode;
	int icheckz = czHeight % m_iVertexPerNode;
	if ( icheckx != 0 || icheckz != 0 || cxHeight < m_iVertexPerNode || czHeight < m_iVertexPerNode )
	{
		LOG_ERROR_F( "Texture Size Error:[%d,%d]", cxHeight, czHeight );
		return false;
	}

	icheckx = cxHeight / m_iVertexPerNode;
	icheckz = czHeight / m_iVertexPerNode;
	if ( icheckx > 0 && icheckz > 0 )
	{
		m_ixheight = icheckx;
		m_izheight = icheckz;
	}
	else
	{
		ASSERT( 0 );
		return false;
	}


	LPDIRECT3DVERTEXBUFFER9 pVB;
	LOG_WARNING_F( "Texture Size:[%d,%d]", cxHeight, czHeight );
	if( FAILED( RENDERER.GetDevice()->CreateVertexBuffer( ddsd.Width*ddsd.Height*sizeof( GDSVERTEX ),
		0, GDSVERTEX::FVF,
		D3DPOOL_DEFAULT, &pVB, NULL ) ) )
	{
		return false;
	}

	/// 텍스처 메모리 락!
	texheight->Get()->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	VOID* pVertices;
	/// 정점버퍼 락!
	if( FAILED( pVB->Lock( 0, cxHeight*czHeight*sizeof(GDSVERTEX), (void**)&pVertices, 0 ) ) )
		return false;

	GDSVERTEX	v;
	GDSVERTEX*	pV = (GDSVERTEX*)pVertices;
	
	D3DXVECTOR3	minPos(0.f , 0.f, 0.f );
	D3DXVECTOR3 maxPos( 0.f, 0.f , 0.f );

	for( int z = 0 ; z < czHeight ; z++ )
	{
		for( int x = 0 ; x < cxHeight ; x++ )
		{
			v.p.x = -((float)x-cxHeight);		/// 정점의 x좌표(메시를 원점에 정렬)
			v.p.z = -((float)z-czHeight);	/// 정점의 z좌표(메시를 원점에 정렬), z축이 모니터안쪽이므로 -를 곱한다.
			v.p.y = ((float)(*((LPDWORD)d3drc.pBits+x+z*(d3drc.Pitch/4))&0x000000ff))/10.0f;	/// DWORD이므로 pitch/4
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize( &v.n, &v.n );
			v.t.x = (float)x / (cxHeight-1);
			v.t.y = (float)z / (czHeight-1);
			*pV++ = v;

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
	}
	pVB->Unlock();
	texheight->Get()->UnlockRect( 0 );

	m_pRootTile = new TILE;
	m_pRootTile->m_minPos = minPos;
	m_pRootTile->m_maxPos = maxPos;
	build( m_pRootTile , (GDSVERTEX*)pVertices );


	LPDIRECT3DINDEXBUFFER9 m_pIB;
	if( FAILED( RENDERER.GetDevice()->CreateIndexBuffer( ( m_iVertexPerNode-1)*(m_iVertexPerNode-1)*2 * sizeof(GDSINDEX), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL ) ) )
	{
		return false;
	}

	GDSINDEX	i;
	GDSINDEX*	pI;

	if( FAILED( m_pIB->Lock( 0, (m_iVertexPerNode-1)*(m_iVertexPerNode-1)*2 * sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
		return false;

	for( int z = 0 ; z < m_iVertexPerNode-1 ; z++ )
	{
		for( int x = 0 ; x < m_iVertexPerNode-1 ; x++ )
		{
			i._0 = (z*m_iVertexPerNode+x);
			i._1 = (z*m_iVertexPerNode+x+1);
			i._2 = ((z+1)*m_iVertexPerNode+x);
			*pI++ = i;
			i._0 = ((z+1)*m_iVertexPerNode+x);
			i._1 = (z*m_iVertexPerNode+x+1);
			i._2 = ((z+1)*m_iVertexPerNode+x+1);
			*pI++ = i;
		}
	}
	m_pIB->Unlock();

	pNode->SetDrawAxis( true );
	pNode->SetDrawBox( true );
	//pNode->SetScale( 10.f );

	genIndex( m_pRootTile );

	return true;
}

void GdsTerrain::genIndex( TILE* tile )
{
	if ( tile == NULL )
		return;

	if ( tile->m_pChild[0] ) genIndex( tile->m_pChild[0] );
	if ( tile->m_pChild[1] ) genIndex( tile->m_pChild[1] );
	if ( tile->m_pChild[2] ) genIndex( tile->m_pChild[2] );
	if ( tile->m_pChild[3] ) genIndex( tile->m_pChild[3] );

	if ( tile->m_RenderToken == NULL )
		return;

	if ( tile->m_pVertex == NULL )
		return;	

	tile->m_RenderToken->SetIndexMaxCount( (m_iVertexPerNode-1)*(m_iVertexPerNode-1)*2 );
	tile->m_RenderToken->SetIndexBuffer( m_pIB );
	tile->m_RenderToken->SetStartIndex( 0 );
	tile->m_RenderToken->SetEndIndex( (m_iVertexPerNode-1)*(m_iVertexPerNode-1)*2 );	

}