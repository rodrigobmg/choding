#ifndef _JUNGDING_APPMAIN_H_
#define _JUNGDING_APPMAIN_H_

class Jungding
{
public:
	Jungding( HWND hWnd );
	virtual ~Jungding();

public:
	static LRESULT	CALLBACK MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	
	void			Init();
	void			Destory();
	void			MsgIdle();

	static Jungding*	ms_pAppWin;

private:
	
	void			OnIdle();

	SnowBoard*		m_pSnowBoard;	
	HWND			m_hWnd;	

};

#endif