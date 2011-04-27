#ifndef _GDS_H_RENDEROBJECT_H_
#define _GDS_H_RENDEROBJECT_H_

#include "../Base/GdsObject.h"

class GdsRenderObject : public GdsObject
{	
	D3DMATERIAL9			m_Material;
	D3DLIGHT9				m_Light;

	LPDIRECT3DTEXTURE9		m_Texture;
	int						m_iTextureIndex;//멀티 텍스쳐링할때 필요한 디바이스의 인덱스

	D3DXMATRIX				m_TM;

	LPDIRECT3DVERTEXBUFFER9	m_vb;
	int						m_VertexSize;
	int						m_FVF;
	int						m_Vertex_Maxcount;
	int						m_iStartVertexIndex;
	int						m_iEndVertexIndex;

	LPDIRECT3DINDEXBUFFER9	m_ib;
	int						m_Index_Maxcount;
	int						m_iStartIndex;
	int						m_iEndIndex;

	bool					m_bCull;

public:

	GdsRenderObject();
	virtual ~GdsRenderObject();

	virtual void			vRender( LPDIRECT3DDEVICE9 device );
	
	void					SetMaterial( D3DMATERIAL9& material ){ m_Material = material; }
	void					SetLight( D3DLIGHT9& light ){ m_Light = light; }

	void					SetTexture( LPDIRECT3DTEXTURE9 texture ){ m_Texture = texture; }
	void					SetTextureIndex( int index ){ m_iTextureIndex = index; }

	void					SetMatrix( D3DXMATRIX& tm ){ m_TM = tm; }

	void					SetVertexBuffer( LPDIRECT3DVERTEXBUFFER9 vb ){ m_vb = vb; }
	void					SetVertexSize( int ivertexsize ){ m_VertexSize = ivertexsize; }
	void					SetFVF( int ifvf ){ m_FVF = ifvf; }
	void					SetVertexMaxCount( int icount ){ m_Vertex_Maxcount = icount; }
	void					SetStartVertexIndex( int index ){ m_iStartVertexIndex = index; }
	void					SetEndVertexIndex( int index ){ m_iEndVertexIndex = index; }

	void					SetIndexBuffer( LPDIRECT3DINDEXBUFFER9	ib ){ m_ib = ib; }
	void					SetIndexMaxCount( int count ){ m_Index_Maxcount = count; }
	void					SetStartIndex( int index ){ m_iStartIndex = index; }
	void					SetEndIndex( int index ){ m_iEndIndex = index; }

	void					SetCull( bool flag ){ m_bCull = flag; }
	bool					GetCull(){ return m_bCull; }

protected:		

};

typedef boost::shared_ptr< GdsRenderObject > GdsRenderObjectPtr;

#endif