#include "stdafx.h"
#include "IMesh.h"

IMesh::IMesh()
{
	//Set Name
	//this->Name = Name;

	//Set World
	//this->World = World;

	//Initialize Component Vertex Data arrays
	Positions = std::vector<Vec3>();
	UVs = std::vector<Vec2>();
	Normals = std::vector<Vec3>();
	Tangents = std::vector<Vec3>();
	BiTangents = std::vector<Vec3>();

	//Initialize Combined Vertex Data array
	Vertices = std::vector<Vertex>();

	//Initialize Index Data Array
	Indices = std::vector<unsigned int>();

	//Initialize COLLADA pointers to NULL
	geometry = NULL;

	//Initialize DirectX resources to NULL
	vertexBuffer = NULL;
	indexBuffer = NULL;
}

IMesh::~IMesh()
{

}

void IMesh::onDeviceReset()
{

}

void IMesh::onDeviceLost()
{

}
