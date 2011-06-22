#include "GdsRendererManager.h"
#include "GdsRendererBase.h"
#include "..\..\System\Thread\GdsThreadPool.h"
#include "InputSystem\GdsInputSystem.h"

GdsRendererManager::GdsRendererManager()
:m_pRenderer(NULL)
,m_IsCurRendering(true)
,m_ProcessTick(0)
,m_CountRenderObject(0)
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
		//������ ���̹Ƿ� ����� �ʱ�ȭ
		m_pRenderer->GetRenderFrame()->ClearBackFrameBuffer();
		return false;
	}
	
	SetRenderingEnable( false );

	GdsBGThread* thread = THREADPOOL.GetThread( 0 );
	if ( thread == NULL )
		return false;

	if ( INPUTSYSTEM.GetKeyIsDown( VK_P ) )
		m_pRenderer->ToggleWireMode();

	m_ProcessTick = thread->GetProcessTick();
	m_CountRenderObject = m_pRenderer->GetRenderFrame()->GetRenderObjectCount();
	m_pRenderer->GetRenderFrame()->Swap_buffer();
	//void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	thread->Push( m_pRenderer , fAccumtime , &GdsRendererBase::Render );
	//m_pRenderer->Render( 0.f );	
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