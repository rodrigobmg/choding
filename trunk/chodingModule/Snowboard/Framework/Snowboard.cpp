#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/Type/GdsResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "../System/Thread/GdsThreadPool.h"
#include "SceneNode/GdsMeshNode.h"
#include "SceneNode/GdsNode.h"

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
	InitCamera( m_pRenderer->GetDevice() );
	InitResource( m_pRenderer->GetDevice() );

	TestFunc();

	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRenderer = boost::shared_dynamic_cast< GdsRendererDX9 >( GdsCoreFactory::CreateCore( CORE_RENDERER ) );
	m_pRenderer->Create( hWnd );
	
	return TRUE;
}

bool	CSnowboard::InitCamera( LPDIRECT3DDEVICE9 device )
{
	m_pCamMgr = boost::shared_dynamic_cast< GdsCameraManagerDX9 >( GdsCoreFactory::CreateCore( CORE_CAMMGR_DX9 ));
	m_pCamMgr->Create( device );
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
	util::Logger::destroySingleton();
	GdsThreadPool::destroySingleton();

	if ( m_pRenderer )
		m_pRenderer->Release();
	if ( m_pResMgr )
		m_pResMgr->Release();	
}


void CSnowboard::OnIdle()
{
	if ( m_pRenderer )
		m_pRenderer->Update( 0.f );

	if ( m_pRenderer )
		m_pRenderer->Render( 0.f );

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
}
