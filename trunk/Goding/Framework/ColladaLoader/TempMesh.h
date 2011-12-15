#pragma once

#include <string>
#include <vector>

#include "dae.h"
#include "dom.h"
#include "TempVertex.h"

using namespace std;

class TempMesh
{
public:

	daeElement* geometry;

	vector<D3DXVECTOR3> Positions;
	vector<D3DXVECTOR2> UVs;
	vector<D3DXVECTOR3> Normals;
	vector<D3DXVECTOR3> Tangents;
	vector<D3DXVECTOR3> BiTangents;
	
	void combineComponents()
	{
		for(unsigned int i = 0; i < Positions.size(); i++)
		{
			Vertices.push_back(TempVertex(Positions[i], Normals[i], UVs[i], Tangents[i], BiTangents[i]));
		}
	}

	static const int STRIDE_SIZE = (sizeof(DWORD) * 14);
	
public:

	string Name;
	D3DXMATRIX World;
	vector<TempVertex> Vertices;
	vector<unsigned int> Indices;

	TempMesh(string Name, D3DXMATRIX World)
	{
		this->Name = Name;
		this->World = World;
		geometry = NULL;

	}

	~TempMesh()
	{
	}
};
