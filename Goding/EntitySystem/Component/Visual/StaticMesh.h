#pragma once

#include "..\IComponent.h"

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL StaticMesh : public IComponent
{
public:
	StaticMesh();
	virtual ~StaticMesh(void);

	virtual void onDeviceReset( D3DDevice* pDevice );
	virtual void onDeviceLost();


	D3DVertexBuffer*		m_vertexBuffer;
	D3DIndexBuffer*			m_indexBuffer;
	D3DXEffect*				m_pEffect;
	D3DVertexDeclaration*	m_VertexDeclaration;

	uint32					m_VerticesSize;

	std::vector< Vertex >	Vertices;
	std::vector< uint32 >	Indices;

	//Name of this mesh
	std::string				Name;

	bool					m_bLoaded;
};

