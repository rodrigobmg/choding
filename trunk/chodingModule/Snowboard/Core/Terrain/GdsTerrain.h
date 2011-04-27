#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

#include "../Base/GdsObject.h"

class GdsTerrain : public GdsObject
{	

public:
	GdsTerrain();
	virtual ~GdsTerrain();
};

typedef boost::shared_ptr< GdsTerrain >	GdsTerrainPtr;

#endif