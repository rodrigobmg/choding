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

	//<geometry> element to get mesh data from
	daeElement* geometry;

	//Component Vertex Data, to be compiled into Vertices later...
	vector<D3DXVECTOR3> Positions;
	vector<D3DXVECTOR2> UVs;
	vector<D3DXVECTOR3> Normals;
	vector<D3DXVECTOR3> Tangents;
	vector<D3DXVECTOR3> BiTangents;
	
	//Combine the component vertex data to Vertices array
	void combineComponents()
	{
		for(unsigned int i = 0; i < Positions.size(); i++)
		{
			Vertices.push_back(TempVertex(Positions[i], Normals[i], UVs[i], Tangents[i], BiTangents[i]));
		}
	}

	//Stride size
	static const int STRIDE_SIZE = (sizeof(DWORD) * 14);
	
public:
	//Name of this mesh
	string Name;

	//World transform
	D3DXMATRIX World;

	//Combined Vertex Data, ready for Vertex Buffer
	vector<TempVertex> Vertices;

	//Index data, ready for Index Buffer
	vector<unsigned int> Indices;

	//Constructor
	TempMesh(string Name, D3DXMATRIX World)
	{
		//Set Name
		this->Name = Name;
		//Set World
		this->World = World;
		//Initialize COLLADA pointers to NULL
		geometry = NULL;

	}

	//Destructor
	~TempMesh()
	{
	}

	//Draw
// 	void Draw(IDirect3DDevice9* Device, ID3DXEffect* effect)
// 	{
// 		//Set vertex buffer
// 		Device->SetStreamSource(0, vertexBuffer, 0, STRIDE_SIZE);
// 
// 		//Set index buffer
// 		Device->SetIndices(indexBuffer);
// 
// 		//Set parameters
// 		effect->SetMatrix("World", &World);
// 
// 		//Begin drawing
// 		effect->Begin(NULL, NULL);
// 		effect->BeginPass(0);
// 
// 		//Draw
// 		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Vertices.size(), 0, Indices.size() / 3);
// 
// 		//End drawing
// 		effect->EndPass();
// 		effect->End();
// 	}
};
