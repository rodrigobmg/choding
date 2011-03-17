#ifndef _GDS_CORE_PROPERTY_VB_H_
#define _GDS_CORE_PROPERTY_VB_H_

#include "GdsProperty.h"

class GdsVertexBufferProperty : public GdsProperty
{
public:
	GdsVertexBufferProperty(void);
	virtual ~GdsVertexBufferProperty(void);

protected:

	LPDIRECT3DVERTEXBUFFER9		m_VB;

	virtual	void	vUpdate( float fElapsedTime );
	virtual void	vClear();
};

typedef boost::shared_ptr< GdsVertexBufferProperty > GdsVertexBufferPropertyPtr;

#endif
