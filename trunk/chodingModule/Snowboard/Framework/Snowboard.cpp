#include "Snowboard.h"

#include "../Core/CoreFactory.h"
#include "../Core/Renderer/RendererDX9.h"
#include "../Core/Resource/Type/ResTexture.h"

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
	m_pResMgr = dynamic_cast< CResMrg* >( CCoreFactory::CreateCore( CORE_RESOURCE ) );
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
	TCHAR curpath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, curpath );
	TCHAR respath[MAX_PATH];
	wsprintf( respath , L"%s\\%s" , curpath , L"Resource" );

	if ( m_pResMgr )
	{
		m_pResMgr->CreateList( L"test" , respath , L"bmp;tga;jpg;" , 1 );
		m_pResMgr->LoadRes( L"test" );

		m_pResMgr->CreateList( L"tex" , respath , L"" , 1 );
		m_pResMgr->LoadRes( L"tex" );

		//m_pResMgr->ReleaseRes( L"test" );
		m_pResMgr->Release();
	}
}