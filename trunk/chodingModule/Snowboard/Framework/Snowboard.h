#ifndef _GDS_H_MAIN_
#define _GDS_H_MAIN_

#include "Snowboard_stdafx.h"

#include "../Core/GdsCoreFactory.h"
#include "../Core/Renderer/GdsRendererDX9.h"
#include "../Core/Resource/GdsResMgr.h"

class CSnowboard
{
public:
	CSnowboard();
	virtual ~CSnowboard();

	bool		InitModule( HWND hWnd );
	void		DestroyModule();
	
	void		OnIdle();
	HRESULT		MsgProc(  HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam  );

	bool		InitRenderer( HWND hWnd );
	bool		InitResource( LPDIRECT3DDEVICE9 device );
	bool		InitRootNode();
	bool		InitCamMgr();

	void		Clear();

	void		TestFunc();

	float		GetFrameRate(){ return m_fFrameRate; }
	int			GetCountofRenderObject(){ return m_iRenderobjectCount; }
	HWND		GetWnd(){ return m_hWnd; }
private:
	
	GdsRendererDX9Ptr	m_pRenderer;
	GdsResMgrPtr		m_pResMgr;
	GdsNodePtr			m_pRootNode;
	
	float				m_fFrameRate;
	int					m_iRenderobjectCount;
	HWND				m_hWnd;

	void		Update( float fAccumTime );
	void		Render();
};

#endif