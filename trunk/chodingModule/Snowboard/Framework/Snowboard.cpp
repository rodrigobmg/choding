#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"

#include "../System/Logger/logger.h"
#include "../System/PerformanceCheck/Performance.h"

#include "SceneNode/GdsNode.h"
#include "../System/Time/GdsSystemTime.h"
#include "../System/FrameMemory/GdsFrameMemory.h"
#include "Resource/Type/GdsResASE.h"

CSnowboard::CSnowboard()
{
	Clear(); 
}

CSnowboard::~CSnowboard()
{
	DestroyModule();
}

void CSnowboard::Clear()
{
	m_fFrameRate = 0.0f;
}

bool CSnowboard::InitModule( HWND hWnd )
{
	LOGGER.Init(NULL , NULL , NULL , NULL );
	FRAMEMEMORY.Init( 1024 * 1024 );
	LOG_WARNING_F( "Init FrameMemory = %d Byte" , FRAMEMEMORY.GetSize() );

	InitRenderer( hWnd );
	InitResource( RENDERER.GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	//m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	RENDERER.Create( hWnd );
	//m_pRenderer->Create( hWnd );
	
	GdsRenderStateGroupPtr renderstate = GdsRenderStateGroupPtr( new GdsRenderStateGroup );
	renderstate->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );	/// 0번 텍스처 스테이지의 확대 필터

	renderstate->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		/// 0번 텍스처 : 0번 텍스처 인덱스 사용
	renderstate->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	renderstate->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	renderstate->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	renderstate->SetRenderState( D3DRS_ZENABLE, TRUE );
	renderstate->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	renderstate->SetRenderState( D3DRS_LIGHTING, FALSE );
	renderstate->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	RENDERER.GetRenderFrame()->AddRenderStateGroup( renderstate , 0 );

	GDS::SetMaxFrameRate( 0 );

	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	//m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	RESMGR.Create( device );
	return TRUE;
}

void CSnowboard::DestroyModule()
{
// 	if ( m_pRenderer )
// 		m_pRenderer->Release();
// 	if ( m_pResMgr )
// 		m_pResMgr->Release();	
}


void CSnowboard::OnIdle()
{
	if ( !GDS::MeasureTime() )
	{
		return;
	}

// 	if ( m_pRenderer )
// 	{		
// 		m_pRenderer->Update( GDS::GetAccumTime() );
// 		m_fFrameRate = 1.0f / GDS::GetFrameTime();
// 	}
	RENDERER.Update( GDS::GetAccumTime() );
	m_fFrameRate = 1.0f / GDS::GetFrameTime();
}


void CSnowboard::TestFunc()
{	
	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource");

	RESMGR.CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"ase;bmp;dds;tga;jpg" , true ) );

	//GdsResASEPtr resASE	= boost::shared_dynamic_cast< GdsResASE >( RESMGR.Get( L"woman.ase" ) );

	//GdsNodePtr node = resASE->GetNode();
//	GdsNodePtr node = GdsNodePtr( new GdsNode );
	//node->SetName( L"Child" );
	//RENDERER.GetRootNode()->AttachChild( node );

	//node->SetTranslate( 11.0 , 0, 0  );

	RENDERER.GetRootNode()->SetName( L"ROOT" );

	{
// 		GdsNodePtr mesh = GdsNodePtr( new GdsNode );		
// 		
// 		mesh->SetDrawAxis( true );
// 
// 		mesh->SetTranslate( 10 , 2 , 0 );

	//	RENDERER.GetRootNode()->AttachChild( node );
		
	}

	GdsNodePtr mapNode = GdsNodePtr( new GdsNode );
	mapNode->SetName( L"height Map" );
	MakeHeightMap( mapNode );
	RENDERER.GetRootNode()->AttachChild( mapNode );

}

HRESULT CSnowboard::MsgProc()
{
	return true;
}

void CSnowboard::MakeHeightMap( GdsNodePtr pNode )
{
	GdsResTexturePtr texheight = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"map128.bmp") );
	GdsResTexturePtr texcolor = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( L"tile2.tga") );

	D3DSURFACE_DESC		ddsd;
	D3DLOCKED_RECT		d3drc;
	int g_cxHeight , g_czHeight;

	texheight->Get()->GetLevelDesc( 0, &ddsd );	/// 텍스처의 정보
	g_cxHeight = ddsd.Width;				/// 텍스처의 가로크기
	g_czHeight = ddsd.Height;				/// 텍스처의 세로크기

	LPDIRECT3DVERTEXBUFFER9 g_pVB;
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

	g_pVB->Unlock();
	texheight->Get()->UnlockRect( 0 );


	LPDIRECT3DINDEXBUFFER9 g_pIB;

	if( FAILED( RENDERER.GetDevice()->CreateIndexBuffer( (g_cxHeight-1)*(g_czHeight-1)*2 * sizeof(GDSINDEX), 0, 
														D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL ) ) )
	{
		return;
	}

	GDSINDEX		i;
	GDSINDEX*	pI;
	
	if( FAILED( g_pIB->Lock( 0, (g_cxHeight-1)*(g_czHeight-1)*2 * sizeof(GDSINDEX), (void**)&pI, 0 ) ) )
		return;

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


	GdsRenderObjectPtr renderObject = GdsRenderObjectPtr( new GdsRenderObject );
	renderObject->SetVertexMaxCount( g_cxHeight*g_czHeight );
	renderObject->SetVertexSize( sizeof( GDSVERTEX ) );
	renderObject->SetFVF( GDSVERTEX::FVF );
	renderObject->SetIndexMaxCount( (g_cxHeight-1)*(g_czHeight-1)*2 );
	renderObject->SetIndexBuffer( g_pIB );
	renderObject->SetVertexBuffer( g_pVB );
	renderObject->SetStartIndex( 0 );
	renderObject->SetEndIndex( (g_cxHeight-1)*(g_czHeight-1)*2 );
	renderObject->SetStartVertexIndex( 0 );
	renderObject->SetEndVertexIndex( g_cxHeight*g_czHeight );
	renderObject->SetTexture( texcolor->Get() );
	D3DXMATRIX tm = pNode->GetLocalMatrix();
	renderObject->SetMatrix( tm );

	pNode->AddRenderObject( renderObject , 0 );	

	pNode->CreateOctree();
//	pNode->SetDrawOctreeNodeBoxLine( true );
}
