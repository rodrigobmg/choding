#pragma once

#include "IMesh.h"

class StaticMesh : public IMesh
{
public:
	StaticMesh(void);
	virtual ~StaticMesh(void);

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect );
	virtual void Update( float fAccumtime );
};

