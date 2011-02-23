#pragma once

#include "Snowboard_stdafx.h"

#include "../Core/CoreFactory.h"
#include "../Core/Renderer/RendererDX9.h"
#include "../Core/Resource/ResMgr.h"

class CSnowboard
{
public:
	CSnowboard();
	virtual ~CSnowboard();

	bool		InitModule( HWND hWnd );
	void		DestroyModule();
	void		Update();
	HRESULT		MsgProc();

	bool		InitRenderer( HWND hWnd );
	bool		InitCamera();
	bool		InitResource( LPDIRECT3DDEVICE9 device );

	void		Clear();

	void		TestFunc();
private:
	
	CRendererBase*	m_pRendererBase;
	CResMrg*		m_pResMgr;
};
