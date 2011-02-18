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
	m_pRendererDX9	= NULL;
	m_pResMgr		= NULL;
}

bool CSnowboard::InitModule( HWND hWnd )
{
	InitRenderer( hWnd );
	InitCamera();
	InitResource( m_pRendererDX9->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRendererDX9 = dynamic_cast< CRendererDX9* >( CCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRendererDX9->Create( hWnd );
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
	if ( m_pRendererDX9 )
		m_pRendererDX9->UpdateFrame();
}

void CSnowboard::DestroyModule()
{
	if ( m_pRendererDX9 )
		m_pRendererDX9->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();
}

void CSnowboard::TestFunc()
{
	util::Logger::createSingleton();
	util::Logger::getInstance().Init(NULL , NULL , NULL , NULL );

	BGThread* pThread = BGThread::New();
	pThread->CreateAndRunThread();

	LOG_WARNING_F( L"%s", L"logger init" );
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	_stprintf_s( respath , L"%s\\%s" , L"D:\\Project\\Client\\Trunk\\WorkGroup\\Client" , L"Application" );

	if ( m_pResMgr )
	{		
		SAMPLE_PERFORMANCE loadsample;
		SAMPLE_PERFORMANCE ressample;
		BEGIN_PERFORMANCE( L"list" );
		if ( m_pResMgr->CreateList( L"test" , respath , L"tga;dds;" , 1 ) )
		{
			END_PERFORMANCE( L"list" );
			BEGIN_PERFORMANCE(L"res" );
			pThread->Push( /*m_pResMgr , &CResMrg::Clear*/ ) ;
			//m_pResMgr->LoadRes( L"test" );
			END_PERFORMANCE(L"res" );
			//m_pResMgr->ReleaseRes( L"test" );
		}

		OUTPUT_PERFORMANCE( L"list" , loadsample );
		OUTPUT_PERFORMANCE( L"res" , ressample );
		LOG_ERROR_F( "list avg tick = %d" , loadsample.ulAvg );
		LOG_ERROR_F( "res  avg tick = %d" , ressample.ulAvg );

		CResTexture* p = static_cast< CResTexture*>( m_pResMgr->Get( L"test" , L"banana.bmp" ) );
		//할당을 받으면 꼭 릴리즈해서 반환을 한다.
		if ( p )
			p->Release();		
	}
}