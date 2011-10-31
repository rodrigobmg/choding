#pragma once

#include <string>
#include <dae\daeElement.h>

#include "..\IComponent.h"
#include "..\..\Common\Type\RendererType.h"

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
	std::string				Name;
	std::vector<Vertex>		Vertices;
	std::vector<uint32>		Indices;
};

