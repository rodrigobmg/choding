#include "stdafx.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh( std::string& Name , Matrix44 world )
{
	this->Name = Name;
	this->World = world;
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
