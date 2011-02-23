#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "SceneNode.h"

class MeshNode : public SceneNode{		

protected:
	
	virtual HRESULT vUpdate( float fElapsedtime );	

public:
	MeshNode();
	virtual ~MeshNode();

	
};

#endif