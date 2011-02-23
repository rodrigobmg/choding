#pragma once

#include "RendererBase.h"

class SceneNode;

class CRendererDX9 : public CRendererBase{

private:
	
 	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
 	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device


public:

	CRendererDX9();
	virtual ~CRendererDX9();
	
	virtual	void			Clear();
	virtual HRESULT			Create( HWND hWnd );
	virtual HRESULT			Release();

	virtual void			Render( SceneNode* pNode );
	LPDIRECT3DDEVICE9		GetDevice()		{ return m_pd3dDevice; }

};