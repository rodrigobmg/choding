#include "GdsRendererManager.h"
#include "GdsRendererBase.h"
#include "..\..\System\Thread\GdsThreadPool.h"
#include "InputSystem\GdsInputSystem.h"

GdsRendererManager::GdsRendererManager()
:m_pRenderer(NULL)
,m_IsCurRendering(true)
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

void GdsRendererManager::Render( float fAcuumTime )
{
	if ( m_IsCurRendering == false )
	{
		//렌더링 중이므로 백버퍼 초기화
		m_pRenderer->GetRenderFrame()->ClearBackFrameBuffer();
		return;
	}
	
	SetRenderingEnable( false );

	GdsBGThread* thread = THREADPOOL.GetThread( 0 );
	if ( thread == NULL )
		return;

	if ( INPUTSYSTEM.GetKeyIsDown( VK_P ) )
		m_pRenderer->ToggleWireMode();

	m_pRenderer->GetRenderFrame()->Swap_buffer();
	//void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	thread->Push( m_pRenderer , 0.f , &GdsRendererBase::Render );
	//m_pRenderer->Render( 0.f );	
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