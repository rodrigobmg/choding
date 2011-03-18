#ifndef _GDS_H_RESTEXTURE_
#define _GDS_H_RESTEXTURE_

#include "GdsResBaseType.h"

class GdsResTexture : public GdsResBase{
	
public:
	DeclareBoostPool	

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
