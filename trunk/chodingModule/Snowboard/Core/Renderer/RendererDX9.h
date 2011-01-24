#pragma once

#include "RendererBase.h"

class CRendererDX9 : public CRendererBase{

private:

	
	virtual void begin();
	virtual void end();
	virtual void render();

	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device

public:

	CRendererDX9();
	virtual ~CRendererDX9();

	
	virtual HRESULT Initialize( HWND hWnd );	
	virtual void UpdateFrame();

	LPDIRECT3DDEVICE9		GetDevice()	{ return m_pd3dDevice; }

};