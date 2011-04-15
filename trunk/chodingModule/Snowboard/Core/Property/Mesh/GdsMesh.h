#ifndef _GDS_CORE_PROPERTY_MESH_H_
#define _GDS_CORE_PROPERTY_MESH_H_

#include "../GdsProperty.h"

class GdsMesh : public GdsProperty
{

	LPDIRECT3DVERTEXBUFFER9			m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9			m_IndexBuffer;


public:
	GdsMesh(){};
	virtual ~GdsMesh(){};
};

typedef boost::shared_ptr< GdsMesh > GdsMeshPtr;

#endif