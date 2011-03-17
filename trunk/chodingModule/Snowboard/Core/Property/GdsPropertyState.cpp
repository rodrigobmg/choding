#include "GdsPropertyState.h"
#include "GdsTextureProperty.h"
#include "GdsPolygonProperty.h"

GdsPropertyState::GdsPropertyState(void):
m_PolygonProperty( GdsPolygonPropertyPtr( (GdsPolygonProperty*)NULL ) ),
m_TextureProperty( GdsTexturePropertyPtr( ( GdsTextureProperty*)NULL ) )
{
	SetName( OBJECT_PROPERTYSTATE );
}

GdsPropertyState::~GdsPropertyState(void)
{
}

GdsTexturePropertyPtr GdsPropertyState::GetTextureProperty()
{
	if ( m_TextureProperty == NULL )
	{
		m_TextureProperty = GdsTexturePropertyPtr( new GdsTextureProperty );
	}
	return m_TextureProperty;
}

GdsPolygonPropertyPtr GdsPropertyState::GetPolygonProperty()
{
	if ( m_PolygonProperty == NULL )
	{
		m_PolygonProperty = GdsPolygonPropertyPtr( new GdsPolygonProperty );
	}
	return m_PolygonProperty;
}

void GdsPropertyState::Render( LPDIRECT3DDEVICE9 device )
{
	if ( m_TextureProperty )
		m_TextureProperty->Render(device);

	if ( m_PolygonProperty )
		m_PolygonProperty->Render(device);
}