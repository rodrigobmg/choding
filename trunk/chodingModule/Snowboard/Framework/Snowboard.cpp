#include "Snowboard.h"

#include "../Core/CoreMgr.h"
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
	m_pCoreMgr		= NULL;
	m_pRendererDX9	= NULL;
	m_pResMgr		= NULL;
}

bool CSnowboard::InitModule()
{
	m_pCoreMgr = CCoreMgr::New();
	if ( m_pCoreMgr == NULL )
	{
		assert("할당실패");
		return false;
	}
	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	if ( m_pCoreMgr == NULL )
	{
		assert( "코어 널포인트" );
		return FALSE;
	}

	if ( m_pCoreMgr->GetCore( CORE_RENDERER ) != NULL )
	{
		assert( "널포인터이어야하며 재할당할시에는 코어매니져에서 해제한후 할당받아야한다." );
		return FALSE;
	}
		
	m_pRendererDX9 = dynamic_cast< CRendererDX9* >( m_pCoreMgr->RegisterCore( CORE_RENDERER , new CRendererDX9 ) );
	if ( m_pRendererDX9 )
	{
		if ( FAILED( m_pRendererDX9->Create( hWnd ) ) )
		{
			return FALSE;
		}		
	}

	return TRUE;
}

bool	CSnowboard::InitCamera()
{
	return TRUE;
}

bool	CSnowboard::InitResource()
{
	if ( m_pCoreMgr == NULL )
	{
		assert( "코어 널포인트" );
		return FALSE;
	}

	if ( m_pCoreMgr->GetCore( CORE_RESOURCE ) != NULL )
	{
		assert( "널포인터이어야하며 재할당할시에는 코어매니져에서 해제한후 할당받아야한다." );
		return FALSE;
	}

	m_pResMgr = dynamic_cast< CResMrg* >( m_pCoreMgr->RegisterCore( CORE_RESOURCE , new CResMrg ) );
	if ( m_pResMgr )
		m_pResMgr->Create();

	TestFunc();

	return TRUE;
}

void CSnowboard::Update()
{
	if ( m_pRendererDX9 )
		m_pRendererDX9->UpdateFrame();
}

void CSnowboard::DestroyModule()
{
	if ( m_pCoreMgr )
		m_pCoreMgr->Destroy();
}

void CSnowboard::TestFunc()
{
	if ( m_pResMgr )
		m_pResMgr->Load( L"banana.bmp" , m_pRendererDX9->GetDevice() );

	CResTexture* pTexture = dynamic_cast<CResTexture*>( m_pResMgr->Get( L"banana.bmp" ) );
}