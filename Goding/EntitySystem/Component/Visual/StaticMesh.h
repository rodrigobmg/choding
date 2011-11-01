#pragma once

#include <string>
#include <dae\daeElement.h>

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
	
	D3DVertexBuffer*		m_vertexBuffer;
	D3DIndexBuffer*			m_indexBuffer;
	D3DXEffect*				m_pEffect;

	uint32					m_VerticesSize;

	std::vector< Vertex >	Vertices;
	std::vector< uint32 >	Indices;

	//Name of this mesh
	std::string				Name;
};

