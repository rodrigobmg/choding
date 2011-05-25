#include "GdsTerrain.h"
#include "Resource\GdsResMgr.h"
#include "..\..\System\Logger\logger.h"
#include "Renderer\GdsRendererDX9.h"
#include "Renderer\GdsRenderStateGroup.h"

GdsTerrain::GdsTerrain() 
: m_iVertexPerNode( 9 )
, m_pIB( NULL )
, m_ixheight( 0 )
, m_izheight( 0 )
, m_iMaxLOD( 0 )
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
	for ( size_t i=0 ; i<m_iMaxLOD ; i++)
	{
		if ( m_pIB[i] != NULL )
		{
			RESMGR.FreeIndexBuffer( m_pIB[i] );
		}
	}
	SAFE_DELETE(m_pIB);
	
}


void GdsTerrain::build( TILE* tile , GDSVERTEX* pVB )
{
	tile->m_cenPos = ( tile->m_minPos + tile->m_maxPos ) * 0.5;
 	tile->m_cenPos.x = floor( tile->m_cenPos.x );
 	tile->m_cenPos.y = floor( tile->m_cenPos.y );
 	tile->m_cenPos.z = floor( tile->m_cenPos.z );
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

		build( tile->m_pChild[0] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[1] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[2] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[3] , (GDSVERTEX*)pVB );
	}
	else
	{
		tile->m_pVertex = new GDSVERTEX[ m_iVertexPerNode*m_iVertexPerNode ];
		int icount = 0;
		float minY = 0;
		float maxY = 0;
		int istartx = static_cast<int>(tile->m_minPos.x);
		int istartz = static_cast<int>(tile->m_minPos.z);

		for ( int z = istartz ; z < istartz + m_iVertexPerNode ; z++ )
		{
			for ( int x = istartx ; x < istartx + m_iVertexPerNode ; x++ )
			{
				tile->m_pVertex[icount] = pVB[ z*m_ixheight + x];
				if ( tile->m_pVertex[icount].p.y > maxY )
					maxY = tile->m_pVertex[icount].p.y;

				if ( tile->m_pVertex[icount].p.y < minY )
					minY = tile->m_pVertex[icount].p.y;
				
				icount++;				
			}		
		}		
		tile->m_minPos.y = minY;
		tile->m_maxPos.y = maxY;

		RESMGR.AllocRenderObject( tile->m_RenderToken );

		LPDIRECT3DVERTEXBUFFER9 VB;
		RESMGR.AllocVertexBuffer( VB , m_iVertexPerNode*m_iVertexPerNode*sizeof( GDSVERTEX ) );
		
		VOID* pVertices;
		if( FAILED( VB->Lock( 0, m_iVertexPerNode*m_iVertexPerNode*sizeof(GDSVERTEX), (void**)&pVertices, 0 ) ) )
			return;

		memcpy( pVertices , tile->m_pVertex , m_iVertexPerNode*m_iVertexPerNode*sizeof(GDSVERTEX) );
		VB->Unlock();

		tile->m_RenderToken->SetVertexBuffer( VB );
		tile->m_RenderToken->SetVertexMaxCount( m_iVertexPerNode*m_iVertexPerNode );
		tile->m_RenderToken->SetVertexSize( sizeof( GDSVERTEX ) );
		tile->m_RenderToken->SetFVF( GDSVERTEX::FVF );
		tile->m_RenderToken->SetStartVertexIndex( 0 );
		tile->m_RenderToken->SetEndVertexIndex( m_iVertexPerNode*m_iVertexPerNode );

	}
}

bool GdsTerrain::MakeHeightMap()
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

	//maxlod 계산
	for ( int i=m_iVertexPerNode; i > 0 ; i /= 2 )
	{
		m_iMaxLOD++;
	}
	m_iMaxLOD -= 1;

	int icheckx = cxHeight % (m_iVertexPerNode-1);
	int icheckz = czHeight % (m_iVertexPerNode-1);
	if ( icheckx != 0 || icheckz != 0 || cxHeight < (m_iVertexPerNode-1) || czHeight < (m_iVertexPerNode-1) )
	{
		LOG_ERROR_F( "Texture Size Error:[%d,%d]", cxHeight, czHeight );
		return false;
	}

	icheckx = cxHeight / (m_iVertexPerNode-1);
	icheckz = czHeight / (m_iVertexPerNode-1);
	if ( icheckx > 0 && icheckz > 0 )
	{
		m_ixheight = cxHeight;
		m_izheight = czHeight;
	}
	else
	{
		ASSERT( 0 );
		return false;
	}


	LOG_WARNING_F( "Texture Size:[%d,%d]", cxHeight, czHeight );

	/// 텍스처 메모리 락!
	texheight->Get()->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	
	
	D3DXVECTOR3	minPos( 0.f , 0.f, 0.f );
	D3DXVECTOR3 maxPos( 0.f, 0.f , 0.f );
	GDSVERTEX vertex;
	GDSVERTEX* v = new GDSVERTEX[czHeight*cxHeight];
	for( int z = 0 ; z < czHeight ; z++ )
	{
		for( int x = 0 ; x < cxHeight ; x++ )
		{

			vertex.p.x = (float)x;
			vertex.p.z = -( (float)z - ( cxHeight - 1) );
			vertex.p.y = ((float)(*( (LPDWORD)d3drc.pBits+x+z*(d3drc.Pitch/4) )&0x000000ff) ) / 10.0f;	/// DWORD이므로 pitch/4
			vertex.n.x = vertex.p.x;
			vertex.n.y = vertex.p.y;
			vertex.n.z = vertex.p.z;
			D3DXVec3Normalize( &vertex.n, &vertex.n );
			vertex.t.x = (float)x / (cxHeight-1);
			vertex.t.y = (float)z / (czHeight-1);
			
			if ( minPos.x > vertex.p.x )
				minPos.x = vertex.p.x;
			if ( minPos.y > vertex.p.y )
				minPos.y = vertex.p.y;
			if ( minPos.z > vertex.p.z )
				minPos.z = vertex.p.z;

			if ( maxPos.x < vertex.p.x )
				maxPos.x = vertex.p.x;
			if ( maxPos.y < vertex.p.y )
				maxPos.y = vertex.p.y;
			if ( maxPos.z < vertex.p.z )
				maxPos.z = vertex.p.z;

			v[z*cxHeight+x] = vertex;
		}
	}

	texheight->Get()->UnlockRect( 0 );

	m_pRootTile = new TILE;
	m_pRootTile->m_minPos = minPos;
	m_pRootTile->m_maxPos = maxPos;
	build( m_pRootTile , (GDSVERTEX*)v );
	SAFE_DELETE( v );

	createTempletIB();

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

	int iLodlv = 2;
	if ( iLodlv > m_iMaxLOD )
		iLodlv = m_iMaxLOD;

	float iOffset;
	if ( iLodlv == 0 )
		iOffset = 1;
	else
		iOffset = pow( 2.0f , iLodlv );

	tile->m_RenderToken->SetIndexMaxCount( (m_iVertexPerNode-iOffset)*(m_iVertexPerNode-iOffset)*2 );
	tile->m_RenderToken->SetIndexBuffer( m_pIB[iLodlv] );
	tile->m_RenderToken->SetStartIndex( 0 );
	tile->m_RenderToken->SetEndIndex( (m_iVertexPerNode-iOffset)*(m_iVertexPerNode-iOffset)*2 );	
	
	RENDERER.GetRenderFrame()->AttachRenderObject( tile->m_RenderToken , 0 );
}

void GdsTerrain::Update( float fElapsedtime )
{
	if( m_pRootTile )
		genIndex( m_pRootTile );
}

bool GdsTerrain::createTempletIB()
{
	m_pIB = new LPDIRECT3DINDEXBUFFER9[m_iMaxLOD];
	for ( int iLodlv = 0 ; iLodlv< m_iMaxLOD ; iLodlv++)
	{
		m_pIB[iLodlv] = NULL;

		int iOffset;
		if ( iLodlv == 0 )
			iOffset = 1;
		else
			iOffset = static_cast<int>( pow( 2.0f , iLodlv ) );

		RESMGR.AllocIndexBuffer( m_pIB[iLodlv] , (m_iVertexPerNode-iOffset)*(m_iVertexPerNode-iOffset)*2 * sizeof(GDSINDEX) );		

		GDSINDEX*	pI;
		if( FAILED( m_pIB[iLodlv]->Lock( 0, (m_iVertexPerNode-iOffset)*(m_iVertexPerNode-iOffset)*2 * sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
			return false;

		GDSINDEX	i;
		int icount = 0;
		for( int z = 0 ; z < m_iVertexPerNode-1 ; z += iOffset )
		{
			for( int x = 0 ; x < m_iVertexPerNode-1 ; x += iOffset )
			{
				i._0 = (z*m_iVertexPerNode+x);
				i._1 = (z*m_iVertexPerNode+x+iOffset);
				i._2 = ((z+iOffset)*m_iVertexPerNode+x);
				*pI++ = i;
				i._0 = ((z+iOffset)*m_iVertexPerNode+x);
				i._1 = (z*m_iVertexPerNode+x+iOffset);
				i._2 = ((z+iOffset)*m_iVertexPerNode+x+iOffset);
				*pI++ = i;
				icount += 2;
			}
		}
		
		m_pIB[iLodlv]->Unlock();	

	}
	
	return true;
}
