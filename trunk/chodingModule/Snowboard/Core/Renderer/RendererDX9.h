#pragma once

#include "RendererBase.h"

class CRendererDX9 : public CRendererBase{

private:

	typedef struct CUSTOMVERTEX{
		float x,y,z,rhw;
		DWORD color;
	};
	
	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

	HRESULT					initvb();

public:

	CRendererDX9();
	virtual ~CRendererDX9();
	
	virtual	void			Clear();
	virtual HRESULT			Create( HWND hWnd );
	virtual HRESULT			Release();

	virtual void			UpdateFrame();


	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

};