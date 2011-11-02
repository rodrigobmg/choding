#pragma once

#include <string>
#include <dae\daeElement.h>

#include "Vertex.h"
#include "..\IComponent.h"

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL StaticMesh : public IComponent
{
public:
	StaticMesh(){};
	StaticMesh( std::string& name );
	virtual ~StaticMesh(void);


	virtual void			SetComponent( int32 id );
	
	//Name of this mesh
	std::string		Name;

	//Combine the component vertex data to Vertices array
	void combineComponents();
	
	//Component TempVertex Data, to be compiled into Vertices later...
	std::vector<Vec3> Positions;
	std::vector<Vec2> UVs;
	std::vector<Vec3> Normals;
	std::vector<Vec3> Tangents;
	std::vector<Vec3> BiTangents;

	//Stride size
	static const int STRIDE_SIZE = (sizeof(DWORD) * 14);
	std::vector<Vertex> Vertices;

	//Index data, ready for Index Buffer
	std::vector<uint32> Indices;
	
};
