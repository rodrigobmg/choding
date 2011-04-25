#ifndef _GDS_H_RENDEROBJECT_H_
#define _GDS_H_RENDEROBJECT_H_

#include "../Base/GdsObject.h"
#include "../SceneNode/GdsNode.h"

class GdsRenderObject : public GdsObject{	

public:

	GdsRenderObject(){ SetName( OBJECT_RENDEROBJECT ); }
	virtual ~GdsRenderObject(){};

protected:

	LPDIRECT3DVERTEXBUFFER9	m_vb;
	LPDIRECT3DINDEXBUFFER9	m_ib;

	int						m_VertexSize;
	int						m_FVF;
	int						m_Vertex_Maxcount;

	int						m_Index_Maxcount;

};

#endif