#pragma once

//#include "Snowboard_stdafx.h"


#include <d3dx9.h>
#include <d3d9.h>

class CSnowboard
{
public:
	CSnowboard(){};
	virtual ~CSnowboard(){};

	HRESULT	Init( HWND hWnd );
	/*
	void	Create();
		void	Release();
	
		LPDIRECT3DDEVICE9		GetDevice() { return m_pd3dDevice; }*/
	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;       // = NULL; // Buffer to hold vertices
	LPDIRECT3DTEXTURE9      m_pTexture;  // = NULL; // Our texture

	void	Run();

protected:
private:

};
