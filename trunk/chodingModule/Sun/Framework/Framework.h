#pragma once


class Sun_CFramewrok
{
public:
	Sun_CFramewrok();
	virtual ~Sun_CFramewrok();

public:
	virtual bool CreateCoreManager();
	virtual int  OnInitialize()	{return 1;}
	virtual void ShutDown();
	virtual bool MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void Run( HWND hWnd);
	void ProcLoop();

protected:
	HWND m_hWnd;

	Sun_CGraphicCore* m_pGraphicCore;

	bool m_bExitFlag;
};