#ifndef _GDS_CORE_PROPERTY_TEXTURE_H_
#define _GDS_CORE_PROPERTY_TEXTURE_H_

#include "GdsProperty.h"

class GdsTextureProperty : public GdsProperty
{
public:
	GdsTextureProperty(void);
	virtual ~GdsTextureProperty(void);

protected:

	virtual void	vUpdate( float fElapsedTime );
};

#endif