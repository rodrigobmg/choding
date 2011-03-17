#ifndef _GDS_CORE_PROPERTY_IV_H_
#define _GDS_CORE_PROPERTY_IV_H_

#include "GdsProperty.h"

class GdsIndexBufferProperty : public GdsProperty
{
public:
	GdsIndexBufferProperty(void);
	virtual ~GdsIndexBufferProperty(void);

protected:

	virtual void	vUpdate(float fElapsedTime );
};

#endif