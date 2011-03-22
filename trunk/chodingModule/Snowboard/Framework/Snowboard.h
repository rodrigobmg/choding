#ifndef _GDS_H_MAIN_
#define _GDS_H_MAIN_


//메모리릭 탐지 체크
//#define _CRTDBG_MAP_ALLOC

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
	HRESULT		MsgProc();

	bool		InitRenderer( HWND hWnd );
	bool		InitResource( LPDIRECT3DDEVICE9 device );

	void		Clear();

	void		TestFunc();

	float		GetFrameRate(){ return m_fFrameRate; }

private:
	
	GdsRendererDX9Ptr	m_pRenderer;
	GdsResMgrPtr		m_pResMgr;
	
	float		m_fFrameRate;
};

#endif