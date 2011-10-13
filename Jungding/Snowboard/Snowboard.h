#ifndef _SNOWBOARD_MAIN_H_
#define _SNOWBOARD_MAIN_H_



class SnowBoard
{
public:
	SnowBoard();
	virtual ~SnowBoard();
	
	void			OnIdle();
	void			MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	void			Init( HWND hwnd );
	void			Destroy();	

	// time
	float			GetAccumTime(){ return m_fAccumTime; }
	float			GetFrameTime(){ return m_fFrameTime; }
	void			ResetFrameTimings();
	float			GetCurrentTimeInSec();
	bool			MeasureTime();
	void			SetMaxFrameRate(float fMax);

private:

	GdsGameObject*	m_pRootNode;

	// frame rate counting and display
	int m_iMaxTimer;
	int m_iTimer;
	float m_fCurrentTime; // Time reported by system
	float m_fLastTime; // Last time reported by system
	float m_fAccumTime; // Time elapsed since application start
	float m_fFrameTime; // Time elapsed since previous frame
	int m_iClicks;
	float m_fFrameRate;
	float m_fLastFrameRateTime;
	int m_iLastFrameRateClicks;

	bool m_bUseFixedTime;
	bool m_bNVPerfHUD;
	float m_fMinFramePeriod;
	float m_fLastFrame;
	float m_fFixedTimeIncrement;

};

#endif