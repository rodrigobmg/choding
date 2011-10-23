#pragma once

#include "..\IComponent.h"

#include <dae\daeElement.h>
#include "Vertex.h"


class StaticMesh : public IComponent
{
public:
	StaticMesh(void);
	virtual ~StaticMesh(void);
	virtual void			SetComponent( int32 id );
	
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

	//Stride size
	static const int STRIDE_SIZE = (sizeof(DWORD) * 14);
	std::vector<Vertex> Vertices;

	//Index data, ready for Index Buffer
	std::vector<uint32> Indices;

};

