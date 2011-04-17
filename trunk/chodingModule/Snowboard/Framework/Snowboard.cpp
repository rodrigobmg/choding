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
	
	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	m_pResMgr->Create( device );
	return TRUE;
}

void CSnowboard::DestroyModule()
{
	if ( m_pRenderer )
		m_pRenderer->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();	
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

	if ( m_pResMgr )
	{	 
 		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"ase;bmp;dds;tga" , true ) );
	}

	GdsResASEPtr resASE	= boost::shared_dynamic_cast< GdsResASE >( m_pResMgr->Get( L"ase\\woman.ase" ) );

	GdsNodePtr node = resASE->GetNode();
	node->SetDrawAxis( true );
//	m_pRenderer->GetRootNode()->AttachChild( node );

	RENDERER.GetRootNode()->SetDrawAxis( true );

	{
		GdsNodePtr mesh = GdsNodePtr( new GdsNode );		
		
		mesh->SetDrawAxis( true );

		mesh->SetTranslate( 10 , 2 , 0 );

		RENDERER.GetRootNode()->AttachChild( mesh );
		
		GdsNodePtr billboard = GdsNodePtr( new GdsNode );
		billboard->SetBillboard( true );
		billboard->SetDrawAxis( true );
		billboard->SetTranslate( -20 , 0 , 0 );
		mesh->AttachChild( billboard );

	}	
}

HRESULT CSnowboard::MsgProc()
{
	return true;
}

GdsNodePtr CSnowboard::MakeHeightMap()
{
	GdsResTexturePtr texheight = boost::shared_dynamic_cast< GdsResTexture >( m_pResMgr->Get( L"tex\\map128.bmp") );
	GdsResTexturePtr texcolor = boost::shared_dynamic_cast< GdsResTexture >( m_pResMgr->Get( L"tex\\tile2.tga") );

	GdsNodePtr mapNode = GdsNodePtr( new GdsNode );
	return mapNode;
}
