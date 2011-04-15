#ifndef _GDS_CORE_PROPERTY_GEOMETRY_H_
#define _GDS_CORE_PROPERTY_GEOMETRY_H_

#include "../GdsProperty.h"

class GdsGeometry : public GdsProperty
{
public:
	GdsGeometry(){};
	virtual ~GdsGeometry(){};
};

typedef boost::shared_ptr< GdsGeometry > GdsGeometryPtr;

#endif