#pragma once

#include "ResBaseType.h"

class CResTexture : public CBaseRes{

private:

	LPDIRECT3DTEXTURE9			m_pTexture;

public:
	CResTexture();
	virtual ~CResTexture();
	CResTexture& operator=(const CResTexture& rhs)
	{
		m_pTexture = rhs.m_pTexture;
	}

	LPDIRECT3DTEXTURE9			Get() { return m_pTexture; }
	LPDIRECT3DTEXTURE9*			GetPtr() { return &m_pTexture; }

	virtual void		Clear();
	virtual HRESULT		Create();
};