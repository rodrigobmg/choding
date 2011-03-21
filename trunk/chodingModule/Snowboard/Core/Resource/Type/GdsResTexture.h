#ifndef _GDS_H_RESTEXTURE_
#define _GDS_H_RESTEXTURE_

#include "GdsResBaseType.h"

class GdsResTexture : public GdsResBase{
	
public:
	//DeclareBoostPool(GdsResTexture)

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsResTexture();
	virtual ~GdsResTexture();

	LPDIRECT3DTEXTURE9		GetTexturePtr(){ return m_pTex; }
	LPDIRECT3DTEXTURE9*		GetTexture(){ return &m_pTex; }

private:

	LPDIRECT3DTEXTURE9		m_pTex;
	
};

typedef boost::shared_ptr< GdsResTexture >	GdsResTexturePtr;

#endif
