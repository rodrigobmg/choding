#ifndef _GDS_SYSTEM_TIME_H_
#define _GDS_SYSTEM_TIME_H_

namespace GDS
{
	// frame rate counting and display
	int m_iMaxTimer = 30;
	int m_iTimer = 1;
	float m_fCurrentTime = 0.f; // Time reported by system
	float m_fLastTime = -1.f; // Last time reported by system
	float m_fAccumTime = 0.f; // Time elapsed since application start
	float m_fFrameTime = 0.f; // Time elapsed since previous frame
	int m_iClicks = 0;
	float m_fFrameRate = 0.f;
	float m_fLastFrameRateTime = 0.f;
	int m_iLastFrameRateClicks = 0;

	bool m_bUseFixedTime = false;
	bool m_bNVPerfHUD = false;
	float m_fMinFramePeriod = 1.0f / 100.0f;
	float m_fLastFrame = 0.0f;
	float m_fFixedTimeIncrement = 1.0f / 60.0f;
	
	float GetAccumTime(){ return m_fAccumTime; }
	float GetFrameTime(){ return m_fFrameTime; }
	
	void  ResetFrameTimings()
	{
// 		m_fUpdateTime = 0.0f;
// 		m_fCullTime = 0.0f;
// 		m_fRenderTime = 0.0f;
	}

	float GetCurrentTimeInSec()
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

	bool MeasureTime()
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

	void SetMaxFrameRate(float fMax)
	{
		// convert from rate to period - if a bogus max framerate is passed in,
		// disable framerate limitation
		if(fMax < 1e-5f)
			m_fMinFramePeriod = 1e-5f;    
		else
			m_fMinFramePeriod = 1.0f / fMax;    
	}

	// frame rate counting and display
	int m_iMaxTimer_Render = 30;
	int m_iTimer_Render = 1;
	float m_fCurrentTime_Render = 0.f; // Time reported by system
	float m_fLastTime_Render = -1.f; // Last time reported by system
	float m_fAccumTime_Render = 0.f; // Time elapsed since application start
	float m_fFrameTime_Render = 0.f; // Time elapsed since previous frame
	int m_iClicks_Render = 0;
	float m_fFrameRate_Render = 0.f;
	float m_fLastFrameRateTime_Render = 0.f;
	int m_iLastFrameRateClicks_Render = 0;

	bool m_bUseFixedTime_Render = false;
	bool m_bNVPerfHUD_Render = false;
	float m_fMinFramePeriod_Render = 1.0f / 100.0f;
	float m_fLastFrame_Render = 0.0f;
	float m_fFixedTimeIncrement_Render = 1.0f / 60.0f;

	float GetAccumTime_Render(){ return m_fAccumTime_Render; }
	float GetFrameTime_Render(){ return m_fFrameTime_Render; }

	void  ResetFrameTimings_Render()
	{
		// 		m_fUpdateTime = 0.0f;
		// 		m_fCullTime = 0.0f;
		// 		m_fRenderTime = 0.0f;
	}

	float GetCurrentTimeInSec_Render()
	{
		static bool bFirst_Render = true;
		static LARGE_INTEGER freq_Render;
		static LARGE_INTEGER initial_Render;

		if (bFirst_Render)
		{
			QueryPerformanceFrequency(&freq_Render);
			QueryPerformanceCounter(&initial_Render);
			bFirst_Render = false;
		}

		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return (float)((long double)
			(counter.QuadPart - initial_Render.QuadPart) / 
			(long double) freq_Render.QuadPart);
	}

	bool MeasureTime_Render()
	{
		if (!m_bUseFixedTime_Render)
		{
			// start performance measurements
			if (m_fLastTime_Render == -1.0f)
			{
				m_fLastTime_Render = GetCurrentTimeInSec_Render();
				m_fAccumTime_Render = 0.0f;
				m_iClicks_Render = 0;
			}

			// measure time
			m_fCurrentTime_Render = GetCurrentTimeInSec_Render();
			float fDeltaTime = m_fCurrentTime_Render - m_fLastTime_Render;

#if defined(WIN32)
			// NVPerfHUD support!
			if (m_bNVPerfHUD_Render && fDeltaTime == 0.0f)
				return true;
#endif

			if (fDeltaTime < 0.0f)
				fDeltaTime = 0.0f;
			m_fLastTime_Render = m_fCurrentTime_Render;
			m_fAccumTime_Render += fDeltaTime;

			// frame rate limiter
			if (m_fAccumTime_Render < (m_fLastFrame_Render + m_fMinFramePeriod_Render))
				return false;

			m_fFrameTime_Render = m_fAccumTime_Render - m_fLastFrame_Render;
			m_fLastFrame_Render = m_fAccumTime_Render;

			return true;
		}
		else
		{
			m_fCurrentTime_Render += m_fFixedTimeIncrement_Render;
			m_fAccumTime_Render = ((float)m_iClicks_Render) * m_fFixedTimeIncrement_Render;      
			m_fFrameTime_Render = m_fAccumTime_Render - m_fLastFrame_Render;
			m_fLastFrame_Render = m_fAccumTime_Render;

			return true;
		}
	}

	void SetMaxFrameRate_Render(float fMax)
	{
		// convert from rate to period - if a bogus max framerate is passed in,
		// disable framerate limitation
		if(fMax < 1e-5f)
			m_fMinFramePeriod_Render = 1e-5f;    
		else
			m_fMinFramePeriod_Render = 1.0f / fMax;    
	}
}

#endif // 