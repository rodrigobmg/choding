#ifndef _GDS_CORE_PROPERTY_POLYGON_H_
#define _GDS_CORE_PROPERTY_POLYGON_H_

#include "../Base/GdsBaseProperty.h"

class GdsPolygon : public GdsBaseProperty
{
public:
	GdsPolygon(){};
	virtual ~GdsPolygon(){};

};

typedef boost::shared_ptr< GdsPolygon > GdsPolygonPtr;

#endif