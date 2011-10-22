#pragma once

#include "IMesh.h"

class SkinnedMesh: public IMesh
{
public:
	SkinnedMesh();
	virtual ~SkinnedMesh();

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect );
	virtual void Update( float fAccumtime );
};

