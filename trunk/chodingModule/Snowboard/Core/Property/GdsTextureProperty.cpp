#include "GdsTextureProperty.h"

GdsTextureProperty::GdsTextureProperty(void):
m_Tex(NULL)
{
	SetName( OBJECT_PROPERTY_TEXTURE );
	SetType( TEXTURE );
	Clear();
}

GdsTextureProperty::~GdsTextureProperty(void)
{
	Clear();
}

void GdsTextureProperty::Clear()
{
	if ( m_Tex )
	{
		m_Tex->Release();
		m_Tex = NULL;
	}
}

void GdsTextureProperty::Render( LPDIRECT3DDEVICE9 device )
{
	device->SetTexture( 0, m_Tex );  	
}