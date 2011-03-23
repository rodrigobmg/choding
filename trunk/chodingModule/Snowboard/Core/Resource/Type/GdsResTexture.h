#ifndef _GDS_H_RESTEXTURE_
#define _GDS_H_RESTEXTURE_

#include "GdsResBaseType.h"
#include "../../Property/GdsTextureProperty.h"

class GdsResTexture : public GdsResBase{
private:

	GdsTexturePropertyPtr		m_TextureProperty;

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource(LPDIRECT3DDEVICE9 device);

public:
	GdsResTexture();
	virtual ~GdsResTexture();	
};

typedef boost::shared_ptr< GdsResTexture >	GdsResTexturePtr;

#endif
