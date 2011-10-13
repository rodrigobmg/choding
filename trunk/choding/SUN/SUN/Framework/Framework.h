#ifndef _SUN_FRAMEWORK_H_
#define _SUN_FRAMEWORK_H_

class DLL_API sunFramework
{
public:
	sunFramework();
	virtual ~sunFramework();


public:
	virtual bool CreateCoreManager();
	virtual int  OnInitialize() {return 1;}
	virtual void ShutDown();
	virtual bool MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void Run( HWND hWnd );
	void ProcLoop();

protected:
	HWND			 m_hWnd;

	sunGraphicCore*  m_pGraphicCore;
	sunSceneCore*	 m_pSceneCore;

	bool		     m_bExitFlag;

};


















#endif