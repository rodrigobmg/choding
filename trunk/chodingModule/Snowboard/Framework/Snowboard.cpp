#include "Snowboard.h"

#include "../Core/CoreMgr.h"
#include "../Core/Renderer/RendererDX9.h"

CSnowboard::CSnowboard():m_pCoreMgr(NULL)
{

}

CSnowboard::~CSnowboard()
{

}
bool CSnowboard::InitModule()
{
	m_pCoreMgr = CCoreMgr::New();
	return true;
}

bool	CSnowboard::InitRenderer( HWND hWnd )
{
	if ( m_pCoreMgr == NULL )
	{
		assert( "�ھ� ������Ʈ" );
		return FALSE;
	}

	if ( m_pRendererDX9 != NULL )
	{
		assert( "���������̾���ϸ� ���Ҵ��ҽÿ��� �ھ�Ŵ������� �������� �Ҵ�޾ƾ��Ѵ�." );
		return FALSE;
	}
		
	m_pRendererDX9 = dynamic_cast< CRendererDX9*>( m_pCoreMgr->RegisterCore( L"Renderer" , new CRendererDX9 ) );

	return TRUE;
}

bool	CSnowboard::InitCamera()
{
	return TRUE;
}

bool	CSnowboard::InitResource()
{
	return TRUE;
}

void CSnowboard::Update()
{
	if ( m_pRendererDX9 )
		m_pRendererDX9->UpdateFrame();
}

void CSnowboard::DestroyModule()
{
	m_pCoreMgr->DestroyCore();
}