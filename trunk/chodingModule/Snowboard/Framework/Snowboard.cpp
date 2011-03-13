#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/Type/GdsResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "../System/Thread/GdsThreadPool.h"
#include "SceneNode/GdsMeshNode.h"
#include "SceneNode/GdsNode.h"
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
}

bool CSnowboard::InitModule( HWND hWnd )
{
	util::Logger::createSingleton();
	util::Logger::getInstance().Init(NULL , NULL , NULL , NULL );

	GdsThreadPool::createSingleton();
	GdsThreadPool::getInstance().Create( 0 );


	InitRenderer( hWnd );
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRenderer->vCreate( hWnd );
	m_pRenderer->GetRootNode()->SetDevice( m_pRenderer->GetDevice() );
	
	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	m_pResMgr->vCreate( device );
	return TRUE;
}

void CSnowboard::DestroyModule()
{
	util::Logger::destroySingleton();
	GdsThreadPool::destroySingleton();

	if ( m_pRenderer )
		m_pRenderer->vRelease();
	if ( m_pResMgr )
		m_pResMgr->vRelease();	
}


void CSnowboard::OnIdle()
{
	if ( m_pRenderer )
	{		
		BEGIN_PERFORMANCE( L"render" );
		m_pRenderer->Update( 0.f );
		END_PERFORMANCE( L"render" );
		SAMPLE_PERFORMANCE sample;
		OUTPUT_PERFORMANCE( L"render" , sample );
		LOG_WARNING_F(" [Render] Avg = %d , DeltaTick = %d , Count = %d", sample.ulAvg , sample.ulTotalDeltaTick , sample.ulCount );
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
  		GdsThreadPool::getInstance().GetBGThread()->Push< GdsResMgr::LOADLIST_WORK_TOKEN >(
  														m_pResMgr.get() ,
  														GdsResMgr::LOADLIST_WORK_TOKEN( L"test" , respath , L"tga;bmp;dds" , true ) , 
  														&GdsResMgr::CreateList );
		
		GdsThreadPool::getInstance().GetBGThread()->Push< const TCHAR* >( m_pResMgr.get() , L"test"  , &GdsResMgr::LoadRes );
	
		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( L"md2" , respath , L"md2" , true ) );
		m_pResMgr->LoadRes( L"md2" );
	}

	GdsResMD2Ptr resmesh = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2" , L"meat.md2" ) );
	if( resmesh )
	{
		for ( size_t i=0 ; i < 1 ; i++)
		{
			GdsMeshNodePtr mesh = GdsMeshNodePtr( new GdsMeshNode );

			mesh->SetResource( resmesh );

			m_pRenderer->GetRootNode()->AttachChild( mesh );
		}		
	}	
}
