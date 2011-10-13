#include "stdafx.h"

GdsRendererManager::GdsRendererManager()
:m_pRenderer(NULL)
,m_IsCurRendering(true)
,m_ProcessTick(0)
,m_CountRenderObject(0)
,m_bUseThreadRender(true)
{

}

GdsRendererManager::~GdsRendererManager()
{
	SAFE_DELETE( m_pRenderer );
}

void GdsRendererManager::SetRenderingEnable( bool flag )
{
	m_IsCurRendering = flag;
}

bool GdsRendererManager::EnableRendering()
{
	return m_IsCurRendering;
}

bool GdsRendererManager::Render( float fAccumtime )
{
	if ( m_IsCurRendering == false )
	{
		//렌더링 중이므로 백버퍼 초기화
		m_pRenderer->GetRenderFrame()->ClearBackFrameBuffer();
		return false;
	}
	
	SetRenderingEnable( false );

	GdsBGThread* thread = THREADPOOL.GetThread( 0 );
	if ( thread == NULL )
		return false;

// 	if ( INPUTSYSTEM.GetKeyIsDown( VK_P ) )
// 		m_pRenderer->ToggleWireMode();
// 
// 	if ( INPUTSYSTEM.GetKeyIsDown( VK_M ) )
// 		m_bUseThreadRender = !m_bUseThreadRender;

	m_pRenderer->GetRenderFrame()->Swap_buffer();
	if ( m_bUseThreadRender )
	{//쓰레드 렌더링
		m_ProcessTick = thread->GetProcessTick();
		m_CountRenderObject = m_pRenderer->GetRenderFrame()->GetRenderObjectCount();		
		thread->Push( m_pRenderer , fAccumtime , &GdsRendererBase::Render );
	}
	else
	{
		m_pRenderer->Render( fAccumtime );	
	}

	return true;
}

bool GdsRendererManager::Create( HWND hwnd )
{
	if ( m_pRenderer != NULL )
		return false;

	m_pRenderer = new GdsRendererDX9;
	if ( m_pRenderer == NULL )
		return false;

	return m_pRenderer->Create( hwnd );
}

void GdsRendererManager::DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point )
{
	if ( m_pRenderer )
		m_pRenderer->DrawAxis( mat , point );
}

void GdsRendererManager::DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos )
{
	if( m_pRenderer )
		m_pRenderer->DrawBox( mat , minPos , maxPos );
}