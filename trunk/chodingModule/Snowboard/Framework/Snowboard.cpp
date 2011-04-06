#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"

#include "../System/Logger/logger.h"
#include "../System/PerformanceCheck/Performance.h"

#include "SceneNode/GdsNode.h"
#include "SceneNode/GdsBillboardNode.h"
#include "../System/Time/GdsSystemTime.h"
#include "../System/FrameMemory/GdsFrameMemory.h"
#include "Resource/Type/GdsResMD2.h"

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
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRenderer->Create( hWnd );
	m_pRenderer->GetRootNode()->SetDevice( m_pRenderer->GetDevice() );
	
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

	if ( m_pRenderer )
	{		
		m_pRenderer->Update( GDS::GetAccumTime() );
		m_fFrameRate = 1.0f / GDS::GetFrameTime();
	}
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
 		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( respath , L"md2;ase;bmp" , true ) );
	}

	GdsResMD2Ptr resmesh = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2\\나무02\\tree2.md2" ) );
	GdsResMD2Ptr resmesh1 = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2\\나무02\\tree2.md2" ) );

	m_pRenderer->GetRootNode()->SetDrawAxis( true );

	if( resmesh )
	{
		GdsNodePtr mesh = GdsNodePtr( new GdsNode );		
		
		mesh->SetPropertyState( resmesh->GetPropertyState() );
		mesh->SetDrawAxis( true );
		//mesh->SetScale( 0.2f );
		mesh->SetTranslate( 10 , 2 , 0 );
		//mesh->SetCullType( GdsNode::CULL_ON );
		m_pRenderer->GetRootNode()->AttachChild( mesh );

		GdsBillboardNodePtr billboard = GdsBillboardNodePtr( new GdsBillboardNode );
		billboard->SetDrawAxis( true );
		billboard->SetTranslate( -20 , 0 , 0 );
		mesh->AttachChild( billboard );
// 		GdsNodePtr mesh1 = GdsNodePtr( new GdsNode );		
// 		mesh1->SetPropertyState( pProperty );		
// 		mesh1->SetTranslate( -1 , 0 , -10 );
// 		mesh1->SetCullType( GdsNode::CULL_ON );
// 		mesh1->SetDrawAxis( true );
// 		mesh->AttachChild( mesh1 );
	}	
}

HRESULT CSnowboard::MsgProc()
{
	return true;
}