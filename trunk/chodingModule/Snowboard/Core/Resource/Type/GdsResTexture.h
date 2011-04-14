#ifndef _GDS_H_RESTEXTURE_
#define _GDS_H_RESTEXTURE_

#include "../GdsResBaseType.h"

class GdsResTexture : public GdsResBase{
private:

	LPDIRECT3DTEXTURE9			m_pTexture;

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsResTexture();
	virtual ~GdsResTexture();	

	LPDIRECT3DTEXTURE9			Get(){ return m_pTexture; }
};

typedef boost::shared_ptr< GdsResTexture >	GdsResTexturePtr;

#endif
