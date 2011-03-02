#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/Type/GdsResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "ThreadPool/GdsThreadPool.h"

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
	GdsThreadPool::getInstance().Create( 10 );


	InitRenderer( hWnd );
	InitCamera();
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRenderer->Create( hWnd );

	m_pRootNode	= GdsNodePtr( new GdsNode );

	return TRUE;
}

bool	CSnowboard::InitCamera()
{
	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	m_pResMgr = boost::shared_dynamic_cast< GdsResMgr >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) );
	m_pResMgr->Create( device );
	return TRUE;
}

void CSnowboard::Update()
{
	if ( m_pRenderer )
		m_pRenderer->Render( m_pRootNode );
}

void CSnowboard::DestroyModule()
{
	if ( m_pRenderer )
		m_pRenderer->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();

	util::Logger::destroySingleton();
	GdsThreadPool::destroySingleton();
}

void CSnowboard::TestFunc()
{	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource" );

	if ( m_pResMgr )
	{	

		SAMPLE_PERFORMANCE loadsample;
		BEGIN_PERFORMANCE( L"list" );
		GdsThreadPool::getInstance().GetBGThread()->Push< GdsResMgr::LOADLIST_WORK_TOKEN >(
														(GdsResMgr*&)m_pResMgr ,
														GdsResMgr::LOADLIST_WORK_TOKEN( L"test" , respath , L"tga;bmp;dds" , true ) , 
														&GdsResMgr::CreateList );
		
		END_PERFORMANCE( L"list" );
		GdsThreadPool::getInstance().GetBGThread()->Push< const TCHAR* >( (GdsResMgr*&)m_pResMgr , L"test"  , &GdsResMgr::LoadRes );

		OUTPUT_PERFORMANCE( L"list" , loadsample );
		LOG_ERROR_F( "list avg tick = %d" , loadsample.ulAvg );		
		
	}
}