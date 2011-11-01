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
	D3DXEffect*				m_pEffect;

	uint32					m_VerticesSize;

	uint32					m_vertexCount;
	uint32					m_indexCount;
	uint32					m_vertexSize;

	static const int32 STRIDE_SIZE = (sizeof(DWORD) * 14);

public:
	RenderObject(void);
	virtual ~RenderObject(void);

	virtual void		Draw( D3DDevice* pDevice , D3DXEffect* pEffect );	
};


#endif _RENDERER_RENDEROBJECT_H_