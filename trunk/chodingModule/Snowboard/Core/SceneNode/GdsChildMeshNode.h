#ifndef _SNOWBOARD_SCENENODE_CHILDMESH_
#define _SNOWBOARD_SCENENODE_CHILDMESH_

#include "GdsMeshNode.h"

class GdsChildMeshNode : public GdsMeshNode{		

protected:
	
	virtual HRESULT vUpdate( float fElapsedtime );	

public:
	GdsChildMeshNode();
	virtual ~GdsChildMeshNode();

	
};

typedef boost::shared_ptr< GdsChildMeshNode >	GdsChildMeshNodePtr;

#endif