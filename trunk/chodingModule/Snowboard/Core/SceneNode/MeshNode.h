#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "SceneNode.h"

class MeshNode : public SceneNode{		

public:
	MeshNode();
	virtual ~MeshNode();

	virtual HRESULT vUpdate( float fElapsedtime );	
};

#endif