#include "GdsPolygonProperty.h"

GdsPolygonProperty::GdsPolygonProperty(void):
m_VB(NULL),
m_IB(NULL)
{
	SetName( OBJECT_PROPERTY_POLYGON );
	SetType( POLYGON );
	Clear();
}

GdsPolygonProperty::~GdsPolygonProperty(void)
{
	Clear();
}

void GdsPolygonProperty::Clear()
{
	SAFE_RELEASE( m_VB );
	SAFE_RELEASE( m_IB );
}

void GdsPolygonProperty::Render( LPDIRECT3DDEVICE9 device )
{
	device->SetFVF( m_FVF );
	device->SetStreamSource( 0, m_VB , 0, m_iSizeofVertexFormat );	
	if ( m_IB )
	{
		//device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_uiPrimitive );
	}
	else
	{
		device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_uiPrimitive );
	}
}