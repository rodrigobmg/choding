#include "stdafx.h"
#include "StaticMesh.h"


StaticMesh::StaticMesh(void)
{
}


StaticMesh::~StaticMesh(void)
{
}

void StaticMesh::SetComponent( int32 id )
{

}

void StaticMesh::combineComponents()
{
	for(unsigned int i = 0; i < Positions.size(); i++)
	{
		Vertices.push_back(Vertex(Positions[i], Normals[i], UVs[i], Tangents[i], BiTangents[i]));
	}
}
