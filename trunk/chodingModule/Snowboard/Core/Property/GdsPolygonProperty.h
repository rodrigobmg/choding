#ifndef _GDS_CORE_PROPERTY_POLYGON_H_
#define _GDS_CORE_PROPERTY_POLYGON_H_

#include "GdsProperty.h"

class GdsPolygonProperty : public GdsProperty
{
public:
	GdsPolygonProperty(void);
	virtual ~GdsPolygonProperty(void);

	LPDIRECT3DVERTEXBUFFER9		GetVBPtr(){ return m_VB; }
	LPDIRECT3DVERTEXBUFFER9*	GetVB(){ return &m_VB; }

	void			SetVB( LPDIRECT3DVERTEXBUFFER9 p );
	
	
	void			SetFVF( uint32_t FVF ){ m_FVF = FVF; }
	DWORD			GetFVF(){ return m_FVF; }

	void			SetVertexFormatSize( int32_t size ){ m_iSizeofVertexFormat = size; }
	int32_t			GetVertexFormatSize(){ return m_iSizeofVertexFormat; }


	LPDIRECT3DINDEXBUFFER9		GetIBPtr(){ return m_IB; }
	LPDIRECT3DINDEXBUFFER9*		GetIB(){ return &m_IB; }

	void			SetIB( LPDIRECT3DINDEXBUFFER9 p );

	void			SetPrimitive( uint32_t uicount ){ m_uiPrimitive = uicount; }
	uint32_t		GetPrimitive(){ return m_uiPrimitive; }
	
	void			Render( LPDIRECT3DDEVICE9 device );
	void			Clear();

protected:

	DWORD			m_FVF;
	int32_t			m_iSizeofVertexFormat;

	LPDIRECT3DVERTEXBUFFER9		m_VB;

	uint32_t		m_uiPrimitive;
	LPDIRECT3DINDEXBUFFER9		m_IB;

};

typedef boost::shared_ptr< GdsPolygonProperty > GdsPolygonPropertyPtr;

#endif
