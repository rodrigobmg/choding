#ifndef _GDS_CORE_PROPERTY_MESH_H_
#define _GDS_CORE_PROPERTY_MESH_H_

#include "../Base/GdsBaseProperty.h"

class GdsMesh : public GdsBaseProperty
{

	LPDIRECT3DVERTEXBUFFER9			m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9			m_IndexBuffer;

	bool				m_bDrawAxis;

	void				drawAxis( LPDIRECT3DDEVICE9 device );

public:
	GdsMesh();
	virtual ~GdsMesh();

	void				SetDrawAxis( bool bflag ){ m_bDrawAxis = bflag; }

protected:

	void				vRender( LPDIRECT3DDEVICE9 device );
};

typedef boost::shared_ptr< GdsMesh > GdsMeshPtr;

#endif