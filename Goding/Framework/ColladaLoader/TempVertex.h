#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class TempVertex
{
public:
	
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 BiTangent;

	TempVertex(D3DXVECTOR3 Position, D3DXVECTOR3 Normal, D3DXVECTOR2 UV, D3DXVECTOR3 Tangent, D3DXVECTOR3 BiTangent)
	{
		this->Position = Position;
		this->Normal = Normal;
		this->UV = UV;
		this->Tangent = Tangent;
		this->BiTangent = BiTangent;
	}
};
