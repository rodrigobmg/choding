#ifndef _GDS_CORE_PROPERTY_MESH_H_
#define _GDS_CORE_PROPERTY_MESH_H_

#include "../GdsProperty.h"

class GdsMesh : public GdsProperty
{
public:
	GdsMesh(){};
	~GdsMesh(){};
};

typedef boost::shared_ptr< GdsMesh > GdsMeshPtr;

#endif