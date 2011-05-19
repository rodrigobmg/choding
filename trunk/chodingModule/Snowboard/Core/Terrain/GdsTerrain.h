#ifndef _GDS_CORE_TERRAIN_H_
#define _GDS_CORE_TERRAIN_H_

#include "..\Base\GdsObject.h"

class GdsTerrain : public GdsObject
{

protected:			

	virtual	void							vUpdate( float fElapsedtime );
	virtual void							vClear();

public:

	GdsTerrain();
	virtual ~GdsTerrain();

	void					Update( float fElapsedtime ){ vUpdate(fElapsedtime); }
	void					Clear(){ vClear(); }

};

typedef boost::shared_ptr< GdsTerrain >	GdsTerrainPtr;

#endif