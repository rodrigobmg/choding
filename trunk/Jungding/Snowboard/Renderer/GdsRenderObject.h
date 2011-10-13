#ifndef _SNOWBOARD_RENDERER_RENDEROBJECT_H_
#define _SNOWBOARD_RENDERER_RENDEROBJECT_H_

class GdsRenderObject// : public GdsMemObject

{	
	D3DLIGHT9				m_Light;
	D3DMATERIAL9			m_Material;	
	D3DXMATRIX				m_TM;
	D3DPRIMITIVETYPE		m_ePrimitive;

	LPDIRECT3DTEXTURE9		m_Texture;
	LPDIRECT3DVERTEXBUFFER9	m_vb;
	LPDIRECT3DINDEXBUFFER9	m_ib;
	
	int						m_iTextureIndex;//멀티 텍스쳐링할때 필요한 디바이스의 인덱스
	int						m_VertexSize;
	int						m_FVF;
	int						m_Vertex_Maxcount;
	int						m_iStartVertexIndex;
	int						m_iEndVertexIndex;	
	int						m_Index_Maxcount;
	int						m_iStartIndex;
	int						m_iEndIndex;
	

public:

	GdsRenderObject();
	virtual ~GdsRenderObject();

	virtual void			vRender( LPDIRECT3DDEVICE9 device );
	
	void					SetMaterial( D3DMATERIAL9& material ){ m_Material = material; }
	void					SetLight( D3DLIGHT9& light ){ m_Light = light; }

	void					SetTexture( LPDIRECT3DTEXTURE9 texture ){ m_Texture = texture; }
	void					SetTextureIndex( int index ){ m_iTextureIndex = index; }

	void					SetMatrix( D3DXMATRIX& tm ){ m_TM = tm; }
	D3DXMATRIX				GetMatrix(){ return m_TM; }

	void					SetVertexBuffer( LPDIRECT3DVERTEXBUFFER9 vb ){ m_vb = vb; }
	LPDIRECT3DVERTEXBUFFER9	GetVertexBuffer(){ return m_vb; }

	void					SetVertexSize( int ivertexsize ){ m_VertexSize = ivertexsize; }
	void					SetFVF( int ifvf ){ m_FVF = ifvf; }
	void					SetVertexMaxCount( int icount ){ m_Vertex_Maxcount = icount; }
	int						GetVertexMaxCount(){ return m_Vertex_Maxcount; }
	void					SetStartVertexIndex( int index ){ m_iStartVertexIndex = index; }
	void					SetEndVertexIndex( int index ){ m_iEndVertexIndex = index; }

	void					SetIndexBuffer( LPDIRECT3DINDEXBUFFER9	ib ){ m_ib = ib; }
	LPDIRECT3DINDEXBUFFER9	GetIndexBuffer(){ return m_ib; }

	void					SetPrimitive( D3DPRIMITIVETYPE type ){ m_ePrimitive = type; }
	D3DPRIMITIVETYPE		GetPrimitive(){ return m_ePrimitive; }
	void					SetIndexMaxCount( int count ){ m_Index_Maxcount = count; }
	int						GetIndexMaxCount(){ return m_Index_Maxcount; }
	void					SetStartIndex( int index ){ m_iStartIndex = index; }
	void					SetEndIndex( int index ){ m_iEndIndex = index; }

	DeclearBoostpool

};

#endif