#pragma once

#ifndef _H_SNOWBOARD_MAIN_
#define _H_SNOWBOARD_MAIN_

#include "Snowboard_stdafx.h"

class CObject
{
public:

	CObject()
	{
		vInitialize();
		IncRefCount();
	};

	virtual ~CObject()
	{
		DecRefCount();
		if ( m_iRefCount != 0 )
			assert( "0�϶� ����" );
	};

	void	IncRefCount(){ ++m_iRefCount; }
	void	DecRefCount(){ --m_iRefCount; }
	int32_t GetRefCount(){ return m_iRefCount; }

private:

	void	vInitialize(){ m_iRefCount = 0; }
	int32_t m_iRefCount;

};


class CSnowboard : public CObject
{
public:
	CSnowboard(){};
	virtual ~CSnowboard(){};

	HRESULT	Init( HWND hWnd );
	void	Create();
	void	Release();

	LPDIRECT3DDEVICE9		GetDevice() { return m_pd3dDevice; }

protected:
private:

	LPDIRECT3D9             m_pD3D;      // = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;// = NULL; // Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;       // = NULL; // Buffer to hold vertices
	LPDIRECT3DTEXTURE9      m_pTexture;  // = NULL; // Our texture

};

#endif	_H_SNOWBOARD_MAIN_