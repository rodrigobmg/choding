#include "Snowboard.h"

#include "../Core/CoreFactory.h"
#include "../Core/Renderer/RendererDX9.h"
#include "../Core/Resource/Type/ResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "ThreadPool/ThreadPool.h"

CSnowboard::CSnowboard()
{
	Clear();
}

CSnowboard::~CSnowboard()
{

}

void CSnowboard::Clear()
{
	m_pRendererBase	= NULL;
	m_pResMgr		= NULL;
}

bool CSnowboard::InitModule( HWND hWnd )
{
	InitRenderer( hWnd );
	InitCamera();
	InitResource( m_pRendererBase->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRendererBase = dynamic_cast< CRendererDX9* >( CCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRendererBase->Create( hWnd );
	return TRUE;
}

bool	CSnowboard::InitCamera()
{
	return TRUE;
}

bool	CSnowboard::InitResource( LPDIRECT3DDEVICE9 device )
{
	m_pResMgr = static_cast< CResMrg* >( CCoreFactory::CreateCore( CORE_RESOURCE ) );
	m_pResMgr->Create( device );
	return TRUE;
}

void CSnowboard::Update()
{
	if ( m_pRendererBase )
		m_pRendererBase->UpdateFrame();
}

void CSnowboard::DestroyModule()
{
	if ( m_pRendererBase )
		m_pRendererBase->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();
}

void CSnowboard::TestFunc()
{
	util::Logger::createSingleton();
	util::Logger::getInstance().Init(NULL , NULL , NULL , NULL );

	ThreadPool::createSingleton();
	ThreadPool::getInstance().Create( 10 );


	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , L"D:\\Project\\Client\\Trunk\\WorkGroup\\Client\\Application" , L"" );

	if ( m_pResMgr )
	{		
		SAMPLE_PERFORMANCE loadsample;
		BEGIN_PERFORMANCE( L"list" );
		if ( m_pResMgr->CreateList( L"test" , respath , L"tga;bmp;dds" , 1 ) )
		{
			END_PERFORMANCE( L"list" );
			ThreadPool::getInstance().GetIdleThread()->Push< const TCHAR* >( m_pResMgr , L"test"  , &CResMrg::LoadRes );
		}

		OUTPUT_PERFORMANCE( L"list" , loadsample );
		LOG_ERROR_F( "list avg tick = %d" , loadsample.ulAvg );

		CResTexture* p = static_cast< CResTexture*>( m_pResMgr->Get( L"test" , L"banana.bmp" ) );
		//할당을 받으면 꼭 릴리즈해서 반환을 한다.
		if ( p )
			p->Release();		
	}
}