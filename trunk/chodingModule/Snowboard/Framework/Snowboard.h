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
	void		Update();
	HRESULT		MsgProc();

	bool		InitRenderer( HWND hWnd );
	bool		InitCamera();
	bool		InitResource( LPDIRECT3DDEVICE9 device );

	void		Clear();

	void		TestFunc();
private:
	
	GdsRendererDX9Ptr	m_pRenderer;
	GdsResMgrPtr		m_pResMgr;
	GdsNodePtr			m_pRootNode;
};

#endif