#ifndef _RENDERER_RENDEROBJECT_H_
#define _RENDERER_RENDEROBJECT_H_
#pragma once

#include "../../Common/Type/CommonType.h"
#include "../../Common/Type/RendererType.h"
#include "../stdafx.h"
#include "IRenderObject.h"
#include <vector>

class RenderObject : public IRenderObject
{

private:

	D3DVertexBuffer*		m_vertexBuffer;
	D3DIndexBuffer*			m_indexBuffer;

	uint32					m_VerticesSize;

	std::vector< Vertex >	Vertices;
	std::vector< uint32 >	Indices;

	static const int32 STRIDE_SIZE = (sizeof(DWORD) * 14);

public:
	RenderObject(void);
	virtual ~RenderObject(void);

	virtual void		Draw( D3DDevice* pDevice , D3DXEffect* pEffect );
	virtual void		onDeviceReset( D3DDevice* pDevice );
	virtual void		onDeviceLost();

	void				SetVerticesSize( uint32 size ){ m_VerticesSize = size; }
	uint32				GetVerticesSize(){ return m_VerticesSize; }
	
	std::vector< Vertex >&	GetVertices(){ return Vertices; }
	std::vector< uint32 >&	GetIndices(){ return Indices; }

	void				PushVertex( Vertex& vertex );
	void				PushIndex( uint32& index );

};


#endif _RENDERER_RENDEROBJECT_H_