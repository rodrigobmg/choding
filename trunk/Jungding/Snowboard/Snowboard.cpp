#include "stdafx.h"

SnowBoard::SnowBoard()
: m_pRootNode(NULL)
{
	//Init();
}

SnowBoard::~SnowBoard()
{
	Destroy();
}

void SnowBoard::Init( HWND hwnd )
{
	LOG_INFO_F( "Jungding engine init" );

	m_iMaxTimer = 30;
	m_iTimer = 1;
	m_fCurrentTime = 0.f; // Time reported by system
	m_fLastTime = -1.f; // Last time reported by system
	m_fAccumTime = 0.f; // Time elapsed since application start
	m_fFrameTime = 0.f; // Time elapsed since previous frame
	m_iClicks = 0;
	m_fFrameRate = 0.f;
	m_fLastFrameRateTime = 0.f;
	m_iLastFrameRateClicks = 0;

	m_bUseFixedTime = false;
	m_bNVPerfHUD = false;
	m_fMinFramePeriod = 1.0f / 100.0f;
	m_fLastFrame = 0.0f;
	m_fFixedTimeIncrement = 1.0f / 60.0f;

	RENDERER.Create( hwnd );
	RESMGR.Create( RENDERER.GetRenderer9()->GetDevice() );
	CAMMGR.Init();

	m_pRootNode = new GdsGameObject( GAMEOBJECT_DUMMY );
	GdsComVisualRect* pComp_Rect = new GdsComVisualRect;
	m_pRootNode->SetComponent( pComp_Rect );
}

void SnowBoard::Destroy()
{
	LOG_INFO_F( "Jungding engine destroy" );
	SAFE_DELETE( m_pRootNode );
}

void SnowBoard::OnIdle()
{
	if( !MeasureTime() )
		return;

	CAMMGR.Update( m_fAccumTime );

	if ( m_pRootNode )
		m_pRootNode->Update( m_fAccumTime );

	RENDERER.Render( m_fAccumTime );
}

void SnowBoard::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	INPUTSYSTEM.Record( hWnd , message , wParam , lParam );
}

	
void  SnowBoard::ResetFrameTimings()
{
// 	m_fUpdateTime = 0.0f;
// 	m_fCullTime = 0.0f;
// 	m_fRenderTime = 0.0f;
}

float SnowBoard::GetCurrentTimeInSec()
{
	static bool bFirst = true;
	static LARGE_INTEGER freq;
	static LARGE_INTEGER initial;

	if (bFirst)
	{
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&initial);
		bFirst = false;
	}

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (float)((long double)
		(counter.QuadPart - initial.QuadPart) / 
		(long double) freq.QuadPart);
}

bool SnowBoard::MeasureTime()
{
	if (!m_bUseFixedTime)
	{
		// start performance measurements
		if (m_fLastTime == -1.0f)
		{
			m_fLastTime = GetCurrentTimeInSec();
			m_fAccumTime = 0.0f;
			m_iClicks = 0;
		}

		// measure time
		m_fCurrentTime = GetCurrentTimeInSec();
		float fDeltaTime = m_fCurrentTime - m_fLastTime;

#if defined(WIN32)
		// NVPerfHUD support!
		if (m_bNVPerfHUD && fDeltaTime == 0.0f)
			return true;
#endif

		if (fDeltaTime < 0.0f)
			fDeltaTime = 0.0f;
		m_fLastTime = m_fCurrentTime;
		m_fAccumTime += fDeltaTime;

		// frame rate limiter
		if (m_fAccumTime < (m_fLastFrame + m_fMinFramePeriod))
			return false;

		m_fFrameTime = m_fAccumTime - m_fLastFrame;
		m_fLastFrame = m_fAccumTime;

		return true;
	}
	else
	{
		m_fCurrentTime += m_fFixedTimeIncrement;
		m_fAccumTime = ((float)m_iClicks) * m_fFixedTimeIncrement;      
		m_fFrameTime = m_fAccumTime - m_fLastFrame;
		m_fLastFrame = m_fAccumTime;

		return true;
	}
}

void SnowBoard::SetMaxFrameRate(float fMax)
{
	// convert from rate to period - if a bogus max framerate is passed in,
	// disable framerate limitation
	if(fMax < 1e-5f)
		m_fMinFramePeriod = 1e-5f;    
	else
		m_fMinFramePeriod = 1.0f / fMax;    
}