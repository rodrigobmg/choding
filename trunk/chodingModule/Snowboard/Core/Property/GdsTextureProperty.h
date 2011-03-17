#ifndef _GDS_CORE_PROPERTY_TEXTURE_H_
#define _GDS_CORE_PROPERTY_TEXTURE_H_

#include "GdsProperty.h"

class GdsTextureProperty : public GdsProperty
{
public:
	GdsTextureProperty(void);
	virtual ~GdsTextureProperty(void);

protected:

	LPDIRECT3DTEXTURE9		m_Tex;

	virtual void	vUpdate( float fElapsedTime );
	virtual void	vClear();
};

typedef boost::shared_ptr< GdsTextureProperty > GdsTexturePropertyPtr;

#endif