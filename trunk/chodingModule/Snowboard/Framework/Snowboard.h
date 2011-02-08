#pragma once

#include "Snowboard_stdafx.h"

#include "../Core/CoreMgr.h"
#include "../Core/Renderer/RendererDX9.h"
#include "../Core/Resource/ResMgr.h"

class CSnowboard
{
public:
	CSnowboard();
	virtual ~CSnowboard();

	bool		InitModule();
	void		DestroyModule();
	void		Update();
	HRESULT		MsgProc();

	bool		InitRenderer( HWND hWnd );
	bool		InitCamera();
	bool		InitResource();

	void		Clear();

	void		TestFunc();
private:
	
	CRendererDX9*	m_pRendererDX9;
	CResMrg*		m_pResMgr;
};
