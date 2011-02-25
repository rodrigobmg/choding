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

}

void CSnowboard::Clear()
{
}

bool CSnowboard::InitModule( HWND hWnd )
{
	InitRenderer( hWnd );
	InitCamera();
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = GdsRendererDX9Ptr( dynamic_cast< GdsRendererDX9* >( GdsCoreFactory::CreateCore( CORE_RENDERER ) ) );
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
	m_pResMgr = GdsResMgrPtr( dynamic_cast< GdsResMgr* >( GdsCoreFactory::CreateCore( CORE_RESOURCE ) ) );
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
}

void CSnowboard::TestFunc()
{
	util::Logger::createSingleton();
	util::Logger::getInstance().Init(NULL , NULL , NULL , NULL );

	GdsThreadPool::createSingleton();
	GdsThreadPool::getInstance().Create( 10 );


	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , curpath , L"Resource" );

	if ( m_pResMgr )
	{		
		SAMPLE_PERFORMANCE loadsample;
		BEGIN_PERFORMANCE( L"list" );
		if ( m_pResMgr->CreateList( L"test" , respath , L"tga;bmp;dds" , 1 ) )
		{
			END_PERFORMANCE( L"list" );
			//GdsThreadPool::getInstance().GetIdleThread()->Push< const TCHAR* >( (GdsResMgr*&)m_pResMgr , L"test"  , &GdsResMgr::LoadRes );
			m_pResMgr->LoadRes( L"test" );
		}

		GdsResTexturePtr p = boost::shared_dynamic_cast< GdsResTexture >( m_pResMgr->Get( L"test" , L"banana.bmp" ) );

		OUTPUT_PERFORMANCE( L"list" , loadsample );
		LOG_ERROR_F( "list avg tick = %d" , loadsample.ulAvg );		
		//할당을 받으면 꼭 릴리즈해서 반환을 한다.
	}
}