#include "GdsTerrain.h"
#include "Resource\GdsResMgr.h"
#include "..\..\System\Logger\logger.h"
#include "Renderer\GdsRendererDX9.h"
#include "Renderer\GdsRenderStateGroup.h"

GdsTerrain::GdsTerrain() 
: m_iVertexPerPatch( 32 )
, m_pIB( NULL )
, m_ixheight( 0 )
, m_izheight( 0 )
, m_iMaxLOD( 6 )
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
	SAFE_DELETE( m_pPatch );
}

bool GdsTerrain::MakeHeightMap( GdsNodePtr pNode )
{
	if ( m_pPatch != NULL )
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

	int icheckx = cxHeight % m_iVertexPerPatch;
	int icheckz = czHeight % m_iVertexPerPatch;
	if ( icheckx != 0 || icheckz != 0 || cxHeight < m_iVertexPerPatch || czHeight < m_iVertexPerPatch )
	{
		LOG_ERROR_F( "Texture Size Error:[%d,%d]", cxHeight, czHeight );
		return false;
	}

	icheckx = cxHeight / m_iVertexPerPatch;
	icheckz = czHeight / m_iVertexPerPatch;
	if ( icheckx > 0 && icheckz > 0 )
	{
		m_ixheight = icheckx;
		m_izheight = icheckz;
		m_pPatch = new PATCH[icheckx*icheckz];
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
	for( DWORD z = 0 ; z < czHeight ; z++ )
	{
		for( DWORD x = 0 ; x < cxHeight ; x++ )
		{
			v.p.x = (float)x-cxHeight/2.0f;		/// 정점의 x좌표(메시를 원점에 정렬)
			v.p.z = -((float)z-czHeight/2.0f);	/// 정점의 z좌표(메시를 원점에 정렬), z축이 모니터안쪽이므로 -를 곱한다.
			v.p.y = ((float)(*((LPDWORD)d3drc.pBits+x+z*(d3drc.Pitch/4))&0x000000ff))/10.0f;	/// DWORD이므로 pitch/4
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize( &v.n, &v.n );
			v.t.x = (float)x / (cxHeight-1);
			v.t.y = (float)z / (czHeight-1);
			*pV++ = v;
		}
	}

	pVB->Unlock();
	texheight->Get()->UnlockRect( 0 );


	LPDIRECT3DINDEXBUFFER9 pIB;
	if( FAILED( RENDERER.GetDevice()->CreateIndexBuffer( (cxHeight-1)*(czHeight-1)*2 * sizeof(GDSINDEX), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIB, NULL ) ) )
	{
		return false;
	}

	GDSINDEX	i;
	GDSINDEX*	pI;

	if( FAILED( pIB->Lock( 0, (cxHeight-1)*(czHeight-1)*2 * sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
		return false;

	for( DWORD z = 0 ; z < czHeight-1 ; z++ )
	{
		for( DWORD x = 0 ; x < cxHeight-1 ; x++ )
		{
			i._0 = (z*cxHeight+x);
			i._1 = (z*cxHeight+x+1);
			i._2 = ((z+1)*cxHeight+x);
			*pI++ = i;
			i._0 = ((z+1)*cxHeight+x);
			i._1 = (z*cxHeight+x+1);
			i._2 = ((z+1)*cxHeight+x+1);
			*pI++ = i;
		}
	}
	pIB->Unlock();


	GdsRenderObjectPtr renderObject = GdsRenderObjectPtr( new GdsRenderObject );
	renderObject->SetVertexMaxCount( cxHeight*czHeight );
	renderObject->SetVertexSize( sizeof( GDSVERTEX ) );
	renderObject->SetFVF( GDSVERTEX::FVF );
	renderObject->SetIndexMaxCount( (cxHeight-1)*(czHeight-1)*2 );
	renderObject->SetIndexBuffer( pIB );
	renderObject->SetVertexBuffer( pVB );
	renderObject->SetStartIndex( 0 );
	renderObject->SetEndIndex( (cxHeight-1)*(czHeight-1)*2 );
	renderObject->SetStartVertexIndex( 0 );
	renderObject->SetEndVertexIndex( cxHeight*czHeight );
	renderObject->SetTexture( texcolor->Get() );
	D3DXMATRIX tm = pNode->GetLocalMatrix();
	renderObject->SetMatrix( tm );

	pNode->AddRenderObject( renderObject , 0 );	

	//pNode->CreateOctree();
	//	pNode->SetDrawOctreeNodeBoxLine( true );
	pNode->SetDrawAxis( true );
	pNode->SetDrawBox( true );
	pNode->SetUseLOD( true );
	//pNode->SetScale( 10.f );

	return true;
}

void GdsTerrain::createVB()
{

}

void GdsTerrain::createIB()
{

}