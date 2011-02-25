#ifndef _GDS_H_RESTEXTURE_
#define _GDS_H_RESTEXTURE_

#include "GdsResBaseType.h"

class GdsResTexture : public GdsResBase{
	
public:
	DeclareBoostPool

private:

	LPDIRECT3DTEXTURE9			m_pTexture;

public:
	GdsResTexture();
	virtual ~GdsResTexture();
	GdsResTexture& operator=(const GdsResTexture& rhs)
	{
		m_pTexture = rhs.m_pTexture;
	}

	LPDIRECT3DTEXTURE9			Get() { return m_pTexture; }
	LPDIRECT3DTEXTURE9*			GetPtr() { return &m_pTexture; }

	virtual void		Clear();
	virtual HRESULT		Create();
	virtual HRESULT		Release();
		
};

typedef boost::shared_ptr< GdsResTexture >	GdsResTexturePtr;

#endif