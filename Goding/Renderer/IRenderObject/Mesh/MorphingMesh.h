#ifndef _RENDERER_MESH_MORPHINGMESH_H_
#define _RENDERER_MESH_MORPHINGMESH_H_

#include "IMesh.h"

class MorphingMesh : public IMesh
{
public:
	MorphingMesh();
	virtual ~MorphingMesh();

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect );
	virtual void Update( float fAccumtime );
};
#endif