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
	m_pRendererDX9	= NULL;
	m_pResMgr		= NULL;
}

bool CSnowboard::InitModule()
{
	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	m_pRendererDX9 = dynamic_cast< CRendererDX9* >( CCoreFactory::CreateCore( CORE_RENDERER ) );

	return TRUE;
}

bool	CSnowboard::InitCamera()
{
	return TRUE;
}

bool	CSnowboard::InitResource()
{
	m_pResMgr = dynamic_cast< CResMrg* >( CCoreFactory::CreateCore( CORE_RESOURCE ) );

	return TRUE;
}

void CSnowboard::Update()
{
	if ( m_pRendererDX9 )
		m_pRendererDX9->UpdateFrame();
}

void CSnowboard::DestroyModule()
{
}



void CSnowboard::TestFunc()
{
	if ( m_pResMgr )
		m_pResMgr->Load( L"banana.bmp" , m_pRendererDX9->GetDevice() );

	CResTexture* pTexture = dynamic_cast<CResTexture*>( m_pResMgr->Get( L"banana.bmp" ) );
}