#pragma once

#include "ResBaseType.h"

class CResTexture : public CBaseRes{

private:

	LPDIRECT3DTEXTURE9			m_pTexture;

public:
	CResTexture();
	virtual ~CResTexture();

	LPDIRECT3DTEXTURE9			Get() { return m_pTexture; }
	LPDIRECT3DTEXTURE9*			GetPtr() { return &m_pTexture; }

	HRESULT				Load( const wchar_t* name , LPDIRECT3DDEVICE9 device );

	virtual void		Clear();
	virtual HRESULT		Create();
	virtual HRESULT		Release();

};