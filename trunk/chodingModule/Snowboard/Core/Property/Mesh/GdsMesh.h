#ifndef _GDS_CORE_PROPERTY_MESH_H_
#define _GDS_CORE_PROPERTY_MESH_H_

#include "../Base/GdsBaseProperty.h"
#include "../Material/GdsMaterial.h"

class GdsMesh : public GdsBaseProperty
{
	LPDIRECT3DVERTEXBUFFER9	m_vb;
	LPDIRECT3DINDEXBUFFER9	m_ib;

	D3DXMATRIXA16		m_DXmatWorld;

	int						m_VertexSize;
	int						m_FVF;
	bool					m_bDrawAxis;
	int						m_Vertex_Maxcount;
	int						m_Index_Maxcount;

	GdsMaterialPtr			m_Material;

	void					drawAxis( LPDIRECT3DDEVICE9 device );

public:
	GdsMesh();
	virtual ~GdsMesh();

	void				SetMT( D3DXMATRIXA16& mt ){ m_DXmatWorld = mt; }
	D3DXMATRIXA16		GetMT(){ return m_DXmatWorld; }

	LPDIRECT3DINDEXBUFFER9		GetIB(){ return m_ib; }
	LPDIRECT3DVERTEXBUFFER9		GetVB(){ return m_vb; }

	LPDIRECT3DINDEXBUFFER9*		GetIBBuffer(){ return &m_ib; }
	LPDIRECT3DVERTEXBUFFER9*	GetVBBuffer(){ return &m_vb; }

	void					SetVertexSize( int isize ){ m_VertexSize = isize; }
	void					SetVertexMaxCount( int icount ){ m_Vertex_Maxcount = icount; }
	void					SetFVF( int ifvf ){ m_FVF = ifvf; }

	void					SetIndexMaxCount( int size ){ m_Index_Maxcount = size; }

	void					SetMaterial( GdsMaterialPtr material ){ m_Material = material; }
	GdsMaterialPtr			GetMaterial(){ return m_Material; }

	void				SetDrawAxis( bool bflag ){ m_bDrawAxis = bflag; }

protected:

	void				vRender( LPDIRECT3DDEVICE9 device );
};

typedef boost::shared_ptr< GdsMesh > GdsMeshPtr;

#endif