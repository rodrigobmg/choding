#pragma once

#include "../../Base/Snow.h"

class CResTexture : public CSnow{

private:

	LPDIRECT3DTEXTURE9			m_pTexture;

public:
	CResTexture();
	virtual ~CResTexture();

	LPDIRECT3DTEXTURE9			Get() { return m_pTexture; }
	LPDIRECT3DTEXTURE9*			GetPtr() { return &m_pTexture; }
	virtual	void				Clear();
	virtual void				Create();
	virtual void				Destroy();	

};