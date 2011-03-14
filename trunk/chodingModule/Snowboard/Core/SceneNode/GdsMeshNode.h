#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "GdsNode.h"

class GdsMeshNode : public GdsNode{		

protected:

	virtual	void				vInitGeometry( float fElapsedtime );
	virtual void				vRender( float fElapsedtime );
	virtual void				vClear();

public:
	GdsMeshNode();
	virtual ~GdsMeshNode();

};

typedef boost::shared_ptr<GdsMeshNode>	GdsMeshNodePtr;

#endif