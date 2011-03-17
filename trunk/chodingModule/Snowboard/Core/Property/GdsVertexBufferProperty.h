#ifndef _GDS_CORE_PROPERTY_VB_H_
#define _GDS_CORE_PROPERTY_VB_H_

#include "GdsProperty.h"

class GdsVertexBufferProperty : public GdsProperty
{
public:
	GdsVertexBufferProperty(void);
	virtual ~GdsVertexBufferProperty(void);

protected:

	virtual	void	vUpdate( float fElapsedTime );
};

#endif
