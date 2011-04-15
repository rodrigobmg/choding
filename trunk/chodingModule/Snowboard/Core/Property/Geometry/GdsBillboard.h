#ifndef _GDS_CORE_PROPERTY_BILLBOARD_H_
#define _GDS_CORE_PROPERTY_BILLBOARD_H_

#include "../GdsGeometry.h"

class GdsBillboard : public GdsGeometry
{
public:
	GdsBillboard(){};
	virtual ~GdsBillboard(){};
};

typedef boost::shared_ptr< GdsBillboard > GdsBillboardPtr;

#endif