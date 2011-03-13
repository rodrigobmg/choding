#ifndef _SNOWBOARD_SCENENODE_MESH_
#define _SNOWBOARD_SCENENODE_MESH_

#include "GdsNode.h"

class GdsMeshNode : public GdsNode{		

protected:

	virtual	void				vInitGeometry();
	virtual void				vRender();
	virtual void				vClear();

	void						Triangle();

	struct CUSTOMVERTEX
	{
		float x,y,z;
		DWORD color;
	};

	LPDIRECT3DVERTEXBUFFER9		g_pVB;

public:
	GdsMeshNode();
	virtual ~GdsMeshNode();

};

typedef boost::shared_ptr<GdsMeshNode>	GdsMeshNodePtr;

#endif