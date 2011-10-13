#ifndef _SUN_TESTAPP_APPMAIN_TESTAPP_H_
#define _SUN_TESTAPP_APPMAIN_TESTAPP_H_


class TestApp
{
public:
	TestApp();
	virtual ~TestApp();

public:
	static LRESULT	CALLBACK MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	
	bool			CreateCoreManager();
	bool			OnInitialize();

	bool			Start(HWND hWnd);
	void			End();


public:
	void			MsgLoop();

private:
	HWND			m_hWnd;

	sunGraphicCore*  m_pGraphicCore;
	sunSceneCore*	 m_pSceneCore;


};

extern	TestApp* g_pkTestApp;




#endif