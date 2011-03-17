#ifndef _GDS_CORE_PROPERTY_IV_H_
#define _GDS_CORE_PROPERTY_IV_H_

#include "GdsProperty.h"

class GdsIndexBufferProperty : public GdsProperty
{
public:
	GdsIndexBufferProperty(void);
	virtual ~GdsIndexBufferProperty(void);

protected:
	
	LPDIRECT3DINDEXBUFFER9 m_IB;

	virtual void	vUpdate(float fElapsedTime );
	virtual void	vClear();
};

typedef boost::shared_ptr< GdsIndexBufferProperty > GdsIndexBufferPropertyPtr;

#endif