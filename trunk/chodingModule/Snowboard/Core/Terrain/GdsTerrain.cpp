#include "GdsTerrain.h"
#include "Resource\GdsResMgr.h"
#include "..\..\System\Logger\logger.h"
#include "Renderer\GdsRendererDX9.h"
#include "Renderer\GdsRenderStateGroup.h"
#include "GdsTerrain_Triangle.h"


GdsTerrain::GdsTerrain() 
: m_iVertexPerNode( 33 )
, m_ixheight( 0 )
, m_izheight( 0 )
, m_iMaxLOD( 0 )
, m_pRootTile(NULL)
, m_iLodRate( 50 )
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
		tile->m_pChild[0]->m_pParent = tile;
		tile->m_pChild[0]->m_minPos = tile->m_minPos;
		tile->m_pChild[0]->m_maxPos = tile->m_cenPos;		

		tile->m_pChild[1] = new TILE;
		tile->m_pChild[1]->m_pParent = tile;
		tile->m_pChild[1]->m_minPos = D3DXVECTOR3( tile->m_cenPos.x , 0 , tile->m_minPos.z );
		tile->m_pChild[1]->m_maxPos = D3DXVECTOR3( tile->m_maxPos.x , 0 , tile->m_cenPos.z );

		tile->m_pChild[2] = new TILE;
		tile->m_pChild[2]->m_pParent = tile;
		tile->m_pChild[2]->m_minPos = tile->m_cenPos;
		tile->m_pChild[2]->m_maxPos = tile->m_maxPos;

		tile->m_pChild[3] = new TILE;
		tile->m_pChild[3]->m_pParent = tile;
		tile->m_pChild[3]->m_minPos = D3DXVECTOR3( tile->m_minPos.x , 0 , tile->m_cenPos.z );
		tile->m_pChild[3]->m_maxPos = D3DXVECTOR3( tile->m_cenPos.x , 0 , tile->m_maxPos.z );

		build( tile->m_pChild[0] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[1] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[2] , (GDSVERTEX*)pVB );
		build( tile->m_pChild[3] , (GDSVERTEX*)pVB );
	}
	else
	{
		tile->m_pVB		= GdsVertexBufferPtr( new GdsVertexBuffer );
		float minY = 0;
		float maxY = 0;
		int istartx = static_cast<int>(tile->m_minPos.x);
		int istartz = static_cast<int>(tile->m_minPos.z);

		for ( int z = istartz ; z < istartz + m_iVertexPerNode ; z++ )
		{
			for ( int x = istartx ; x < istartx + m_iVertexPerNode ; x++ )
			{
				tile->m_pVB->AddVertex( pVB[ z*m_ixheight + x] );
				if ( pVB[ z*m_ixheight + x].p.y > maxY )
					maxY = pVB[ z*m_ixheight + x].p.y;

				if ( pVB[ z*m_ixheight + x].p.y < minY )
					minY = pVB[ z*m_ixheight + x].p.y;			
			}		
		}		
		tile->m_minPos.y = minY;
		tile->m_maxPos.y = maxY;
	}
}

bool GdsTerrain::MakeHeightMap()
{
	if ( m_pRootTile != NULL )
	{
		ASSERT( 0 && "중복 로딩" );
		return false;
	}

	GdsResTexturePtr texheight = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"map.jpg") );
	GdsResTexturePtr texcolor = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"tile2.tga") );

	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;
	int cxHeight , czHeight;

	texheight->Get()->GetLevelDesc( 0, &ddsd );	/// 텍스처의 정보
	cxHeight = ddsd.Width;				/// 텍스처의 가로크기
	czHeight = ddsd.Height;				/// 텍스처의 세로크기

	//maxlod 계산
	for ( int i= (m_iVertexPerNode-1) ; i > 0 ; i /= 2 )
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
		m_ixheight = cxHeight+1;
		m_izheight = czHeight+1;
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
	GDSVERTEX* v = new GDSVERTEX[m_ixheight*m_izheight];
	for( int z = 0 ; z < czHeight+1 ; z++ )
	{
		for( int x = 0 ; x < cxHeight+1 ; x++ )
		{
			if ( z == czHeight )
			{
				vertex = v[(z-1)*m_ixheight+x];
				vertex.p.z -= 1;

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

				v[z*m_ixheight+x] = vertex;
			}
			else if ( x == cxHeight )
			{
				vertex = v[z*m_ixheight+(x-1)];
				vertex.p.x += 1;

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

				v[z*m_ixheight+x] = vertex;
			}			
			else
			{
				vertex.p.x = (float)x;
				vertex.p.z = (float)z;//-( (float)(z-1) - ( cxHeight - 1) );
				vertex.p.y = ((float)(*( (LPDWORD)d3drc.pBits+x+( czHeight - z)*(d3drc.Pitch/4) )&0x000000ff) ) / 10.0f;	/// DWORD이므로 pitch/4
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

				v[z*m_ixheight+x] = vertex;
			}			
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

	if ( tile->m_pVB->Get() == NULL )
		return;

	if ( CAMMGR.GetCurCam()->GetFrustum().VertexIsInFrustum( tile->m_cenPos ) == false )
		return;

	D3DXVECTOR3 eyepos = CAMMGR.GetCurCam()->GetEye();	
	D3DXVECTOR3 dir = eyepos - tile->m_cenPos;
	float fdist = D3DXVec3Length( &dir ); 
	int iLodlv = 0;
	for (int i=0; i<m_iMaxLOD; i++)
	{
		iLodlv++;
		if ( i*m_iLodRate > fdist )
		{			
			break;
		}
	}
}

void GdsTerrain::Update( float fElapsedtime )
{
	if( m_pRootTile )
		genIndex( m_pRootTile );
}

bool GdsTerrain::createTempletIB()
{	
	TRIANGLE* pRootTri1 = new TRIANGLE;	
	D3DXVECTOR3 minPos( 0,0,0 );
	D3DXVECTOR3 maxPos( (float)m_ixheight-1 , 0 , (float)m_izheight-1 );
	int iLodLimit = 4;

	pRootTri1->m_pLeft = new TRIANGLE;
	pRootTri1->m_pLeft->p1 = minPos;
	pRootTri1->m_pLeft->corner = D3DXVECTOR3( (float)m_ixheight-1 ,0, 0 );
	pRootTri1->m_pLeft->p2 = maxPos;

	pRootTri1->m_pRight = new TRIANGLE;
	pRootTri1->m_pRight->p1 = maxPos;
	pRootTri1->m_pRight->corner = D3DXVECTOR3( 0, 0, (float)m_izheight-1 );
	pRootTri1->m_pRight->p2 = minPos;

	pRootTri1->split( pRootTri1->m_pLeft , pRootTri1->m_iLodlv , iLodLimit );
	pRootTri1->split( pRootTri1->m_pRight , pRootTri1->m_iLodlv , iLodLimit );	
	
	GdsIndexBufferPtr pIndexBuffer = GdsIndexBufferPtr( new GdsIndexBuffer );
	
	pRootTri1->genIndexTemplet( pIndexBuffer , 3 , TRIANGLE::SOUTH );
	pIndexBuffer->Alloc();

	SAFE_DELETE( pRootTri1 );
	return true;
}
