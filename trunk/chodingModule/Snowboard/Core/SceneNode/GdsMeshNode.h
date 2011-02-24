#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "GdsNode.h"

class GdsMeshNode : public GdsNode{		

protected:
	
	virtual HRESULT vUpdate( float fElapsedtime );	

public:
	GdsMeshNode();
	virtual ~GdsMeshNode();

	
};

typedef boost::shared_ptr<GdsMeshNode>	GdsMeshNodePtr;

#endif