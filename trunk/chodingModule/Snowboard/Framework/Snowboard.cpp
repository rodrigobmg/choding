#include "Snowboard.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/Type/GdsResTexture.h"

#include "../Utility/Log/logger.h"
#include "../Utility/PerformanceCheck/Performance.h"

#include "../System/Thread/GdsThreadPool.h"
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
 		m_pResMgr->CreateList( GdsResMgr::LOADLIST_WORK_TOKEN( L"md2" , respath , L"md2" , true ) );
		m_pResMgr->LoadRes( L"md2" );
	}

	GdsResMD2Ptr resmesh = boost::shared_dynamic_cast< GdsResMD2 >( m_pResMgr->Get( L"md2" , L"meat.md2" ) );
	if( resmesh )
	{
		GdsNodePtr mesh = GdsNodePtr( new GdsNode );		
		GdsPropertyStatePtr pProperty = GdsPropertyStatePtr( new GdsPropertyState );
		pProperty->GetPolygonProperty()->SetVB(resmesh->GetVertexBufferPtr() );
		pProperty->GetPolygonProperty()->SetFVF( resmesh->GetFVF() );
		pProperty->GetPolygonProperty()->SetPrimitive( resmesh->GetPrimitive() );
		pProperty->GetPolygonProperty()->SetVertexFormatSize( resmesh->GetVertexSize() );
		pProperty->GetTextureProperty()->SetTexture( resmesh->GetTexturePtr() );
		mesh->SetPropertyState( pProperty );
		
		m_pRenderer->GetRootNode()->AttachChild( mesh );

		GdsNodePtr mesh1 = GdsNodePtr( new GdsNode );
		mesh1->SetPropertyState( pProperty );
		mesh1->SetTranslate( -10 , 0 , 50 );
		m_pRenderer->GetRootNode()->AttachChild( mesh1 );

	}	
}
