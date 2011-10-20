#pragma once

#include "..\IComponent.h"

class ComRenderObject : IComponent
{
public:
	ComRenderObject(void);
	virtual ~ComRenderObject(void);
	virtual void			SetComponent( com_id id );

	LIGHT				m_Light;
	MATERIAL			m_Material;	
	
	PRIMITIVETYPE		m_ePrimitive;

	LPTEXTURE		m_Texture;
	LPVERTEXBUFFER	m_vb;
	LPINDEXBUFFER	m_ib;

	int						m_iTextureIndex;//멀티 텍스쳐링할때 필요한 디바이스의 인덱스
	int						m_VertexSize;
	int						m_FVF;
	int						m_Vertex_Maxcount;
	int						m_iStartVertexIndex;
	int						m_iEndVertexIndex;	
	int						m_Index_Maxcount;
	int						m_iStartIndex;
	int						m_iEndIndex;

	
	virtual void			vRender( LPDEVICE device );

	void					SetMaterial( MATERIAL& material ){ m_Material = material; }
	void					SetLight( LIGHT& light ){ m_Light = light; }

	void					SetTexture( LPTEXTURE texture ){ m_Texture = texture; }
	void					SetTextureIndex( int index ){ m_iTextureIndex = index; }
	
	void					SetVertexBuffer( LPVERTEXBUFFER vb ){ m_vb = vb; }
	LPVERTEXBUFFER			GetVertexBuffer(){ return m_vb; }

	void					SetVertexSize( int ivertexsize ){ m_VertexSize = ivertexsize; }
	void					SetFVF( int ifvf ){ m_FVF = ifvf; }
	void					SetVertexMaxCount( int icount ){ m_Vertex_Maxcount = icount; }
	int						GetVertexMaxCount(){ return m_Vertex_Maxcount; }
	void					SetStartVertexIndex( int index ){ m_iStartVertexIndex = index; }
	void					SetEndVertexIndex( int index ){ m_iEndVertexIndex = index; }

	void					SetIndexBuffer( LPINDEXBUFFER	ib ){ m_ib = ib; }
	LPINDEXBUFFER			GetIndexBuffer(){ return m_ib; }

	void					SetPrimitive( PRIMITIVETYPE type ){ m_ePrimitive = type; }
	PRIMITIVETYPE			GetPrimitive(){ return m_ePrimitive; }
	void					SetIndexMaxCount( int count ){ m_Index_Maxcount = count; }
	int						GetIndexMaxCount(){ return m_Index_Maxcount; }
	void					SetStartIndex( int index ){ m_iStartIndex = index; }
	void					SetEndIndex( int index ){ m_iEndIndex = index; }
};

