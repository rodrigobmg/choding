#ifndef _IMESH_H_
#define _IMESH_H_

#include <vector>

#include "dae.h"
#include "dom.h"

#include "Vertex.h"
#include "..\IRenderObject.h"
#include "..\..\..\Common\Type\type.h"


class IMesh : public IRenderObject
{
public:
	IMesh();
	virtual ~IMesh();

	virtual void onDeviceReset();
	virtual void onDeviceLost();

	daeElement* geometry;

	//Component Vertex Data, to be compiled into Vertices later...
	std::vector<Vec3> Positions;
	std::vector<Vec2> UVs;
	std::vector<Vec3> Normals;
	std::vector<Vec3> Tangents;
	std::vector<Vec3> BiTangents;

	//Set it so COLLADALoader can access privates
	//friend class COLLADALoader;

	//Combine the component vertex data to Vertices array
	void combineComponents()
	{
		for(unsigned int i = 0; i < Positions.size(); i++)
		{
			Vertices.push_back(Vertex(Positions[i], Normals[i], UVs[i], Tangents[i], BiTangents[i]));
		}
	}

	//Vertex Buffer
	IDirect3DVertexBuffer9* vertexBuffer;

	//Index Buffer
	IDirect3DIndexBuffer9* indexBuffer;

	//Stride size
	static const int STRIDE_SIZE = (sizeof(DWORD) * 14);
	std::vector<Vertex> Vertices;

	//Index data, ready for Index Buffer
	std::vector<uint32> Indices;

};

#endif